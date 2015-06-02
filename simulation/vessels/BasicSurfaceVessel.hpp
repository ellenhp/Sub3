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
    virtual std::shared_ptr<VesselUI> constructUI(std::shared_ptr<Vessel> vessel);

    virtual CapsuleShape getBoundingCapsule() const;

private:
    const double mLength = 27; //meters
    const double mBeam = 6; //meters
};

class BasicSurfaceVessel::UI : public Vessel::VesselUI
{
public:
    UI(std::weak_ptr<BasicSurfaceVessel> vessel);
    virtual ~UI();

    std::shared_ptr<sfg::Widget> setupUI();
    void updateUI(double dt, SubWindow& subWindow);

private:
    sfg::SpinButton::Ptr mHeading;
    sfg::SpinButton::Ptr mVelocity;

    sfg::Label::Ptr mLocation;

    std::weak_ptr<BasicSurfaceVessel> mVessel;
};
