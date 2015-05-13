#pragma once

#include "../Sub3.hpp"
#include "PlayerID.hpp"

#include <ostream>

#include <boost/serialization/export.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

//Stores the ID of any vessel, including its spawning player's ID.
class VesselID
{
    //Serialization boilerplate
    friend class Ocean;
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & player;
        ar & playerVesselID;
    }

public:
    VesselID(PlayerID player, uint32_t playerVesselID);
    VesselID();

    PlayerID getPlayer();

    uint32_t getPlayerVesselID();

    friend bool operator<(const VesselID& left, const VesselID& right);
    friend bool operator>(const VesselID& left, const VesselID& right);

    friend std::ostream& operator<<(std::ostream& stream, VesselID& vesselID);

private:
    PlayerID player;
    uint32_t playerVesselID;
};
