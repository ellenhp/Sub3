#include "UpdateMessage.hpp"
#include "../simulation/Ocean.hpp"

//This is necessary for serialization to work correctly
BOOST_CLASS_EXPORT_GUID(UpdateMessage, "UpdateMessage")

UpdateMessage::UpdateMessage()
{
}

UpdateMessage::UpdateMessage(VesselID vesselID, VesselState newState) :
    mNewState(newState), mVesselID(vesselID)
{
}

void UpdateMessage::execute()
{
    Ocean::getOcean()->localUpdateVessel(mVesselID, mNewState);
}
