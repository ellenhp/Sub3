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

Position::Position(const Position& other, double range, double bearing, double pitch)
{
    const double lat1 = toRadians(other.latitude());
    const double lng1 = toRadians(other.longitude());
    const double alt1 = other.altitude();
    const double radius = usml::types::wposition::earth_radius + other.altitude();

    //Convert to radians.
    double rangeRadians = range / radius;
    double verticalRange = range * sin(pitch);
    double horizontalRange = rangeRadians * cos(pitch);

    double lat2 = asin(sin(lat1) * cos(horizontalRange) + cos(lat1) * sin(horizontalRange) * cos(bearing));
    double lng2 = lng1 + atan2(
        sin(bearing) * sin(horizontalRange) * cos(lat1),
        cos(horizontalRange) - sin(lat1) * sin(lat2) );
    double alt2 = alt1 + verticalRange;

    latitude(toDegrees(lat2));
    longitude(toDegrees(lng2));
    altitude(alt2);

}

double Position::getLatitude() const
{
    return latitude();
}

double Position::getLongitude() const
{
    return longitude();
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

double Position::toRadians(double degrees)
{
    return degrees * M_PI / 180.0;
}

double Position::toDegrees(double radians)
{
    return radians * 180.0 / M_PI;
}
