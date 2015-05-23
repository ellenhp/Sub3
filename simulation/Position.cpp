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
