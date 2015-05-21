#include "GameManager.hpp"

std::shared_ptr<GameManager> GameManager::gameInst = NULL;

GameManager::GameManager() :
    mHasSetVessel(false), mHasSetPlayer(false)
{
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

VesselID GameManager::getCurrentVessel()
{
    return mCurrentVessel;
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
