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

#include "SetPlayerIDMessage.hpp"
#include "simulation/GameManager.hpp"

#include <boost/assert.hpp>

//This is necessary for serialization to work correctly.
BOOST_CLASS_EXPORT_GUID(SetPlayerIDMessage, "SetPlayerIDMessage")

SetPlayerIDMessage::SetPlayerIDMessage()
{
    mNewPlayerID = -1;
}

SetPlayerIDMessage::SetPlayerIDMessage(uint32_t newPlayerID) :
    mNewPlayerID(newPlayerID)
{
}

void SetPlayerIDMessage::execute()
{
    //Don't execute the packet if it wasn't constructed properly.
    if (mNewPlayerID != -1)
    {
        auto currentGame = GameManager::getCurrent().lock();
        BOOST_ASSERT_MSG((bool)currentGame, "Fatal: SetPlayerIDMessage.execute() called without a GameManager");
        currentGame->setPlayer(mNewPlayerID);
    }
}
