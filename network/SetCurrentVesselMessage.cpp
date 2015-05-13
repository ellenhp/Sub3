#include "SetCurrentVesselMessage.hpp"

//This is necessary for serialization to work correctly.
BOOST_CLASS_EXPORT_GUID(SetCurrentVesselMessage, "SetCurrentVesselMessage")

SetCurrentVesselMessage::SetCurrentVesselMessage()
{
}

SetCurrentVesselMessage::SetCurrentVesselMessage(VesselID vesselID) :
    vesselID(vesselID)
{
}

void SetCurrentVesselMessage::execute()
{
    //TODO: Tell make it known to the UI which vessel to control.
}
