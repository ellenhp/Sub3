#include "DespawnMessage.hpp"
#include "../simulation/Ocean.hpp"

DespawnMessage::DespawnMessage()
{
}

DespawnMessage::DespawnMessage(VesselID vesselID) :
    vesselID(vesselID)
{
}

void DespawnMessage::execute()
{
    Ocean::getOcean()->localDespawnVessel(vesselID);
}
