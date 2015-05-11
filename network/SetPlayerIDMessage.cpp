#include "SetPlayerIDMessage.hpp"
#include "../simulation/Ocean.hpp"

//This is necessary for serialization to work correctly.
BOOST_CLASS_EXPORT_GUID(SetPlayerIDMessage, "SetPlayerIDMessage")

SetPlayerIDMessage::SetPlayerIDMessage()
{
    newPlayerID = -1;
}

SetPlayerIDMessage::SetPlayerIDMessage(uint32_t newPlayerID) :
    newPlayerID(newPlayerID)
{
}

void SetPlayerIDMessage::execute()
{
    //Don't execute the packet if it wasn't constructed properly.
    if (newPlayerID != -1)
    {
        Ocean::getOcean()->setPlayerID(PlayerID(newPlayerID));
    }
}
