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

#include "BasicSurfaceVessel.hpp"

#include <sstream>

//IMPORTANT: This lets Boost serialize our spawn message.
REGISTER_PACKETS(BasicSurfaceVessel, "BasicSurfaceVessel")

BasicSurfaceVessel::BasicSurfaceVessel()
{
}

VesselState BasicSurfaceVessel::getNewState(float dt)
{
    //TODO: This doesn't take into account pitch. It'll work for a surface vessel though.
    double range = dt * mState.getVelocity();
    double bearing = mState.getHeading();
    Position newPos(mState.getLocation(), range, bearing);
    return VesselState(newPos, mState.getVelocity(), mState.getHeading(), mState.getPitch());
}

std::vector<std::shared_ptr<Message>> BasicSurfaceVessel::getSpawnMessages(VesselID vesselID)
{
    //Just return the one spawn message. Nothing special here.
    std::vector<std::shared_ptr<Message> > messages;
    messages.push_back(std::make_shared<SpawnMessage<BasicSurfaceVessel> >(vesselID, mState));
    return messages;
}

std::shared_ptr<Vessel::VesselUI> BasicSurfaceVessel::constructUI(std::shared_ptr<Vessel> vessel)
{
    auto surfaceVessel = std::dynamic_pointer_cast<BasicSurfaceVessel>(vessel);
    BOOST_ASSERT_MSG(surfaceVessel, "Fatal: Not a BasicSurfaceVessel");
    return std::make_shared<BasicSurfaceVessel::UI>(surfaceVessel);
}

void BasicSurfaceVessel::setHeading(double heading)
{
    mState.setHeading(heading);
}

void BasicSurfaceVessel::setVelocity(double velocity)
{
    mState.setVelocity(velocity);
}

BasicSurfaceVessel::UI::UI(std::weak_ptr<BasicSurfaceVessel> vessel) :
    mHeading(NULL), mVelocity(NULL), mLocation(NULL), mVessel(vessel)
{
}

BasicSurfaceVessel::UI::~UI()
{
    mHeading = NULL;
    mVelocity = NULL;
    mLocation = NULL;
}

std::shared_ptr<sfg::Widget> BasicSurfaceVessel::UI::setupUI()
{
    auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
    auto controlsBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);

    auto labels = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
    auto spinBoxes = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);

    mHeading = sfg::SpinButton::Create(0.f, 359.f, 1.f);
    mVelocity = sfg::SpinButton::Create(0.f, 20.f, 1.f);

    labels->Pack(sfg::Label::Create("Heading (deg):"));
    labels->Pack(sfg::Label::Create("Velocity (kts):"));

    spinBoxes->Pack(mHeading);
    spinBoxes->Pack(mVelocity);

    labels->SetSpacing(20.f);
    spinBoxes->SetSpacing(20.f);

    controlsBox->Pack(labels);
    controlsBox->Pack(spinBoxes);

    box->Pack(controlsBox, false, false);
    box->SetSpacing(20.f);

    mLocation = sfg::Label::Create("Location here");

    box->Pack(mLocation);

    return box;
}

void BasicSurfaceVessel::UI::updateUI(double dt)
{
    auto vessel = mVessel.lock();
    if (vessel)
    {
        auto vesselPosition = vessel->getState().getLocation();

        double lat = vesselPosition.getLatitude();
        double lng = vesselPosition.getLongitude();

        std::stringstream locationText;
        locationText << "Location: " << std::endl;
        locationText << lat << std::endl << lng;

        mLocation->SetText(locationText.str());

        auto ocean = Ocean::getOcean();
        ocean->lockAccess();

        vessel->setHeading(mHeading->GetValue());
        vessel->setVelocity(mVelocity->GetValue() * 0.514444444); //Convert to m/s.

        ocean->unlockAccess();
    }
}
