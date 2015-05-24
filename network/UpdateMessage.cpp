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

#include "UpdateMessage.hpp"
#include "simulation/Ocean.hpp"

//This is necessary for serialization to work correctly
BOOST_CLASS_EXPORT_GUID(UpdateMessage, "UpdateMessage")

UpdateMessage::UpdateMessage()
{
}

UpdateMessage::UpdateMessage(VesselID vesselID, VesselState newState) :
    mNewState(newState), mVesselID(vesselID)
{
}

void UpdateMessage::execute()
{
    Ocean::getOcean()->localUpdateVessel(mVesselID, mNewState);
}
