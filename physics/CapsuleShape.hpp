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

#include "physics/CartesianPosition.hpp"
#include "simulation/VesselState.hpp"

class CapsuleShape
{
public:
    CapsuleShape(CartesianPosition center1, CartesianPosition center2, double radius);
    CapsuleShape(VesselState state, double length, double beam);

    bool collidesWith(CapsuleShape other) const;

private:
    CartesianPosition mCenter1, mCenter2;
    double mRadius;

    double distanceBetweenCenterlines(CapsuleShape other) const;
    double distanceBetweenCenterlinesParallel(CapsuleShape other) const;

    static double dot(sf::Vector3<double> left, sf::Vector3<double> right);
    static double magnitude(sf::Vector3<double> vec);
    static double clamp(double val, double min, double max);

    static double minValOf(double v1, double v2, double v3, double v4);
};
