#include <iostream>
#include <string>

#include "Sub3.hpp"
#include "server/SubServer.hpp"

int main(int argc, char** argv)
{
    std::cout << "Sub^3 version " << subVersionMajor << "." << subVersionMinor << std::endl;
    if (subCommitHash.size() > 0)
    {
        std::cout << "Sub^3 commit hash: " << subCommitHash << std::endl;
    }

    uint16_t port = 22969;

    //Sanitize port number.
    if (argc == 2)
    {
        int tmp = std::stoi(std::string(argv[1]));
        if (tmp >= 0 && tmp <= 0xFFFF)
        {
            port = tmp;
        }
    }

    SubServer server(port);
    server.start();

    std::cout<<"Port number: "<<port<<std::endl;

    server.wait();

    return 0;
}
