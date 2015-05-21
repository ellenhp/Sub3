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
