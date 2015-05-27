/*
 * Sub^3 is a free software submarine simulator focused on realism.
 * Copyright (C) 2015 Nolan Poe
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of  MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Ocean.hpp"

#include "network/UpdateMessage.hpp"
#include "simulation/Vessel.hpp"
#include "simulation/GameManager.hpp"

#include <boost/assert.hpp>

Ocean* Ocean::oceanInst = NULL;

Ocean::Ocean() :
    mMonth(Month::Undefined)
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

std::vector<std::shared_ptr<Message>> Ocean::tick(float dt) const
{
    std::vector<std::shared_ptr<Message>> messages;
    for (auto& vesselKV : mVessels)
    {
        auto newState = vesselKV.second->getNewState(dt);
        messages.push_back(std::make_shared<UpdateMessage>(vesselKV.first, newState));
    }
    return messages;
}

std::vector<std::shared_ptr<Message>> Ocean::getInitiationMessages() const
{
    //Loop through all the vessels and ask them for the messages required to duplicate them.
    std::vector<std::shared_ptr<Message>> messages;
    for (auto const & kv : mVessels)
    {
        //Get spawn messages.
        auto newMessages = kv.second->getSpawnMessages(kv.first);
        //Add them to the master list.
        messages.insert(messages.end(), newMessages.begin(), newMessages.end());
    }
    return messages;
}

void Ocean::localResetOcean()
{
    mAccessMutex.lock();
    mVessels.clear();
    mAccessMutex.unlock();
}

void Ocean::localSpawnVessel(VesselID id, std::shared_ptr<Vessel> vessel)
{
    subDebug << "Ocean: Spawning " << id << std::endl;
    BOOST_ASSERT_MSG(!mVessels.count(id), "Fatal: Ocean already contains vessel to be spawned");
    mAccessMutex.lock();
    mVessels[id] = vessel;
    mAccessMutex.unlock();
}

void Ocean::localDespawnVessel(VesselID id)
{
    subDebug << "Ocean: Despawning " << id << std::endl;
    BOOST_ASSERT_MSG(mVessels.count(id), "Fatal: Ocean doesn't contain vessel to be despawned");
    mAccessMutex.lock();
    mVessels.erase(id);
    mAccessMutex.unlock();
}

void Ocean::localUpdateVessel(VesselID id, VesselState state)
{
    // subDebug << "Ocean: Updating " << id << std::endl;
    BOOST_ASSERT_MSG(mVessels.count(id), "Fatal: Ocean doesn't contain vessel to be updated");
    mAccessMutex.lock();
    mVessels[id]->setState(state);
    mAccessMutex.unlock();
}

void Ocean::localSetMonth(Ocean::Month month)
{
    mMonth = month;
}

Ocean::Month Ocean::getMonth() const
{
    return mMonth;
}

bool Ocean::getHasVessel(VesselID id) const
{
    return mVessels.count(id) != 0;
}

VesselState Ocean::getState(VesselID id) const
{
    BOOST_ASSERT_MSG(getHasVessel(id), "Fatal: Ocean doesn't contain vessel to get state of");
    return mVessels.at(id)->getState();
}

std::vector<VesselID> Ocean::getNearestVesselIDs(double d, std::shared_ptr<const Vessel> target) const
{
    //NULL means we should use our player's vessel.
    if (target == NULL)
    {
        auto gameManager = GameManager::getCurrent().lock();
        //Make sure everything is initialized.
        BOOST_ASSERT_MSG(gameManager, "Fatal: GameManager.getCurrent() NULL in Ocean.getNearestVessels(d)");
        BOOST_ASSERT_MSG(gameManager->isInitialized(), "Fatal: GameManager not initialized in Ocean.getNearestVessels(d)");
        target = gameManager->getCurrentVessel();
    }

    std::vector<VesselID> nearestVessels;

    auto targetPos = target->getState().getLocation();
    for (auto& vesselKV : mVessels)
    {
        auto otherLocation = vesselKV.second->getState().getLocation();
        if (targetPos.distanceTo(otherLocation) < d)
        {
            nearestVessels.push_back(vesselKV.first);
        }
    }

    return nearestVessels;
}

void Ocean::lockAccess()
{
    mAccessMutex.lock();
}

void Ocean::unlockAccess()
{
    mAccessMutex.unlock();
}
