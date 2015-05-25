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

    void loadDataAround(Position pos);

    //Starts a thread that will continuously calculate propagation loss.
    void start(VesselID listener, double range = 100000);

private:
    USMLManager();

    void usmlLoop();
    void usmlCalculate(VesselID emitter, std::vector<VesselID> listeners);

    bool getContinuing();

    static USMLManager* inst;

    std::thread mUsmlThread;
    std::mutex mContinueMutex;

    std::map<VesselID, VesselEigenrayMap> mAllEigenrays;
    std::mutex mAllEigenrayMutex;

    bool mContinue;
    bool mRunning;

    const double maxTime = 60; //60s * 1500m/s is 90km straight-line. Should be fine for now.
    const double timeStep = 0.1;
};
