#include "SetPlayerIDMessage.hpp"
#include "../simulation/Ocean.hpp"

//This is necessary for serialization to work correctly.
BOOST_CLASS_EXPORT_GUID(SetPlayerIDMessage, "SetPlayerIDMessage")

SetPlayerIDMessage::SetPlayerIDMessage()
{
    mNewPlayerID = -1;
}

SetPlayerIDMessage::SetPlayerIDMessage(uint32_t newPlayerID) :
    mNewPlayerID(newPlayerID)
{
}

void SetPlayerIDMessage::execute()
{
    //Don't execute the packet if it wasn't constructed properly.
    if (mNewPlayerID != -1)
    {
        Ocean::getOcean()->setPlayerID(PlayerID(mNewPlayerID));
    }
}
