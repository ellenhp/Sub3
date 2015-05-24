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

#include "simulation/Position.hpp"

//This keeps track of the most basic states a vessel can have.
class VesselState
{
    //Serialization boilerplate
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & mLocation;
        ar & mVelocity;
        ar & mHeading;
        ar & mPitch;
    }

public:
    VesselState(Position pos, double v, double heading, double pitch);
    VesselState();

    Position getLocation() const;
    double getVelocity() const;
    double getHeading() const;
    double getPitch() const;

    void setLocation(Position location);
    void setVelocity(double velocity);
    void setHeading(double heading);
    void getPitch(double pitch);

private:
    Position mLocation;
    double mVelocity;

    double mHeading;
    double mPitch;
};
