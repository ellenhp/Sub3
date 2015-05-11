#pragma once

#include "VesselState.hpp"
#include "../network/SpawnMessage.hpp"

//This macro lets subclasses easily export all the templated messages that pertain to themselves.
#define REGISTER_PACKETS(T, NAME) BOOST_CLASS_EXPORT_GUID(SpawnMessage<T>, "SpawnMessage" NAME)

class Vessel
{
public:
    Vessel();

    //TODO: return a vector of Messages instead of a VesselState.
    virtual VesselState getNewState(float dt) = 0;
    void setState(VesselState state);
    VesselState getState();

    //More complex Vessels might require custom initialization that will be generated here.
    virtual std::vector<std::shared_ptr<Message> > getSpawnMessages(VesselID vesselID) = 0;

protected:
    VesselState state;
};
