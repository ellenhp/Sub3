#pragma once

#include <memory>
#include <thread>
#include <mutex>
#include <chrono>

#include <SFML/Network/TcpListener.hpp>

#include "simulation/PlayerID.hpp"
#include "network/SubSocket.hpp"

typedef std::chrono::duration<int, std::ratio<1, 10>> network_interval;

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

};
