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

#include <future>
#include <memory>
#include <thread>
#include <mutex>
#include <chrono>
#include <map>

#include <SFML/Network/TcpListener.hpp>

#include "simulation/PlayerID.hpp"
#include "simulation/VesselID.hpp"
#include "network/SubSocket.hpp"

typedef std::chrono::duration<double, std::ratio<1, 10>> network_interval;

class SubSocket;

class SubServer
{
public:
    //Tell the server where to listen.
    SubServer(uint16_t port);

    //Start and stop the server.
    void start();
    void stop();

    //Wait until the server exits.
    void wait();

    //Check to see if the server is running.
    bool isRunning();

    //Get the port we're listening on.
    uint16_t getPort();

private:
    sf::TcpListener mListener;
    std::vector<std::shared_ptr<SubSocket> > mSockets;

    uint16_t mPort;

    std::thread mServerThread;
    bool mKeepRunning;
    std::mutex mKeepRunningMutex;

    bool mIsRunningVal;
    uint32_t mNextPlayerID;

    std::map<PlayerID, std::shared_ptr<SubSocket>> mClients;

    void serverLoop();

    void spawnVesselForPlayer(PlayerID player);
    void despawnVessel(VesselID vessel);

    bool sendMessageToPlayer(PlayerID player, std::shared_ptr<Message> message);
    void kickPlayer(PlayerID player);
};
