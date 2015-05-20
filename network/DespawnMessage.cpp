#include "DespawnMessage.hpp"
#include "../simulation/Ocean.hpp"

DespawnMessage::DespawnMessage()
{
}

DespawnMessage::DespawnMessage(VesselID vesselID) :
    mVesselID(vesselID)
{
}

void DespawnMessage::execute()
{
    Ocean::getOcean()->localDespawnVessel(mVesselID);
}
