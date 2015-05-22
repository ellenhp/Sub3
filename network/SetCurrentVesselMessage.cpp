#include "SetCurrentVesselMessage.hpp"

#include "simulation/GameManager.hpp"

#include <boost/assert.hpp>

//This is necessary for serialization to work correctly.
BOOST_CLASS_EXPORT_GUID(SetCurrentVesselMessage, "SetCurrentVesselMessage")

SetCurrentVesselMessage::SetCurrentVesselMessage()
{
}

SetCurrentVesselMessage::SetCurrentVesselMessage(VesselID vesselID) :
    mVesselID(vesselID)
{
}

void SetCurrentVesselMessage::execute()
{
    //Set the user's current vessel.
    auto currentGame = GameManager::getCurrent().lock();
    BOOST_ASSERT_MSG((bool)currentGame, "Fatal: SetCurrentVesselMessage.execute() called without GameManager");
    currentGame->setCurrentVessel(mVesselID);
}
