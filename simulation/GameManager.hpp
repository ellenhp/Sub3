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

#include <memory>
#include <chrono>

#include "simulation/PlayerID.hpp"
#include "simulation/VesselID.hpp"

class Vessel;
class SubSocket;

//GameManager controls some basic game stuff like who player is and what vessel they own.
class GameManager
{
public:
    //GameManager is not a singleton, but it does have a concept of a current game.
    GameManager();

    //Get/set the current game.
    static std::weak_ptr<GameManager> getCurrent();
    static void setCurrent(std::shared_ptr<GameManager> game);

    //Get basic game information.
    PlayerID getPlayerID();
    VesselID getCurrentVesselID();
    std::shared_ptr<Vessel> getCurrentVessel();
    bool isAlive();

    void setSocket(std::shared_ptr<SubSocket> socket);

    //Has the basic game information been initialized?
    bool isInitialized();

    //Methods to initialize basic game information.
    void setPlayer(PlayerID player);
    void setCurrentVessel(VesselID vessel);

    void startGame();
    void tick(float dt);
    void endGame();

private:
    static std::shared_ptr<GameManager> gameInst;

    std::shared_ptr<SubSocket> mSocket;

    PlayerID mPlayer;
    VesselID mCurrentVessel;

    bool mHasSetPlayer;
    bool mHasSetVessel;

    std::chrono::steady_clock::time_point mLastUpdate;

    uint32_t mNextVesselHandle = 0;

};
