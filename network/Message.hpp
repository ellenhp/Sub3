#pragma once

#include <map>
#include <boost/serialization/map.hpp>
#include <boost/serialization/export.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "../simulation/VesselState.hpp"

#include "../simulation/VesselID.hpp"

class Ocean;

class Message
{
    //Serialization boilerplate
    friend class Ocean;
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & otherData;
    }

public:
    Message();

    //This tells the packet to do whatever it needs to do to accomplish its mission.
    virtual void execute() = 0;

    std::map<std::string, std::string> otherData;
};
