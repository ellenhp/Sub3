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

#include "usml/types/wposition1.h"

//Encapsulates a USML wposition1.
class Position : protected usml::types::wposition1
{
    //Serialization boilerplate
    friend class boost::serialization::access;
    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        double r, t, p;
        r = rho();
        t = theta();
        p = phi();

        ar & r;
        ar & t;
        ar & p;
    }
    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        double r, t, p;
        ar & r;
        ar & t;
        ar & p;

        rho(r);
        theta(t);
        phi(p);
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()

public:
    Position();
    Position(double rho, double theta, double phi);
    Position(const Position& other, double range, double bearing);

    double getLatitude() const;
    double getLongitude() const;
    double getAltitude() const;

    void setLatitude(double lat);
    void setLongitude(double lng);
    void setAltitude(double alt);

    //Distance to another Position object in meters.
    double distanceTo(Position other);

    //Bearing to another Position object in radians
    double bearingTo(Position other);
};
