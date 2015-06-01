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

#include "PhysicsEngine.hpp"

#include "physics/CartesianPosition.hpp"
#include "simulation/Ocean.hpp"
#include "simulation/Vessel.hpp"

std::vector<PhysicsEngine::Collision> PhysicsEngine::tick(float dt)
{
    Ocean* ocean = Ocean::getOcean();
    auto newStates = ocean->getAllVesselStates();

    //Brute-force broadphase
    std::vector<std::pair<VesselID, VesselID>> nearphaseCandidates;
    for (auto& vessel1KV : newStates)
    {
        for (auto& vessel2KV : newStates)
        {
            //The double-for causes every pair of distinct ID's to come up twice (order switched in the second).
            //Don't bother with one of these, and also don't bother with pairs of equal ID's.
            if (vessel1KV.first > vessel2KV.first || vessel1KV.first == vessel2KV.first)
            {
                continue;
            }

            //They can get no closer to each other in this timestep than maxRelativeDistance.
            double maxRelativeSpeed = abs(vessel1KV.second.getVelocity()) + abs(vessel2KV.second.getVelocity());
            double maxRelativeDistance = dt * maxRelativeSpeed;

            //Last positions for each vessel.
            Position vessel1Pos = vessel1KV.second.getLocation();
            Position vessel2Pos = vessel2KV.second.getLocation();

            //This is, intuitively, how far apart they were at the beginning of the timestep.
            double prevDistance = vessel1Pos.distanceTo(vessel2Pos);

            //If there's any possibility they could collide during the timestep.
            if (prevDistance - maxRelativeDistance <= 2 * mMaxVesselRadius)
            {
                //Check them in the nearphase.
                auto id1 = vessel1KV.first;
                auto id2 = vessel2KV.first;
                auto collisionPair = std::make_pair(id1, id2);
                nearphaseCandidates.push_back(collisionPair);
            }
        }
    }

    std::vector<PhysicsEngine::Collision> collisions;

    //Now do the nearphase collision analysis.
    for (auto& candidate : nearphaseCandidates)
    {
        auto capsule1 = ocean->getVessel(candidate.first)->getBoundingCapsule();
        auto capsule2 = ocean->getVessel(candidate.second)->getBoundingCapsule();

        if (capsule1.collidesWith(capsule2))
        {
            collisions.push_back(PhysicsEngine::Collision(candidate.first, candidate.second));
        }
    }

    return collisions;
}
