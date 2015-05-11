#pragma once

#include "Position.hpp"

//This keeps track of the most basic states a vessel can have.
class VesselState
{
    //Serialization boilerplate
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & location;
        ar & velocity;
        ar & heading;
        ar & pitch;
    }

public:
    VesselState(Position pos, double v, double heading, double pitch);
    VesselState();

    Position getLocation();
    double getVelocity();
    double getHeading();
    double getPitch();

private:
    Position location;
    double velocity;

    double heading;
    double pitch;
};
