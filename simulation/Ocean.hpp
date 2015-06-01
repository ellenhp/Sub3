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

#include <map>
#include <memory>
#include <string>
#include <mutex>

#include "Sub3.hpp"
#include "network/Message.hpp"
#include "simulation/VesselID.hpp"
#include "simulation/PlayerID.hpp"

class Vessel;
class VesselState;
template<class VesselClass> class SpawnMessage;

class Ocean
{
public:
    //Ocean is a singleton. I hope I don't regret that.
    static Ocean* getOcean();

    enum Month
    {
        Undefined = 0,
        January = 1,
        February = 2,
        March = 3,
        April = 4,
        May = 5,
        June = 6,
        July = 7,
        August = 8,
        September = 9,
        October = 10,
        November = 11,
        December = 12
    };

    //Get all the messages required to tick or bring a new client up to speed.
    //For SubServer use only.
    std::vector<std::shared_ptr<Message>> tick(float dt) const;
    std::vector<std::shared_ptr<Message>> getInitiationMessages() const;

    //These methods actually do things.
    //For Message use only.
    void localResetOcean();
    void localSpawnVessel(VesselID id, std::shared_ptr<Vessel> vessel);
    void localDespawnVessel(VesselID id);
    void localUpdateVessel(VesselID id, VesselState state);
    void localSetMonth(Month month);

    Month getMonth() const;
    bool getHasVessel(VesselID id) const;
    VesselState getState(VesselID id) const;
    std::shared_ptr<const Vessel> getVessel(VesselID id) const;

    //This is used by PhysicsEngine
    std::map<VesselID, VesselState> getAllVesselStates();

    //Get all vessel ID's within d meters.
    std::vector<VesselID> getNearestVesselIDs(double d, std::shared_ptr<const Vessel> target = NULL) const;

    void lockAccess();
    void unlockAccess();

    //This lets GameManager give out references to vessels at its discretion.
    friend class GameManager;

private:
    Ocean();
    static Ocean* oceanInst;

    Month mMonth;

    std::map<VesselID, std::shared_ptr<Vessel>> mVessels;

    std::mutex mAccessMutex;

};
