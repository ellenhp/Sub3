#include "SubServer.hpp"
#include "../network/SubSocket.hpp"
#include "../network/SetPlayerIDMessage.hpp"
#include "../simulation/Ocean.hpp"
#include "../Sub3.hpp"

#include <SFML/Network/TcpSocket.hpp>

SubServer::SubServer(uint16_t port) :
    port(port), nextPlayerID(1)
{
}

void SubServer::start()
{
    //Don't let the listener block updating clients.
    listener.setBlocking(false);
    listener.listen(port);
    //Run the server in a new thread.
    serverThread = std::thread(&SubServer::serverLoop, this);
    isRunningVal = true;
}

void SubServer::stop()
{
    //Tell the server thread to stop.
    keepRunningMutex.lock();
    keepRunning = false;
    keepRunningMutex.unlock();

    //Wait for the server thread to stop.
    serverThread.join();

    listener.close();
    //TODO send a close message?
    sockets.clear();
}

void SubServer::wait()
{
    //Wait for the server thread to stop on its own (won't happen currently).
    serverThread.join();
}

bool SubServer::isRunning()
{
    return isRunningVal;
}

uint16_t SubServer::getPort()
{
    return listener.getLocalPort();
}

void SubServer::serverLoop()
{
    std::vector<SubSocket> clients;

    keepRunningMutex.lock();
    while (keepRunning)
    {
        keepRunningMutex.unlock();

        //Time to wake up after sleeping.
        auto end_time = std::chrono::steady_clock::now() + network_interval(1);

        auto newSocket = std::make_shared<sf::TcpSocket>();
        while (listener.accept(*newSocket) == sf::Socket::Status::Done)
        {
            //Make a new sub socket
            SubSocket newSubSocket(newSocket);
            auto setPlayerID = std::make_shared<SetPlayerIDMessage>(nextPlayerID);
            nextPlayerID++;

            //Tell it who it is.
            newSubSocket << setPlayerID;

            //Bring it up to speed.
            for (std::shared_ptr<Message> message : Ocean::getOcean()->getInitiationMessages())
            {
                newSubSocket << message;
            }

            //Add it to clients.
            clients.push_back(newSubSocket);
        }

        //TODO update the ocean, run AI.

        for (auto& socket : clients)
        {
            //TODO update all the clients.
        }

        //Sleep for a bit.
        std::this_thread::sleep_until(end_time);

        keepRunningMutex.lock();
    }
}
