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

#pragma once

#include <vector>

class BroadbandState
{
public:
    struct BroadbandContact
    {
        BroadbandContact(float bearing, float intensity, float angularResolution) :
            bearing(bearing), intensity(intensity), angularResolution(angularResolution)
        {}

        float bearing;
        float intensity;
        float angularResolution;
    };

    BroadbandState();

    //noise and noiseGranularity from 0 to 1.
    BroadbandState(float noiseIntensity, float noiseGranularity, float maxAngularResolution);

    void setNoiseIntensity(float noiseIntensity);
    void setNoiseGranularity(float noiseGranularity);

    void pushContact(BroadbandContact contact);
    void clearContacts();

    std::vector<float> getRow();

private:
    std::vector<BroadbandContact> mContacts;

    float mNoiseIntensity;
    float mNoiseGranularity;
    float mMaxAngularResolution;

};
