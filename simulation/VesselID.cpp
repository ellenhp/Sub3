#include "VesselID.hpp"

VesselID::VesselID(PlayerID player, uint32_t playerVesselID) :
    mPlayer(player), mPlayerVesselID(playerVesselID)
{
}

VesselID::VesselID()
{
    mPlayerVesselID = -1;
}

PlayerID VesselID::getPlayer()
{
    return mPlayer;
}

uint32_t VesselID::getPlayerVesselID()
{
    return mPlayerVesselID;
}

//This is used to let it be the key of a std::map
bool operator<(const VesselID& left, const VesselID& right)
{
    if (left.mPlayer.getID() != right.mPlayer.getID())
    {
        return left.mPlayer < right.mPlayer;
    }
    else
    {
        return left.mPlayerVesselID < right.mPlayerVesselID;
    }
}

//This is used to let it be the key of a std::map
bool operator>(const VesselID& left, const VesselID& right)
{
    if (left.mPlayer.getID() != right.mPlayer.getID())
    {
        return left.mPlayer < right.mPlayer;
    }
    else
    {
        return left.mPlayerVesselID < right.mPlayerVesselID;
    }
}

std::ostream& operator<<(std::ostream& stream, VesselID& vesselID)
{
    stream << vesselID.mPlayer << "'s vessel " << vesselID.mPlayerVesselID;
    return stream;
}
