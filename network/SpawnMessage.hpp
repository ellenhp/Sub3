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

#include <boost/serialization/export.hpp>

#include "Sub3.hpp"
#include "network/Message.hpp"
#include "simulation/VesselState.hpp"
#include "simulation/Ocean.hpp"

//Template to allow any vessel to have a spawn message.
template <class VesselClass> class SpawnMessage : public Message
{
    //Serialization boilerplate.
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<Message>(*this);
        ar & mVesselID;
        ar & mInitialState;
    }

public:
    SpawnMessage()
    {
    }

    SpawnMessage(VesselID vesselID, VesselState initialState) :
        mInitialState(initialState), mVesselID(vesselID)
    {
    }

    void execute()
    {
        //Don't run the packet if it wasn't created properly.
        if (mVesselID.getPlayer() == -1)
        {
            subDebug << "SpawnMessage: Player #-1 undefined" << std::endl;
            return;
        }
        std::shared_ptr<VesselClass> vessel = std::make_shared<VesselClass>();
        auto ocean = Ocean::getOcean();
        ocean->localSpawnVessel(mVesselID, vessel);
        ocean->localUpdateVessel(mVesselID, mInitialState);
    }

private:
    VesselState mInitialState;
    VesselID mVesselID;

};
