#pragma once

#include "usml/types/wposition1.h"

//Encapsulates a USML wposition1.
class Position : public usml::types::wposition1
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
    Position(double rho, double theta, double phi)
    {
        wposition1(rho, theta, phi);
    }

    Position(const Position& other)
    {
        wposition1(other.rho(), other.theta(), other.phi());
    }
};
