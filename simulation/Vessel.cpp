#include "Vessel.hpp"

Vessel::Vessel()
{
}

void Vessel::setState(VesselState state)
{
    this->state = state;
}

VesselState Vessel::getState()
{
    return state;
}
