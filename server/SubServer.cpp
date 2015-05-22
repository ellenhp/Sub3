#include "SubServer.hpp"

#include "Sub3.hpp"
#include "network/SetPlayerIDMessage.hpp"
#include "network/SetCurrentVesselMessage.hpp"
#include "simulation/Ocean.hpp"
#include "simulation/vessels/DummyVessel.hpp"

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
            auto newSubSocket = std::make_shared<SubSocket>(newSocket);
            auto setPlayerID = std::make_shared<SetPlayerIDMessage>(mNextPlayerID);
            PlayerID newPlayerID(mNextPlayerID);
            mNextPlayerID++;

            //Tell it who it is.
            *newSubSocket << setPlayerID;

            //Bring it up to speed.
            for (std::shared_ptr<Message> message : Ocean::getOcean()->getInitiationMessages())
            {
                *newSubSocket << message;
            }

            //Add it to clients.
            mClients[newPlayerID] = newSubSocket;

            spawnVesselForPlayer(newPlayerID);

            subDebug << "New player: " << newPlayerID << std::endl;
        }

        //TODO update the ocean, run AI.

        for (auto& clientKV : mClients)
        {
            //TODO update all the clients.
        }

        //Sleep for a bit.
        std::this_thread::sleep_until(end_time);

        mKeepRunningMutex.lock();
    }
    mKeepRunningMutex.unlock();
}

void SubServer::spawnVesselForPlayer(PlayerID player)
{
    std::vector<std::shared_ptr<Message>> messages;
    //Create a message for spawning the new vessel.
    VesselID newVesselID(player, 0); //Guaranteed to be the first vessel spawned by this player
    auto spawnMessage = std::make_shared<SpawnMessage<DummyVessel>>(newVesselID, VesselState());

    //Let everybody know that we're spawning something.
    for (auto& clientKV : mClients)
    {
        *clientKV.second << spawnMessage;
    }

    //Tell the client which vessel it can control.
    *mClients[player] << std::make_shared<SetCurrentVesselMessage>(newVesselID);
}
