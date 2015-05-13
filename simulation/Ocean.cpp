#include "Ocean.hpp"
#include "Vessel.hpp"

#include <boost/assert.hpp>

Ocean* Ocean::oceanInst = NULL;

Ocean::Ocean()
{
}

Ocean* Ocean::getOcean()
{
    if (!oceanInst)
    {
        oceanInst = new Ocean();
    }
    return oceanInst;
}

std::vector<std::shared_ptr<Message> > Ocean::tick(float dt) const
{
    //TODO: update the world.
}

std::vector<std::shared_ptr<Message> > Ocean::getInitiationMessages() const
{
    //Loop through all the vessels and ask them for the messages required to duplicate them.
    std::vector<std::shared_ptr<Message> > messages;
    for (auto const & kv : vessels)
    {
        //Get spawn messages.
        auto newMessages = kv.second->getSpawnMessages(kv.first);
        //Add them to the master list.
        messages.insert(messages.end(), newMessages.begin(), newMessages.end());
    }
    return messages;
}

void Ocean::setPlayerID(PlayerID id)
{
    player = id;
}

void Ocean::localResetOcean()
{
    vessels.clear();
}

void Ocean::localSpawnVessel(VesselID id, std::shared_ptr<Vessel> vessel)
{
    subDebug << "Ocean: Spawning " << id << std::endl;
    BOOST_ASSERT_MSG(!vessels.count(id), "Fatal: Ocean already contains vessel to be spawned");
    vessels[id] = vessel;
}

void Ocean::localDespawnVessel(VesselID id)
{
    subDebug << "Ocean: Despawning " << id << std::endl;
    BOOST_ASSERT_MSG(vessels.count(id), "Fatal: Ocean doesn't contain vessel to be despawned");
    vessels.erase(id);
}

void Ocean::localUpdateVessel(VesselID id, VesselState state)
{
    subDebug << "Ocean: Updating " << id << std::endl;
    BOOST_ASSERT_MSG(vessels.count(id), "Fatal: Ocean doesn't contain vessel to be updated");
    vessels[id]->setState(state);
}
