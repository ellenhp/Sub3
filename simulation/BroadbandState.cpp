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

#include "BroadbandState.hpp"

BroadbandState::BroadbandState() :
    mNoiseIntensity(0), mNoiseGranularity(0), mMaxAngularResolution(1)
{
}

BroadbandState::BroadbandState(float noiseIntensity, float noiseGranularity, float maxAngularResolution) :
    mNoiseIntensity(noiseIntensity), mNoiseGranularity(noiseGranularity), mMaxAngularResolution(maxAngularResolution)
{
}

void BroadbandState::setNoiseIntensity(float noiseIntensity)
{
    mNoiseIntensity = noiseIntensity;
}

void BroadbandState::setNoiseGranularity(float noiseGranularity)
{
    mNoiseGranularity = noiseGranularity;
}

void BroadbandState::pushContact(BroadbandState::BroadbandContact contact)
{
    mContacts.push_back(contact);
}

void BroadbandState::clearContacts()
{
    mContacts.clear();
}

std::vector<float> BroadbandState::getRow()
{
    int buckets = (int)(360 / mMaxAngularResolution);
    std::vector<float> row(buckets);

    for (auto& contact : mContacts)
    {
        //Determine the appropriate buckets.
        int minBucket = (int)((contact.bearing - contact.angularResolution) / 360 * buckets);
        int maxBucket = (int)((contact.bearing + contact.angularResolution) / 360 * buckets);

        //We need to go one beyond the end because the check in the do-while loop uses a !=
        maxBucket++;

        //Avoid overflows.
        minBucket = (minBucket + buckets) % buckets;
        maxBucket = (maxBucket + buckets) % buckets;

        //Fill up the appropriate buckets.
        int bucket = minBucket;
        do
        {
            row[bucket] += contact.intensity;
            bucket = (bucket + 1) % buckets;
        } while (bucket != maxBucket);
    }

    for (float& intensity : row)
    {
        if (intensity > 1)
        {
            intensity = 1;
        }
    }

    return row;
}
