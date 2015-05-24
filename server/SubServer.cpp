#include "SubServer.hpp"

#include "Sub3.hpp"
#include "network/SetPlayerIDMessage.hpp"
#include "network/SetCurrentVesselMessage.hpp"
#include "simulation/Ocean.hpp"
#include "simulation/vessels/BasicSubmarine.hpp"

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
    using seconds = std::chrono::duration<float, std::ratio<1, 1>>;

    auto lastUpdate = std::chrono::steady_clock::now();

    //Time to wake up after sleeping.
    auto end_time = std::chrono::steady_clock::now() + network_interval(1);

    mKeepRunningMutex.lock();
    while (mKeepRunning)
    {
        mKeepRunningMutex.unlock();

        auto newSocket = std::make_shared<sf::TcpSocket>();
        while (mListener.accept(*newSocket) == sf::Socket::Status::Done)
        {
            subDebug<<"accepting a new player"<<std::endl;
            //Make a new sub socket
            auto newSubSocket = std::make_shared<SubSocket>(newSocket);
            auto setPlayerID = std::make_shared<SetPlayerIDMessage>(mNextPlayerID);
            PlayerID newPlayerID(mNextPlayerID);
            mNextPlayerID++;

            //Add it to clients.
            mClients[newPlayerID] = newSubSocket;

            //Tell it who it is.
            sendMessageToPlayer(newPlayerID, setPlayerID);

            //Bring it up to speed.
            for (std::shared_ptr<Message> message : Ocean::getOcean()->getInitiationMessages())
            {
                sendMessageToPlayer(newPlayerID, message);
            }

            spawnVesselForPlayer(newPlayerID);

            subDebug << "New player: " << newPlayerID << std::endl;
        }

        //Run the clients' messages.
        for (auto& clientKV : mClients)
        {
            std::shared_ptr<Message> message = NULL;
            while (clientKV.second->hasPackets())
            {
                *clientKV.second >> message;
                message->execute();
            }
        }

        //TODO run AI.

        auto duration = std::chrono::steady_clock::now() - lastUpdate;
        lastUpdate = std::chrono::steady_clock::now();

        auto updateMessages = Ocean::getOcean()->tick(seconds(duration).count());

        for (auto message : updateMessages)
        {
            message->execute();
        }

        for (auto& clientKV : mClients)
        {
            for (auto message : updateMessages)
            {
                bool success = sendMessageToPlayer(clientKV.first, message);

                //If we didn't succeed sending the message, move on to the next client.
                if (!success)
                {
                    kickPlayer(clientKV.first);
                    break;
                }
            }
        }

        //Sleep for a bit.
        std::this_thread::sleep_until(end_time);
        end_time = std::chrono::steady_clock::now() + network_interval(1);

        mKeepRunningMutex.lock();
    }
    mKeepRunningMutex.unlock();
}

void SubServer::spawnVesselForPlayer(PlayerID player)
{
    std::vector<std::shared_ptr<Message>> messages;

    //Guaranteed to be the first vessel spawned by this player
    uint32_t vesselNum = 0;

    VesselID newVesselID(player, vesselNum);

    //Create a new VesselState.
    Position newPos;
    newPos.setLatitude(0);
    newPos.setLongitude(0);
    newPos.setAltitude(0);
    VesselState newState;
    subDebug << "important pos: (" << newPos.getLatitude() << ", " << newPos.getLongitude() << ")" << std::endl;

    //Create a message for spawning the new vessel.
    auto spawnMessage = std::make_shared<SpawnMessage<BasicSubmarine>>(newVesselID, newState);

    spawnMessage->execute();

    //Let everybody know that we're spawning something.
    for (auto& clientKV : mClients)
    {
        sendMessageToPlayer(clientKV.first, spawnMessage);
    }

    //Tell the client which vessel it can control.
    sendMessageToPlayer(player, std::make_shared<SetCurrentVesselMessage>(newVesselID));
}

bool SubServer::sendMessageToPlayer(PlayerID player, std::shared_ptr<Message> message)
{
    BOOST_ASSERT_MSG(mClients.count(player) > 0, "Fatal: Player doesn't exist or disconnected.");
    bool messageResult = (*mClients.at(player) << message);
    return messageResult;
}

void SubServer::kickPlayer(PlayerID player)
{
    BOOST_ASSERT_MSG(mClients.count(player) > 0, "Fatal: Player doesn't exist or disconnected.");
    //TODO: maybe send a disconnect message.
    mClients.erase(player);

    //This causes a segfault or sometimes a double-free.
    //TODO: why?
    //mSendMessageSuccessful.erase(player);

    subDebug << "Kicked player: " << player << " for network problems." << std::endl;
}
