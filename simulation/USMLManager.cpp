/*
 * Sub^3 is a free software submarine simulator focused on realism.
 * Copyright (C) 2015 Nolan Poe
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of  MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "USMLManager.hpp"

#include "simulation/Ocean.hpp"
#include "simulation/VesselState.hpp"
#include "simulation/GameManager.hpp"

#include <usml/ocean/ocean_shared.h>
#include <usml/waveq3d/waveq3d.h>
#include <usml/netcdf/netcdf_files.h>
#include <usml/waveq3d/wave_queue.h>
#include <usml/waveq3d/eigenray_collection.h>

#include <boost/assert.hpp>

#include <chrono>

using usml::netcdf::netcdf_woa;
using usml::netcdf::netcdf_bathy;
using usml::waveq3d::wave_queue;
using usml::waveq3d::eigenray_collection;
using namespace usml::ocean;

USMLManager* USMLManager::inst = NULL;

USMLManager::USMLManager() :
    mRunning(false)
{
}

USMLManager* USMLManager::getInstance()
{
    if (!inst)
    {
        inst = new USMLManager();
    }
    return inst;
}

//Loads oceanographic data around pos.
void USMLManager::loadDataAround(Position pos)
{
    const double distance = 1000000; //1000km on every side should be plenty.

    //Calculate the amount of data to load.
    Position northLimit(pos, distance, 0);
    Position southLimit(pos, distance, 180);

    Position northeastLimit(northLimit, distance, 90);
    Position southwestLimit(southLimit, distance, 270);

    // const double lat1 = northwestLimit.getLatitude();
    // const double lng1 = northwestLimit.getLongitude();
    //
    // const double lat2 = southeastLimit.getLatitude();
    // const double lng2 = southeastLimit.getLongitude();

    const double lat1 = southwestLimit.getLatitude();
    const double lng1 = southwestLimit.getLongitude();

    const double lat2 = northeastLimit.getLatitude();
    const double lng2 = northeastLimit.getLongitude();

    subDebug << "Loading data from " << lat1 << ", " << lng1 << " to " << lat2 << ", " << lng2 << std::endl;

    //Load temperature and salinity.
    //TODO: Use the correct month.
    netcdf_woa* temperature = new netcdf_woa(
        "usml/woa09/temperature_seasonal_1deg.nc",
        "usml/woa09/temperature_monthly_1deg.nc",
        10, lat1, lat2, lng1, lng2);

    netcdf_woa* salinity = new netcdf_woa(
        "usml/woa09/salinity_seasonal_1deg.nc",
        "usml/woa09/salinity_monthly_1deg.nc",
        10, lat1, lat2, lng1, lng2);

    subDebug << "Loaded temp and salinity" << std::endl;


    //Compute sound speed.
    profile_model* profile = new profile_lock(new profile_grid<double,3>(
        data_grid_mackenzie::construct(temperature, salinity)));

    //Load bathymetry.
    boundary_model* bottom = new boundary_lock(new boundary_grid<double,2>(new netcdf_bathy(
        "usml/bathymetry/ETOPO1_Ice_g_gmt4.grd",
        lat1, lat2, lng1, lng2)));

    subDebug << "Loaded bathymetry" << std::endl;

    //TODO: Get weather data somehow?
    boundary_model* surface = new boundary_lock(new boundary_flat());

    //Updated the ocean_shared reference.
    ocean_shared::reference ocean(new ocean_model(surface, bottom, profile));
    ocean_shared::update(ocean);
}

//Starts a thread that will continuously calculate propagation loss.
void USMLManager::start(VesselID listener, double range)
{
    BOOST_ASSERT_MSG(!getRunning(), "Fatal: USML thread already running");
    mContinue = true;
    mUsmlThread = std::thread(&USMLManager::usmlLoop, this);
}

//Starts a thread that will continuously calculate propagation loss.
void USMLManager::stop()
{
    BOOST_ASSERT_MSG(getRunning(), "Fatal: USML thread not running");

    //Let the thread know it needs to stop.
    mContinueMutex.lock();
    mContinue = false;
    mContinueMutex.unlock();

    //Join the thread (dubious whether this is necessary, TODO).
    mUsmlThread.join();
}

//I'm not sure if accessing a bool is atomic so I'm being safe.
bool USMLManager::getContinuing()
{
    mContinueMutex.lock();
    bool continuing = mContinue;
    mContinueMutex.unlock();
    return continuing;
}

void USMLManager::setRunning(bool running)
{
    //We're using the same mutex for mContinue and mRunning.
    mContinueMutex.lock();
    mRunning = running;
    mContinueMutex.unlock();
}

bool USMLManager::getRunning()
{
    bool running;
    mContinueMutex.lock();
    running = mRunning;
    mContinueMutex.unlock();
    return running;
}

void USMLManager::usmlLoop()
{
    setRunning(true);
    while (getContinuing())
    {
        //Eventually do something more refined than this.
        auto gameManager = GameManager::getCurrent().lock();
        BOOST_ASSERT_MSG(gameManager, "Fatal: GameManager doesn't exist");

        auto ocean = Ocean::getOcean();

        //Copy all the ID's we want to work with into local variables.
        ocean->lockAccess();
        VesselID currentVessel = gameManager->getCurrentVesselID();
        std::vector<VesselID> nearbyVessels = ocean->getNearestVesselIDs(maxTime * 1500);
        ocean->unlockAccess();

        //Don't listen for our own sound.
        nearbyVessels.erase(std::remove(nearbyVessels.begin(), nearbyVessels.end(), currentVessel), nearbyVessels.end());

        if (nearbyVessels.size() > 0)
        {
            usmlCalculate(currentVessel, nearbyVessels);
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    setRunning(false);
}

//This method takes a while. To be called from usmlLoop() only.
void USMLManager::usmlCalculate(VesselID emitter, std::vector<VesselID> listeners)
{
    auto ocean = Ocean::getOcean();

    //These local variables store the positions of the emitter and listener so we don't have to keep locking and unlocking.
    Position emitterPosition;
    std::vector<Position> listenerPositions;

    //Copy the positions to local variables.
    ocean->lockAccess();
    emitterPosition = ocean->getState(emitter).getLocation();
    for (auto& listenerID : listeners)
    {
        listenerPositions.push_back(ocean->getState(listenerID).getLocation());
    }
    ocean->unlockAccess();

    //Ceate the wposition object for the listeners.
    wposition wListeners(listenerPositions.size(), 1);
    for (int i = 0; i < listenerPositions.size(); i++)
    {
        double lat = listenerPositions[i].getLatitude();
        double lng = listenerPositions[i].getLongitude();
        double alt = listenerPositions[i].getAltitude();
        wListeners.latitude(i, 0, lat);
        wListeners.longitude(i, 0, lng);
        wListeners.altitude(i, 0, alt);
    }

    //Define the rays.
    seq_linear de(-85.0, 5, 85.0);
    seq_linear az(-0.0, 10.0, 350.0);

    auto oceanModel = ocean_shared::current();

    wave_queue wave(*oceanModel, FREQ_AXIS, emitterPosition, de, az, timeStep, &wListeners);

    eigenray_collection loss(FREQ_AXIS, emitterPosition, de, az, timeStep, &wListeners);
    wave.add_eigenray_listener(&loss);

    //subDebug << "begin calculating eigenrays" << std::endl;

    while (wave.time() < maxTime)
    {
        wave.step();
        if (!getContinuing())
        {
            return;
        }
    }

    loss.sum_eigenrays();

    //subDebug << "done calculating eigenrays" << std::endl;
    for (int listenerIndex = 0; listenerIndex < listenerPositions.size(); listenerIndex++)
    {
        //subDebug << "total for target " << listenerIndex << std::endl;
        auto totalEigenray = loss.total(listenerIndex, 0);
        auto intensities = totalEigenray->intensity;
        for (int freqIndex = 0; freqIndex < intensities.size(); freqIndex++)
        {
            //subDebug << FREQ_AXIS[freqIndex] << "Hz: " << intensities[freqIndex] << "dB" << std::endl;
        }
    }

}
