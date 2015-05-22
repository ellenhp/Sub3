#include "SetPlayerIDMessage.hpp"
#include "simulation/GameManager.hpp"

#include <boost/assert.hpp>

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
        auto currentGame = GameManager::getCurrent().lock();
        BOOST_ASSERT_MSG((bool)currentGame, "Fatal: SetPlayerIDMessage.execute() called without a GameManager");
        currentGame->setPlayer(mNewPlayerID);
    }
}
