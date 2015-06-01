/*
 * Sub^3 is a free software submarine simulator focused on realism.
 * Copyright (C) 2015 Nolan Poe
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of  MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "physics/CapsuleShape.hpp"
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

    //Get the capsule that currently bounds this vessel.
    virtual CapsuleShape getBoundingCapsule() const = 0;

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
