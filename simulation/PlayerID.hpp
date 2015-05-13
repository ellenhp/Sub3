#pragma once

#include "../Sub3.hpp"
#include <boost/serialization/export.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>


//Encapsulates a unit32_t.
class PlayerID
{
    //Serialization boilerplate
    friend class Ocean;
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & playerID;
    }

public:
    PlayerID();
    PlayerID(uint32_t playerID);

    bool isServer() const;

    uint32_t getID() const;

    operator uint32_t() const
    {
        return playerID;
    }

    friend std::ostream& operator<<(std::ostream& stream, PlayerID& playerID);

private:
    uint32_t playerID;
};
