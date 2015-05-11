#pragma once

#include <memory>
#include <thread>
#include <mutex>
#include <chrono>

#include "SFML/Network/TcpListener.hpp"

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
    sf::TcpListener listener;
    std::vector<std::shared_ptr<SubSocket> > sockets;

    uint16_t port;

    std::thread serverThread;
    bool keepRunning;
    std::mutex keepRunningMutex;

    bool isRunningVal;
    uint32_t nextPlayerID;

    void serverLoop();

};
