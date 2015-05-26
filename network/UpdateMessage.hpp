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

#include "network/Message.hpp"
#include "simulation/VesselState.hpp"
#include "simulation/VesselID.hpp"

class UpdateMessage : public Message
{
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<Message>(*this);
        ar & mVesselID;
        ar & mNewState;
    }

public:
    UpdateMessage();
    UpdateMessage(VesselID vesselID, VesselState newState);

    bool shouldServerSendTo(PlayerID player);

    void execute();

private:
    VesselState mNewState;
    VesselID mVesselID;

};
