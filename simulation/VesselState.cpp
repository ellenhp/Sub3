#include "VesselState.hpp"

VesselState::VesselState(Position pos, double v, double heading, double pitch) :
    mLocation(pos), mVelocity(v), mHeading(heading), mPitch(pitch)
{
}

VesselState::VesselState() :
    mLocation()
{
}

Position VesselState::getLocation() const
{
    return mLocation;
}

double VesselState::getVelocity() const
{
    return mVelocity;
}

double VesselState::getHeading() const
{
    return mHeading;
}

double VesselState::getPitch() const
{
    return mPitch;
}

void VesselState::setLocation(Position location)
{
    mLocation = location;
}

void VesselState::setVelocity(double velocity)
{
    mVelocity = velocity;
}

void VesselState::setHeading(double heading)
{
    mHeading = heading;
}

void VesselState::getPitch(double pitch)
{
    mPitch = pitch;
}
