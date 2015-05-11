#include "VesselID.hpp"

VesselID::VesselID(PlayerID player, uint32_t playerVesselID) :
    player(player), playerVesselID(playerVesselID)
{
}

VesselID::VesselID()
{
    playerVesselID = -1;
}

PlayerID VesselID::getPlayer()
{
    return player;
}

uint32_t VesselID::getPlayerVesselID()
{
    return playerVesselID;
}

//This is used to let it be the key of a std::map
bool operator<(const VesselID& left, const VesselID& right)
{
    if (left.player.getID() != right.player.getID())
    {
        return left.player < right.player;
    }
    else
    {
        return left.playerVesselID < right.playerVesselID;
    }
}

//This is used to let it be the key of a std::map
bool operator>(const VesselID& left, const VesselID& right)
{
    if (left.player.getID() != right.player.getID())
    {
        return left.player < right.player;
    }
    else
    {
        return left.playerVesselID < right.playerVesselID;
    }
}
