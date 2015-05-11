#pragma once

#include "../Sub3.hpp"

//Encapsulates a unit32_t.
class PlayerID
{
public:
    PlayerID(uint32_t playerID) :
        playerID(playerID)
    { }

    PlayerID()
    {
        playerID = -1;
    }

    bool isServer() const
    {
        return playerID == 0;
    }

    uint32_t getID() const
    {
        return playerID;
    }

    operator uint32_t() const
    {
        return playerID;
    }

private:
    uint32_t playerID;
};
