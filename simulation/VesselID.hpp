#pragma once

#include "../Sub3.hpp"
#include "PlayerID.hpp"

//Stores the ID of any vessel, including its spawning player's ID.
class VesselID
{
public:
    VesselID(PlayerID player, uint32_t playerVesselID);
    VesselID();

    PlayerID getPlayer();

    uint32_t getPlayerVesselID();

    friend bool operator<(const VesselID& left, const VesselID& right);
    friend bool operator>(const VesselID& left, const VesselID& right);

private:
    PlayerID player;
    uint32_t playerVesselID;
};
