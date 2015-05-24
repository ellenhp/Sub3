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

#include "Position.hpp"

#include "Sub3.hpp"

Position::Position(double rho, double theta, double phi) :
    wposition1(rho, theta, phi)
{
}

Position::Position()
{
    latitude(0);
    longitude(0);
    altitude(0);
}

Position::Position(const Position& other, double range, double bearing) :
    wposition1(other, range, bearing)
{
}

double Position::getLatitude() const
{
    return 90.0 - (theta() * 180.0 / M_PI);
}

double Position::getLongitude() const
{
    return (phi() * 180.0 / M_PI);
}

double Position::getAltitude() const
{
    return altitude();
}

void Position::setLatitude(double lat)
{
    subDebug << "set lat to " << lat << std::endl;
    latitude(lat);
}

void Position::setLongitude(double lng)
{
    longitude(lng);
}

void Position::setAltitude(double alt)
{
    altitude(alt);
}

double Position::distanceTo(Position other)
{
    return gc_range(other);
}

double Position::bearingTo(Position other)
{
    double bearing;
    gc_range(other, &bearing);
    return bearing;
}
