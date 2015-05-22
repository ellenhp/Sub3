#pragma once

#include <memory>

#include "PlayerID.hpp"
#include "VesselID.hpp"
#include "Vessel.hpp"

//GameManager controls some basic game stuff like who player is and what vessel they own.
class GameManager
{
public:
    //GameManager is not a singleton, but it does have a concept of a current game.
    GameManager();

    //Get/set the current game.
    static std::weak_ptr<GameManager> getCurrent();
    static void setCurrent(std::shared_ptr<GameManager> game);

    //Get basic game information.
    PlayerID getPlayerID();
    VesselID getCurrentVesselID();
    std::shared_ptr<Vessel> getCurrentVessel();

    //Has the basic game information been initialized?
    bool isInitialized();

    //Methods to initialize basic game information.
    void setPlayer(PlayerID player);
    void setCurrentVessel(VesselID vessel);

private:
    static std::shared_ptr<GameManager> gameInst;

    PlayerID mPlayer;
    VesselID mCurrentVessel;

    bool mHasSetPlayer;
    bool mHasSetVessel;

    uint32_t mNextVesselHandle = 0;

};
