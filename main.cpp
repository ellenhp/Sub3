#include <string>
#include <memory>

#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/TcpSocket.hpp>

#include "network/SubSocket.hpp"
#include "network/Message.hpp"
#include "simulation/Ocean.hpp"
#include "simulation/vessels/DummyVessel.hpp"
#include "server/SubServer.hpp"

int main(int argc, char** argv)
{
    std::string ip = "127.0.0.1";
    uint16_t port = 22969;
    if (argc == 2)
    {
        std::string target = std::string(argv[1]);
        if (target.find(":") == std::string::npos)
        {
            ip = target;
        }
        else
        {
            ip = target.substr(0, target.find(":"));
            int tmp = std::stoi(target.substr(target.find(":") + 1));
            if (tmp >= 0 && tmp <= 0xFFFF)
            {
                port = tmp;
            }
        }
    }

    std::cout<<"IP: "<<ip<<std::endl<<"Port: "<<port<<std::endl;

    auto tcpSocket = std::make_shared<sf::TcpSocket>();
    tcpSocket->connect(sf::IpAddress(ip), port);

    SubSocket subSocket(tcpSocket);

    while (true)
    {
        auto end_time = std::chrono::steady_clock::now() + network_interval(1);

        std::shared_ptr<Message> message = NULL;
        while (subSocket.hasPackets())
        {
            subSocket >> message;
            message->execute();
        }

        std::this_thread::sleep_until(end_time);
    }

    return 0;
}
