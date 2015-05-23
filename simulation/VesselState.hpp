#pragma once

#include "simulation/Position.hpp"

//This keeps track of the most basic states a vessel can have.
class VesselState
{
    //Serialization boilerplate
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & mLocation;
        ar & mVelocity;
        ar & mHeading;
        ar & mPitch;
    }

public:
    VesselState(Position pos, double v, double heading, double pitch);
    VesselState();

    Position getLocation() const;
    double getVelocity() const;
    double getHeading() const;
    double getPitch() const;

    void setLocation(Position location);
    void setVelocity(double velocity);
    void setHeading(double heading);
    void getPitch(double pitch);

private:
    Position mLocation;
    double mVelocity;

    double mHeading;
    double mPitch;
};
