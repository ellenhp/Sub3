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

#include "Sub3.hpp"
#include "simulation/PlayerID.hpp"

#include <ostream>

#include <boost/serialization/export.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

//Stores the ID of any vessel, including its spawning player's ID.
class VesselID
{
    //Serialization boilerplate
    friend class Ocean;
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & mPlayer;
        ar & mPlayerVesselID;
    }

public:
    VesselID(PlayerID player, uint32_t playerVesselID);
    VesselID();

    PlayerID getPlayer() const;

    uint32_t getPlayerVesselID() const;

    friend bool operator<(const VesselID& left, const VesselID& right);
    friend bool operator>(const VesselID& left, const VesselID& right);
    friend bool operator==(const VesselID& left, const VesselID& right);

    friend std::ostream& operator<<(std::ostream& stream, VesselID& vesselID);

private:
    PlayerID mPlayer;
    uint32_t mPlayerVesselID;
};
