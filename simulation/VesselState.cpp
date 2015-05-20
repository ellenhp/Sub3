#include "VesselState.hpp"

VesselState::VesselState(Position pos, double v, double heading, double pitch) :
    mLocation(pos), mVelocity(v), mHeading(heading), mPitch(pitch)
{
}

VesselState::VesselState() :
    mLocation(0, 0, 0)
{
}

Position VesselState::getLocation()
{
    return mLocation;
}

double VesselState::getVelocity()
{
    return mVelocity;
}

double VesselState::getHeading()
{
    return mHeading;
}

double VesselState::getPitch()
{
    return mPitch;
}
