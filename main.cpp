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

#include <string>
#include <memory>

#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "graphics/SubWindow.hpp"
#include "network/SubSocket.hpp"
#include "network/Message.hpp"
#include "simulation/Ocean.hpp"
#include "simulation/vessels/DummyVessel.hpp"
#include "server/SubServer.hpp"

int main(int argc, char** argv)
{
    std::cout << "Sub^3 version " << subVersionMajor << "." << subVersionMinor << std::endl;
    if (subCommitHash.size() > 0)
    {
        std::cout << "Sub^3 commit hash: " << subCommitHash << std::endl;
    }

    SubWindow subWindow(sf::VideoMode(800, 600));
    subWindow.run();

    return 0;
}
