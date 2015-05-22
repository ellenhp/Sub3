#pragma once

#include "network/Message.hpp"
#include "simulation/VesselID.hpp"

class SetCurrentVesselMessage : public Message
{
    //Serialization boilerplate
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<Message>(*this);
        ar & mVesselID;
    }

public:
    SetCurrentVesselMessage();
    SetCurrentVesselMessage(VesselID vesselID);

    void execute();

private:
    VesselID mVesselID;
};
