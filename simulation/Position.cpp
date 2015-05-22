#include "Position.hpp"

Position::Position(double rho, double theta, double phi)
{
    wposition1(rho, theta, phi);
}

Position::Position(const Position& other)
{
    wposition1(other.rho(), other.theta(), other.phi());
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
