#pragma once

#include "../Vessel.hpp"

//This vessel doesn't do anything. For testing only.
class DummyVessel : public Vessel
{
public:
    DummyVessel();
    VesselState getNewState(float dt);

    std::vector<std::shared_ptr<Message> > getSpawnMessages(VesselID vesselID);

};
