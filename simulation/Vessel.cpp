#include "Vessel.hpp"

Vessel::Vessel()
{
}

void Vessel::setState(VesselState state)
{
    mState = state;
}

VesselState Vessel::getState()
{
    return mState;
}
