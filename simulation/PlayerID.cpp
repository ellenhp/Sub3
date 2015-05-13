#include "PlayerID.hpp"

PlayerID::PlayerID()
{
    playerID = -1;
}

PlayerID::PlayerID(uint32_t playerID) :
    playerID(playerID)
{
}

bool PlayerID::isServer() const
{
    return playerID == 0;
}

uint32_t PlayerID::getID() const
{
    return playerID;
}

std::ostream& operator<<(std::ostream& stream, PlayerID& playerID)
{
    stream << "Player " << playerID;
    return stream;
}
