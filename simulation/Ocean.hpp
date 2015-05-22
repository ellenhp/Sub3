#pragma once

#include <map>
#include <memory>
#include <string>

#include "Sub3.hpp"
#include "network/Message.hpp"
#include "simulation/VesselID.hpp"
#include "simulation/PlayerID.hpp"

class Vessel;
template<class VesselClass> class SpawnMessage;

class Ocean
{
public:
    //Ocean is a singleton. I hope I don't regret that.
    static Ocean* getOcean();

    enum Month
    {
        Undefined = 0,
        January = 1,
        February = 2,
        March = 3,
        April = 4,
        May = 5,
        June = 6,
        July = 7,
        August = 8,
        September = 9,
        October = 10,
        November = 11,
        December = 12
    };

    //Get all the messages required to tick or bring a new client up to speed.
    //For SubServer use only.
    std::vector<std::shared_ptr<Message>> tick(float dt) const;
    std::vector<std::shared_ptr<Message>> getInitiationMessages() const;

    //These methods actually do things.
    //For Message use only.
    void localResetOcean();
    void localSpawnVessel(VesselID id, std::shared_ptr<Vessel> vessel);
    void localDespawnVessel(VesselID id);
    void localUpdateVessel(VesselID id, VesselState state);
    void localSetMonth(Month month);

    Month getMonth();
    bool getHasVessel(VesselID id);
    VesselState getState(VesselID id);

    //Get all vessels within d meters.
    std::vector<std::shared_ptr<const Vessel>> getNearestVessels(double d, std::shared_ptr<const Vessel> target = NULL);

    //This lets GameManager give out references to vessels at its discretion.
    friend class GameManager;

private:
    Ocean();
    static Ocean* oceanInst;

    Month mMonth;

    std::map<VesselID, std::shared_ptr<Vessel>> mVessels;

};
