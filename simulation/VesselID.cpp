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

#include "VesselID.hpp"

VesselID::VesselID(PlayerID player, uint32_t playerVesselID) :
    mPlayer(player), mPlayerVesselID(playerVesselID)
{
}

VesselID::VesselID()
{
    mPlayerVesselID = -1;
}

PlayerID VesselID::getPlayer()
{
    return mPlayer;
}

uint32_t VesselID::getPlayerVesselID()
{
    return mPlayerVesselID;
}

//This is used to let it be the key of a std::map
bool operator<(const VesselID& left, const VesselID& right)
{
    if (left.mPlayer.getID() != right.mPlayer.getID())
    {
        return left.mPlayer < right.mPlayer;
    }
    else
    {
        return left.mPlayerVesselID < right.mPlayerVesselID;
    }
}

//This is used to let it be the key of a std::map
bool operator>(const VesselID& left, const VesselID& right)
{
    if (left.mPlayer.getID() != right.mPlayer.getID())
    {
        return left.mPlayer < right.mPlayer;
    }
    else
    {
        return left.mPlayerVesselID < right.mPlayerVesselID;
    }
}

std::ostream& operator<<(std::ostream& stream, VesselID& vesselID)
{
    stream << vesselID.mPlayer << "'s vessel " << vesselID.mPlayerVesselID;
    return stream;
}
