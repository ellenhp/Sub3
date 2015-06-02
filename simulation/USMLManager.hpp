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
#include "simulation/VesselState.hpp"
#include "Sub3.hpp"

#include <thread>
#include <mutex>
#include <vector>
#include <map>

#include <usml/waveq3d/eigenray.h>


//This class manages all interaction with USML.
class USMLManager
{
public:
    USMLManager();

    //Starts a thread that will continuously calculate propagation loss.
    void start(VesselID listener, double range = 100000);

    //Checks to see if the USML thread is running.
    bool getRunning();

    //Stops the thread and waits for it to end (blocks for a short while).
    void stop();

    //Semantically a listener. Internally this is a source.
    void updateListenerPosition(Position pos);

    //Semantically the sources. Internally these are targets.
    void updateSources(std::map<VesselID, VesselState> targets);

    //Gets a copy of the eigenray map.
    std::map<VesselID, usml::waveq3d::eigenray> getEigenrayMap();

    const double maxTime = 20; //20s * 1500m/s is 30km straight-line. Should be fine for now.

private:
    //Ensures oceanic data is loaded near the specified position.
    void ensureDataAround(Position pos);

    //Loads oceanic data around the specified Position.
    void loadDataAround(Position pos);

    void usmlLoop();
    void usmlCalculate();

    bool getContinuing();
    void setRunning(bool running);

    std::thread mUsmlThread;

    //This to let people tell us to stop.
    std::mutex mContinueMutex;

    //This is to let GameManager update the position to propagate from and the targets to propagate to.
    std::mutex mUpdatePositionsMutex;
    Position mEmitterPosition;
    std::map<VesselID, VesselState> mTargets;

    //These are required to get eigenray information out.
    std::map<VesselID, usml::waveq3d::eigenray> mEigenrayMap;
    std::mutex mEigenrayMutex;

    bool mContinue;
    bool mRunning;

    //Have we loaded oceanic data yet?
    bool mLoadedData;

    //The center of the area we loaded oceanic data for.
    Position mDataCenter;

    const double timeStep = 0.1;
    const double loadDistance = 1000000; //1000km should be plenty.
    const double reloadDistance = 200000; //1000km - 200km = 800km nominal minimum distance, which is fine.
};
