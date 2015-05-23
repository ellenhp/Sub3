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
