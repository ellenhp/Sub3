#pragma once

#include "simulation/Vessel.hpp"

#include <SFGUI/Widgets.hpp>

//A basic surface vessel. It can move, but not much else.
class BasicSurfaceVessel : public Vessel
{
public:
    BasicSurfaceVessel();

    class UI;

    VesselState getNewState(float dt);

    std::vector<std::shared_ptr<Message>> getSpawnMessages(VesselID vesselID);

    void setHeading(double heading);
    void setVelocity(double velocity);

    //Get a UI for this vessel.
    virtual std::shared_ptr<VesselUI> constructUI();
};

class BasicSurfaceVessel::UI : public Vessel::VesselUI
{
public:
    UI(BasicSurfaceVessel* vessel);
    virtual ~UI();

    std::shared_ptr<sfg::Widget> setupUI();
    void updateUI(double dt);

private:
    sfg::SpinButton::Ptr mHeading;
    sfg::SpinButton::Ptr mVelocity;

    sfg::Label::Ptr mLocation;

    BasicSurfaceVessel* mVessel;
};
