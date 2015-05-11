#include "VesselState.hpp"

VesselState::VesselState(Position pos, double v, double heading, double pitch) :
    location(pos), velocity(v), heading(heading), pitch(pitch)
{
}

VesselState::VesselState() :
    location(0, 0, 0)
{
}

Position VesselState::getLocation()
{
    return location;
}

double VesselState::getVelocity()
{
    return velocity;
}

double VesselState::getHeading()
{
    return heading;
}

double VesselState::getPitch()
{
    return pitch;
}
