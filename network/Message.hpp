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

#include <map>
#include <boost/serialization/map.hpp>
#include <boost/serialization/export.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "simulation/PlayerID.hpp"

class Ocean;

class Message
{
    //Serialization boilerplate.
    friend class Ocean;
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & otherData;
    }

public:
    Message();

    virtual bool shouldServerSendTo(PlayerID player)
    {
        return true;
    }

    //This tells the packet to do whatever it needs to do to accomplish its mission.
    virtual void execute() = 0;

    std::map<std::string, std::string> otherData;
};
