#pragma once

#include "network/Message.hpp"
#include "simulation/VesselID.hpp"

//Set the current vessel on the client. This lets them decide which UI to show the user.
class SetCurrentVesselMessage : public Message
{
    //Serialization boilerplate.
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
