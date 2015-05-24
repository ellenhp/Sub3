#include "BasicSubmarine.hpp"

#include <sstream>

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
    auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
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

    box->Pack(controlsBox, false, false);
    box->SetSpacing(20.f);

    mLocation = sfg::Label::Create("Location here");

    box->Pack(mLocation);

    return box;
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
        locationText << "Location: " << std::endl;
        locationText << lat << " deg" << std::endl << lng << " deg" << std::endl;
        locationText << "Depth: " << alt << " meters";

        mLocation->SetText(locationText.str());

        vessel->setHeading(mHeading->GetValue() * M_PI / 180.0); //Convert to radians.
        vessel->setVelocity(mVelocity->GetValue() * 0.514444444); //Convert to m/s.
        vessel->setPitch(mPitch->GetValue() * M_PI / 180.0); //Convert to m/s.
    }
}
