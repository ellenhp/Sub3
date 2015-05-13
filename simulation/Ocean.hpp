#pragma once

#include <map>
#include <memory>
#include <string>

#include "../Sub3.hpp"

#include "../network/Message.hpp"

#include "VesselID.hpp"
#include "PlayerID.hpp"

class Vessel;
template<class VesselClass> class SpawnMessage;

class Ocean
{
public:
    //Ocean is a singleton. I hope I don't regret that.
    static Ocean* getOcean();

    void setPlayerID(PlayerID id);

    //Get all the messages required to tick or bring a new client up to speed.
    //For SubServer use only.
    std::vector<std::shared_ptr<Message> > tick(float dt) const;
    std::vector<std::shared_ptr<Message> > getInitiationMessages() const;

    //These methods actually do things.
    //For Message use only.
    void localResetOcean();
    void localSpawnVessel(VesselID id, std::shared_ptr<Vessel> vessel);
    void localDespawnVessel(VesselID id);
    void localUpdateVessel(VesselID id, VesselState state);

private:
    Ocean();
    static Ocean* oceanInst;

    PlayerID player;

    std::map<VesselID, std::shared_ptr<Vessel> > vessels;
    uint32_t nextVesselHandle = 0;

};
