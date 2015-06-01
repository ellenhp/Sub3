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

#include "CapsuleShape.hpp"

#include <iomanip>

#include "Sub3.hpp"

CapsuleShape::CapsuleShape(CartesianPosition center1, CartesianPosition center2, double radius) :
    mCenter1(center1), mCenter2(center2), mRadius(radius)
{
}

CapsuleShape::CapsuleShape(VesselState state, double length, double beam)
{
    Position pos = state.getLocation();

    //Radius of the cylinder of the capsule.
    double radius = beam / 2;

    //Length/half length of the cylinder part of the capsule.
    double cylinderLength = length - beam;
    double cylinderHalfLength = cylinderLength / 2;

    Position center1 = Position(pos, -cylinderHalfLength, state.getHeading(), state.getPitch());
    Position center2 = Position(pos, cylinderHalfLength, state.getHeading(), state.getPitch());

    mCenter1 = CartesianPosition(center1);
    mCenter2 = CartesianPosition(center2);
    mRadius = radius;
}

double CapsuleShape::dot(sf::Vector3<double> left, sf::Vector3<double> right)
{
    return left.x * right.x + left.y * right.y + left.z * right.z;
}

double CapsuleShape::magnitude(sf::Vector3<double> vec)
{
    return sqrt(dot(vec, vec));
}

double CapsuleShape::clamp(double val, double min, double max)
{
    BOOST_ASSERT_MSG(min <= max, "min > max");
    if (val < min)
    {
        val = min;
    }
    else if (val > max)
    {
        val = max;
    }
    return val;
}

double CapsuleShape::minValOf(double v1, double v2, double v3, double v4)
{
    double minVal = v1;
    minVal = (v2 < minVal) ? v2 : minVal;
    minVal = (v3 < minVal) ? v3 : minVal;
    minVal = (v4 < minVal) ? v4 : minVal;
    return minVal;
}

bool CapsuleShape::collidesWith(CapsuleShape other) const
{
    return distanceBetweenCenterlines(other) <= (mRadius + other.mRadius);
}

double CapsuleShape::distanceBetweenCenterlines(CapsuleShape other) const
{
    //Computes distance between the centerlines of the capsules.
    double smallVal = 0.0001;

    auto vDiff = mCenter1 - other.mCenter1; //Vector representing the difference between each capsule's center1.
    auto vAxis1 = mCenter2 - mCenter1; //Vector representing the first capsule's centerline.
    auto vAxis2 = other.mCenter2 - other.mCenter1; //Vector representing the second capsule's centerline.

    //Buncha dot products.
    double dotDiffAxis1 = dot(vDiff, vAxis1);
    double dotDiffAxis2 = dot(vDiff, vAxis2);
    double dotAxis1Axis2 = dot(vAxis1, vAxis2);

    //Magnitudes of the axis vectors.
    double axis1LengthSq = dot(vAxis1, vAxis1);
    double axis2LengthSq = dot(vAxis2, vAxis2);

    //TODO: small things!!!!!

    //denom will be very close to zero if the capsules centerlines are nearly parallel.
    double denom = (axis1LengthSq * axis2LengthSq) - (dotAxis1Axis2 * dotAxis1Axis2);

    if (denom < smallVal)
    {
        subDebug << "Parallel" << std::endl;
        return distanceBetweenCenterlinesParallel(other);
    }

    double numer = dotDiffAxis2 * dotAxis1Axis2 - dotDiffAxis1 * axis2LengthSq;

    //TODO: comment clarity?
    //Coordinate of an endpoint of the segment between the centerlines with basis vAxis1
    double coord1 = numer / denom;
    //Coordinate of an endpoint of the segment between the centerlines with basis vAxis2
    double coord2 = (dotDiffAxis2 + dotAxis1Axis2 * coord1) / axis2LengthSq;

    subDebug << std::setprecision(4) << "coord1: " << coord1 << " coord2: " << coord2 << std::endl;

    //Clamp these to the centerline of the capsules.
    coord1 = clamp(coord1, 0, 1);
    coord2 = clamp(coord2, 0, 1);

    subDebug << std::setprecision(4) << "coord1: " << coord1 << " coord2: " << coord2 << std::endl;

    //Closest points of approach.
    auto cpa1 = mCenter1 + vAxis1 * coord1;
    auto cpa2 = other.mCenter1 + vAxis2 * coord2;

    auto closestVector = cpa1 - cpa2;

    return magnitude(closestVector);
}

double CapsuleShape::distanceBetweenCenterlinesParallel(CapsuleShape other) const
{
    auto vDiff1 = mCenter1 - other.mCenter1; //Vector representing the difference between each capsule's center1.
    auto vDiff2 = mCenter2 - other.mCenter2; //Vector representing the difference between each capsule's center2.
    auto vAxis1 = mCenter2 - mCenter1; //Vector representing the first capsule's centerline.
    auto vAxis2 = other.mCenter2 - other.mCenter1; //Vector representing the second capsule's centerline.

    //Unitize vAxis1.
    double magnitudeAxis1 = magnitude(vAxis1);
    auto vUnitAxis1 = vAxis1 / magnitudeAxis1;

    //Unitize vAxis2.
    double magnitudeAxis2 = magnitude(vAxis2);
    auto vUnitAxis2 = vAxis2 / magnitudeAxis2;

    //Get the scalar projection of vDiff1 and vDiff2 onto vAxis1.
    //The magnitude of the scalar projection is equal to the length of the vector projection and the sign is the direction.
    double coord1 = dot(vUnitAxis1, vDiff1);
    double coord2 = dot(vUnitAxis1, vDiff2);

    //Unrelated to the above.
    //Get the distance between the lines (NOT segments).
    //We'll use the distance between mCenter1 its CPA on vAxis2.
    auto cpa = other.mCenter1 + vUnitAxis2 * dot(vDiff1, vUnitAxis2);
    double distanceBetweenLines = magnitude(cpa - mCenter1);

    //If the projection of a line onto the other has any overlap.
    if (coord1 > 0 && coord1 < magnitudeAxis1)
    {
        return distanceBetweenLines;
    }
    else if (coord2 > -magnitudeAxis1 && coord2 < 0)
    {
        return distanceBetweenLines;
    }

    //There is no overlap, so find the closest distance between endpoints.
    double dStartStart = magnitude(vDiff1);
    double dEndEnd = magnitude(vDiff2);
    double dStartEnd = magnitude(mCenter1 - other.mCenter2);
    double dEndStart = magnitude(mCenter2 - other.mCenter1);

    return minValOf(dStartStart, dEndEnd, dStartEnd, dEndStart);
}
