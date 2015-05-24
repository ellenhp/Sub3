#pragma once

#include "simulation/Vessel.hpp"

#include <SFGUI/Widgets.hpp>

//A basic surface vessel. It can move, but not much else.
class BasicSubmarine : public Vessel
{
public:
    BasicSubmarine();

    class UI;

    VesselState getNewState(float dt);

    std::vector<std::shared_ptr<Message>> getSpawnMessages(VesselID vesselID);

    void setHeading(double heading);
    void setVelocity(double velocity);
    void setPitch(double pitch);

    //Get a UI for this vessel.
    virtual std::shared_ptr<Vessel::VesselUI> constructUI(std::shared_ptr<Vessel> vessel);
};

class BasicSubmarine::UI : public Vessel::VesselUI
{
public:
    UI(std::weak_ptr<BasicSubmarine> vessel);
    virtual ~UI();

    std::shared_ptr<sfg::Widget> setupUI();
    void updateUI(double dt);

private:
    sfg::SpinButton::Ptr mHeading;
    sfg::SpinButton::Ptr mVelocity;
    sfg::SpinButton::Ptr mPitch;

    sfg::Label::Ptr mLocation;

    std::weak_ptr<BasicSubmarine> mVessel;
};
