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

#include "PlayerID.hpp"

PlayerID::PlayerID()
{
    mPlayerID = -1;
}

PlayerID::PlayerID(uint32_t playerID) :
    mPlayerID(playerID)
{
}

bool PlayerID::isServer() const
{
    return mPlayerID == 0;
}

uint32_t PlayerID::getID() const
{
    return mPlayerID;
}

std::ostream& operator<<(std::ostream& stream, PlayerID& playerID)
{
    stream << "Player " << playerID.mPlayerID;
    return stream;
}
