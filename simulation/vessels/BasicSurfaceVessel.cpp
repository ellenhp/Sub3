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

std::shared_ptr<Vessel::VesselUI> BasicSurfaceVessel::constructUI()
{
    return std::make_shared<BasicSurfaceVessel::UI>(this);
}

void BasicSurfaceVessel::setHeading(double heading)
{
    mState.setHeading(heading);
}

void BasicSurfaceVessel::setVelocity(double velocity)
{
    mState.setVelocity(velocity);
}

BasicSurfaceVessel::UI::UI(BasicSurfaceVessel* vessel) :
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

    auto headingBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
    auto velocityBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);

    mHeading = sfg::SpinButton::Create(0.f, 359.f, 1.f);
    mVelocity = sfg::SpinButton::Create(0.f, 20.f, 1.f);

    headingBox->Pack(sfg::Label::Create("Heading (deg):"));
    headingBox->Pack(mHeading);
    headingBox->SetSpacing(20.f);

    velocityBox->Pack(sfg::Label::Create("Velocity (kts):"));
    velocityBox->Pack(mVelocity);
    velocityBox->SetSpacing(20.f);

    box->Pack(headingBox, false, false);
    box->Pack(velocityBox, false, false);
    box->SetSpacing(20.f);

    mLocation = sfg::Label::Create("Location here");

    box->Pack(mLocation);

    return box;
}

void BasicSurfaceVessel::UI::updateUI(double dt)
{
    auto vesselPosition = mVessel->getState().getLocation();

    double lat = vesselPosition.getLatitude();
    double lng = vesselPosition.getLongitude();

    std::stringstream locationText;
    locationText << "Location: " << std::endl;
    locationText << lat << std::endl << lng;

    mLocation->SetText(locationText.str());

    mVessel->setHeading(mHeading->GetValue() * M_PI / 180.0); //Convert to radians.
    mVessel->setVelocity(mVelocity->GetValue() * 0.514444444); //Convert to m/s.
}
