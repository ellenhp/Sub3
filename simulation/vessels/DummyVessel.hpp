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

//This vessel doesn't do anything. For testing only.
class DummyVessel : public Vessel
{
public:
    DummyVessel();

    class UI;

    VesselState getNewState(float dt);

    std::vector<std::shared_ptr<Message>> getSpawnMessages(VesselID vesselID);

    //Get a dummy UI.
    virtual std::shared_ptr<VesselUI> constructUI();

};

class DummyVessel::UI : public Vessel::VesselUI
{
public:
    virtual ~UI();
    virtual std::shared_ptr<sfg::Widget> setupUI();
    virtual void updateUI(double dt);
};
