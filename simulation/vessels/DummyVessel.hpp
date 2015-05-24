#pragma once

#include "simulation/Vessel.hpp"

//This vessel doesn't do anything. For testing only.
class DummyVessel : public Vessel
{
public:
    DummyVessel();

    class UI;

    VesselState getNewState(float dt);

    std::vector<std::shared_ptr<Message>> getSpawnMessages(VesselID vesselID);

    //Get a dummy UI.
    virtual std::shared_ptr<VesselUI> constructUI(std::shared_ptr<Vessel> vessel);

};

class DummyVessel::UI : public Vessel::VesselUI
{
public:
    virtual ~UI();
    virtual std::shared_ptr<sfg::Widget> setupUI();
    virtual void updateUI(double dt);
};
