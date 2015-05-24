#pragma once

#include "simulation/VesselState.hpp"
#include "network/SpawnMessage.hpp"

#include <SFGUI/Widgets.hpp>

//This macro lets subclasses easily export all the templated messages that pertain to themselves.
#define REGISTER_PACKETS(T, NAME) BOOST_CLASS_EXPORT_GUID(SpawnMessage<T>, "SpawnMessage" NAME)

class Vessel
{
public:
    Vessel();

    class VesselUI;

    //TODO: return a vector of Messages instead of a VesselState.
    virtual VesselState getNewState(float dt) = 0;
    void setState(VesselState state);
    VesselState getState() const;

    //More complex Vessels might require custom initialization that will be generated here.
    virtual std::vector<std::shared_ptr<Message>> getSpawnMessages(VesselID vesselID) = 0;

    //Get the UI for allowing a player to control the vessel passed in.
    //Internally, the UI should create a weak_ptr to the vessel.
    virtual std::shared_ptr<VesselUI> constructUI(std::shared_ptr<Vessel> vessel) = 0;

protected:
    VesselState mState;
};

//This inner class generates and manages the UI screen for controlling a vessel.
class Vessel::VesselUI
{
public:
    //This lets subclasses clean up their mess.
    virtual ~VesselUI() {};

    //This returns a widget that will let the user control the vessel with.
    virtual std::shared_ptr<sfg::Widget> setupUI() = 0;

    //Updates the UI. Subclasses of VesselUI will keep references to their widgets
    //so that they can do things here, like command a new bearing or depth.
    virtual void updateUI(double dt) = 0;
};
