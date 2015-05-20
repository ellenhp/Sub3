#include "PlayerID.hpp"

PlayerID::PlayerID()
{
    mPlayerID = -1;
}

PlayerID::PlayerID(uint32_t playerID) :
    mPlayerID(playerID)
{
}

bool PlayerID::isServer() const
{
    return mPlayerID == 0;
}

uint32_t PlayerID::getID() const
{
    return mPlayerID;
}

std::ostream& operator<<(std::ostream& stream, PlayerID& playerID)
{
    stream << "Player " << playerID.mPlayerID;
    return stream;
}
