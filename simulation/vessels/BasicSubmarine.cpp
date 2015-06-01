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

#include "BasicSubmarine.hpp"

#include "simulation/BroadbandState.hpp"
#include "simulation/USMLManager.hpp"

#include <sstream>
#include <iomanip>

//IMPORTANT: This lets Boost serialize our spawn message.
REGISTER_PACKETS(BasicSubmarine, "BasicSubmarine")

BasicSubmarine::BasicSubmarine()
{
}

VesselState BasicSubmarine::getNewState(float dt)
{
    //TODO: This doesn't take into account pitch. It'll work for a surface vessel though.
    double range = dt * mState.getVelocity();
    double bearing = mState.getHeading();
    double pitch = mState.getPitch();
    Position newPos(mState.getLocation(), range, bearing, pitch);
    return VesselState(newPos, mState.getVelocity(), mState.getHeading(), mState.getPitch());
}

std::vector<std::shared_ptr<Message>> BasicSubmarine::getSpawnMessages(VesselID vesselID)
{
    //Just return the one spawn message. Nothing special here.
    std::vector<std::shared_ptr<Message> > messages;
    messages.push_back(std::make_shared<SpawnMessage<BasicSubmarine> >(vesselID, mState));
    return messages;
}

std::shared_ptr<Vessel::VesselUI> BasicSubmarine::constructUI(std::shared_ptr<Vessel> vessel)
{
    auto submarine = std::dynamic_pointer_cast<BasicSubmarine>(vessel);
    BOOST_ASSERT_MSG(submarine, "Fatal: Not a BasicSubmarine");
    return std::make_shared<BasicSubmarine::UI>(std::weak_ptr<BasicSubmarine>(submarine));
}

void BasicSubmarine::setHeading(double heading)
{
    mState.setHeading(heading);
}

void BasicSubmarine::setVelocity(double velocity)
{
    mState.setVelocity(velocity);
}

void BasicSubmarine::setPitch(double pitch)
{
    mState.setPitch(pitch);
}

CapsuleShape BasicSubmarine::getBoundingCapsule() const
{
    return CapsuleShape(mState, mLength, mBeam);
}

BasicSubmarine::UI::UI(std::weak_ptr<BasicSubmarine> vessel) :
    mHeading(NULL), mVelocity(NULL), mLocation(NULL), mVessel(vessel)
{
}

BasicSubmarine::UI::~UI()
{
    mHeading = NULL;
    mVelocity = NULL;
    mLocation = NULL;
}

std::shared_ptr<sfg::Widget> BasicSubmarine::UI::setupUI()
{
    auto navigationBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);

    auto controlsBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);

    auto labels = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
    auto spinBoxes = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);

    mHeading = sfg::SpinButton::Create(0.f, 359.f, 1.f);
    mVelocity = sfg::SpinButton::Create(0.f, 20.f, 1.f);
    mPitch = sfg::SpinButton::Create(-20, 20.f, 1.f);

    labels->Pack(sfg::Label::Create("Heading (deg):"));
    labels->Pack(sfg::Label::Create("Velocity (kts):"));
    labels->Pack(sfg::Label::Create("Pitch (deg):"));

    spinBoxes->Pack(mHeading);
    spinBoxes->Pack(mVelocity);
    spinBoxes->Pack(mPitch);

    labels->SetSpacing(20.f);
    spinBoxes->SetSpacing(20.f);

    controlsBox->Pack(labels);
    controlsBox->Pack(spinBoxes);

    navigationBox->Pack(controlsBox, false, false);
    navigationBox->SetSpacing(20.f);

    mLocation = sfg::Label::Create("Location here");

    auto locationAndSonar = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
    locationAndSonar->Pack(mLocation, false, false);

    mWaterfall = WaterfallDisplay::Create();
    locationAndSonar->Pack(mWaterfall);

    navigationBox->Pack(locationAndSonar);

    mWaterfall->RequestResize();

    return navigationBox;
}

void BasicSubmarine::UI::updateUI(double dt)
{
    auto vessel = mVessel.lock();
    if (vessel)
    {
        auto vesselPosition = vessel->getState().getLocation();

        double lat = vesselPosition.getLatitude();
        double lng = vesselPosition.getLongitude();
        double alt = vesselPosition.getAltitude();

        std::stringstream locationText;
        locationText << "Position and depth: " << std::endl;
        locationText << std::setprecision(6) << std::fixed << lat << " deg" << std::endl << std::setprecision(6) << std::fixed << lng << " deg" << std::endl;
        locationText << "Depth: " << std::setprecision(2) << alt << " m";

        mLocation->SetText(locationText.str());

        auto ocean = Ocean::getOcean();
        ocean->lockAccess();

        vessel->setHeading(mHeading->GetValue());
        vessel->setPitch(mPitch->GetValue());
        vessel->setVelocity(mVelocity->GetValue() * 0.514444444); //Convert to m/s.

        ocean->unlockAccess();

        BroadbandState state(0.3f, 0.3f, 5.0f);

        auto eigenrayMap = USMLManager::getInstance()->getEigenrayMap();

        //Iterate through all contacts we can hear.
        for (auto& contactKV : eigenrayMap)
        {
            //If the azimuth angle is NaN, that means that there were no eigenrays for this contact.
            if (!isnan(contactKV.second.source_az))
            {
                //Average the intensities across all frequencies.
                //TODO: don't weight them all the same, and figure out what the numbers USML gives us even mean.
                double intensitySum = 0;
                for (auto intensity : contactKV.second.intensity)
                {
                    if (intensity > 299.9999f)
                        continue; //TODO: What causes this?
                    intensitySum += intensity;
                }
                double intensityAverage = intensitySum / contactKV.second.intensity.size();

                float adjustedIntensity = (float)(intensityAverage / 300);

                state.pushContact(BroadbandState::BroadbandContact((float)contactKV.second.source_az, adjustedIntensity, 5.0f));
            }
        }

        mWaterfall->SetState(state);
    }
    else
    {
        subDebug << "fixme2" << std::endl;
    }
}
