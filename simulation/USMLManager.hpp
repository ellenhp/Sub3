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

#pragma once

#include "simulation/Position.hpp"
#include "simulation/VesselID.hpp"
#include "Sub3.hpp"

#include <thread>
#include <mutex>
#include <vector>
#include <map>

#include <usml/waveq3d/eigenray.h>

typedef std::map<VesselID, std::vector<usml::waveq3d::eigenray>> VesselEigenrayMap;

//This class manages all interaction with USML.
class USMLManager
{
public:
    //This is a singleton.
    static USMLManager* getInstance();

    //Ensures oceanic data is loaded near the specified position.
    //This blocks until loading is finished if waitForLoading is true or if data has not been loaded before in this area.
    void ensureDataAround(Position pos, bool waitForLoading);

    //Starts a thread that will continuously calculate propagation loss.
    void start(VesselID listener, double range = 100000);

    //Checks to see if the USML thread is running.
    bool getRunning();

    //Stops the thread and waits for it to end (blocks for a short while).
    void stop();

private:
    USMLManager();

    //Loads oceanic data around the specified Position.
    void loadDataAround(Position pos);

    void usmlLoop();
    void usmlCalculate(VesselID emitter, std::vector<VesselID> listeners);

    bool getContinuing();
    void setRunning(bool running);

    static USMLManager* inst;

    std::thread mUsmlThread;
    std::mutex mContinueMutex;

    std::map<VesselID, VesselEigenrayMap> mAllEigenrays;
    std::mutex mAllEigenrayMutex;

    bool mContinue;
    bool mRunning;

    //Have we loaded oceanic data yet?
    bool mLoadedData;

    //The center of the area we loaded oceanic data for.
    Position mDataCenter;

    //Mutex for mLoadedData and mDataCenter.
    std::mutex mOceanicDataMutex;

    const double maxTime = 20; //20s * 1500m/s is 30km straight-line. Should be fine for now.
    const double timeStep = 0.1;
    const double loadDistance = 1000000; //1000km should be plenty.
    const double reloadDistance = 200000; //1000km - 200km = 800km nominal minimum distance, which is fine.
    const double minDistance = 5000000; //100km - 500km = 500km absolute minimum, which is fine.
};
