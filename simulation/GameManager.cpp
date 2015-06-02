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

#include "GameManager.hpp"

#include "server/SubServer.hpp"
#include "network/UpdateMessage.hpp"
#include "network/SubSocket.hpp"
#include "simulation/USMLManager.hpp"
#include "simulation/Vessel.hpp"

std::shared_ptr<GameManager> GameManager::gameInst = NULL;

GameManager::GameManager() :
    mSocket(NULL), mHasSetPlayer(false), mHasSetVessel(false)
{
    mUsmlManager = std::make_shared<USMLManager>();
    mLastUpdate = std::chrono::steady_clock::now();
}

std::weak_ptr<GameManager> GameManager::getCurrent()
{
    return std::weak_ptr<GameManager>(gameInst);
}

void GameManager::setCurrent(std::shared_ptr<GameManager> game)
{
    gameInst = game;
}

PlayerID GameManager::getPlayerID()
{
    return mPlayer;
}

VesselID GameManager::getCurrentVesselID()
{
    return mCurrentVessel;
}

std::shared_ptr<USMLManager> GameManager::getUsmlManager()
{
    return mUsmlManager;
}

std::shared_ptr<Vessel> GameManager::getCurrentVessel()
{
    BOOST_ASSERT_MSG(isInitialized(), "Fatal: GameManager not initialized in GameManager.getCurrentVessel()");
    return Ocean::getOcean()->mVessels.at(mCurrentVessel);
}

void GameManager::setPlayer(PlayerID player)
{
    mPlayer = player;
    mHasSetPlayer = true;
}

void GameManager::setCurrentVessel(VesselID vessel)
{
    mCurrentVessel = vessel;
    mHasSetVessel = true;
}

bool GameManager::isInitialized()
{
    return mHasSetVessel && mHasSetPlayer;
}

bool GameManager::isAlive()
{
    return Ocean::getOcean()->getHasVessel(mCurrentVessel);
}

void GameManager::setSocket(std::shared_ptr<SubSocket> socket)
{
    BOOST_ASSERT_MSG(!mSocket, "Fatal: Socket already set");
    mSocket = socket;
}

void GameManager::startGame()
{
    subDebug << "Starting game" << std::endl;
    //Kick off the USML thread.
    mUsmlManager->start(getCurrentVesselID());
}

void GameManager::tick(float dt)
{
    auto localUpdateMessages = Ocean::getOcean()->tick(dt);

    for (auto updateMessage : localUpdateMessages)
    {
        updateMessage->execute();
    }

    //Ensure the area around the player is loaded.
    if (isAlive())
    {
        //Let USML know where we are.
        auto currentPos = getCurrentVessel()->getState().getLocation();
        mUsmlManager->updateListenerPosition(currentPos);

        //Now figure out who we're listening for.
        auto sourceIDs = Ocean::getOcean()->getNearestVesselIDs(mUsmlManager->maxTime * 1500);

        //Fill the map.
        std::map<VesselID, VesselState> sources;
        for (auto id : sourceIDs)
        {
            sources[id] = Ocean::getOcean()->getState(id);
        }

        //Remove own vessel.
        sources.erase(mCurrentVessel);

        mUsmlManager->updateSources(sources);
    }

    //Conditionally update the server on our state.
    auto timeSinceUpdate = std::chrono::steady_clock::now() - mLastUpdate;
    if (isInitialized() && timeSinceUpdate > network_interval(1))
    {
        mLastUpdate = std::chrono::steady_clock::now();

        //Loop through all the vessels we own.
        for (auto& vesselKV : Ocean::getOcean()->getAllVesselStates())
        {
            if (vesselKV.first.getPlayer() == getPlayerID())
            {
                VesselState newState = getCurrentVessel()->getState();
                auto updateMyVessel = std::make_shared<UpdateMessage>(mCurrentVessel, newState);
                *mSocket << updateMyVessel;
            }
        }

        //Execute incoming messages.
        std::shared_ptr<Message> message = NULL;
        while (mSocket->hasPackets())
        {
            BOOST_ASSERT_MSG(*mSocket >> message, "Fatal: Failed to load message");
            message->execute();
        }
    }
}

void GameManager::endGame()
{
    using seconds = std::chrono::duration<float>;

    subDebug << "Ending game" << std::endl;

    //Stop the USML thread and record how much time it took.
    auto startTime = std::chrono::high_resolution_clock::now();
    mUsmlManager->stop();
    auto duration = std::chrono::high_resolution_clock::now() - startTime;

    subDebug << "USML thread join took " << seconds(duration).count() << "sec" << std::endl;

    //Clear the ocean.
    Ocean::getOcean()->localResetOcean();
}
