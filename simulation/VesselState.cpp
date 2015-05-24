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

#include "VesselState.hpp"

VesselState::VesselState(Position pos, double v, double heading, double pitch) :
    mLocation(pos), mVelocity(v), mHeading(heading), mPitch(pitch)
{
}

VesselState::VesselState() :
    mLocation()
{
}

Position VesselState::getLocation() const
{
    return mLocation;
}

double VesselState::getVelocity() const
{
    return mVelocity;
}

double VesselState::getHeading() const
{
    return mHeading;
}

double VesselState::getPitch() const
{
    return mPitch;
}

void VesselState::setLocation(Position location)
{
    mLocation = location;
}

void VesselState::setVelocity(double velocity)
{
    mVelocity = velocity;
}

void VesselState::setHeading(double heading)
{
    mHeading = heading;
}

void VesselState::setPitch(double pitch)
{
    mPitch = pitch;
}
