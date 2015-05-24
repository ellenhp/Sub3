#include "GameManager.hpp"

#include "server/SubServer.hpp"
#include "network/UpdateMessage.hpp"
#include "network/SubSocket.hpp"
#include "simulation/Vessel.hpp"

std::shared_ptr<GameManager> GameManager::gameInst = NULL;

GameManager::GameManager() :
    mSocket(NULL), mHasSetPlayer(false), mHasSetVessel(false)
{
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

void GameManager::setSocket(std::shared_ptr<SubSocket> socket)
{
    BOOST_ASSERT_MSG(!mSocket, "Fatal: Socket already set");
    mSocket = socket;
}

void GameManager::tick(float dt)
{
    auto localUpdateMessages = Ocean::getOcean()->tick(dt);

    for (auto updateMessage : localUpdateMessages)
    {
        updateMessage->execute();
    }

    auto timeSinceUpdate = std::chrono::steady_clock::now() - mLastUpdate;
    if (isInitialized() && timeSinceUpdate > network_interval(1))
    {
        VesselState newState = getCurrentVessel()->getState();
        auto updateMyVessel = std::make_shared<UpdateMessage>(mCurrentVessel, newState);
        *mSocket << updateMyVessel;
        mLastUpdate = std::chrono::steady_clock::now();
    }

}
