#include "SubServer.hpp"
#include "../network/SubSocket.hpp"
#include "../network/SetPlayerIDMessage.hpp"
#include "../simulation/Ocean.hpp"
#include "../Sub3.hpp"

#include <SFML/Network/TcpSocket.hpp>

SubServer::SubServer(uint16_t port) :
    mPort(port), mNextPlayerID(1)
{
}

void SubServer::start()
{
    //Don't let the listener block updating clients.
    mListener.setBlocking(false);
    mListener.listen(mPort);
    //Run the server in a new thread.
    mServerThread = std::thread(&SubServer::serverLoop, this);
    mIsRunningVal = true;
}

void SubServer::stop()
{
    //Tell the server thread to stop.
    mKeepRunningMutex.lock();
    mKeepRunning = false;
    mKeepRunningMutex.unlock();

    //Wait for the server thread to stop.
    mServerThread.join();

    mListener.close();
    //TODO send a close message?
    mSockets.clear();
}

void SubServer::wait()
{
    //Wait for the server thread to stop on its own (won't happen currently).
    mServerThread.join();
}

bool SubServer::isRunning()
{
    return mIsRunningVal;
}

uint16_t SubServer::getPort()
{
    return mListener.getLocalPort();
}

void SubServer::serverLoop()
{
    std::vector<SubSocket> clients;

    mKeepRunningMutex.lock();
    while (mKeepRunning)
    {
        mKeepRunningMutex.unlock();

        //Time to wake up after sleeping.
        auto end_time = std::chrono::steady_clock::now() + network_interval(1);

        auto newSocket = std::make_shared<sf::TcpSocket>();
        while (mListener.accept(*newSocket) == sf::Socket::Status::Done)
        {
            //Make a new sub socket
            SubSocket newSubSocket(newSocket);
            auto setPlayerID = std::make_shared<SetPlayerIDMessage>(mNextPlayerID);
            mNextPlayerID++;

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

        mKeepRunningMutex.lock();
    }
    mKeepRunningMutex.unlock();
}
