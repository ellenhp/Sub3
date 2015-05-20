#pragma once

#include "Message.hpp"
#include "../simulation/VesselState.hpp"

class UpdateMessage : public Message
{
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<Message>(*this);
        ar & mVesselID;
        ar & mNewState;
    }

public:
    UpdateMessage();
    UpdateMessage(VesselID vesselID, VesselState newState);

    void execute();

private:
    VesselState mNewState;
    VesselID mVesselID;

};
