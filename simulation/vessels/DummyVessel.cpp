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

#include "DummyVessel.hpp"

//IMPORTANT: This lets Boost serialize our spawn message.
REGISTER_PACKETS(DummyVessel, "DummyVessel")

DummyVessel::DummyVessel()
{
}

VesselState DummyVessel::getNewState(float dt)
{
    return mState;
}

std::vector<std::shared_ptr<Message>> DummyVessel::getSpawnMessages(VesselID vesselID)
{
    //Just return the one spawn message. Nothing special here.
    std::vector<std::shared_ptr<Message> > messages;
    messages.push_back(std::make_shared<SpawnMessage<DummyVessel> >(vesselID, mState));
    return messages;
}

std::shared_ptr<Vessel::VesselUI> DummyVessel::constructUI()
{
    return std::make_shared<DummyVessel::UI>();
}

DummyVessel::UI::~UI()
{
}

std::shared_ptr<sfg::Widget> DummyVessel::UI::setupUI()
{
    return sfg::Label::Create("Hello");
}

void DummyVessel::UI::updateUI(double dt)
{
}
