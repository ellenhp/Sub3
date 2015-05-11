#include "SubSocket.hpp"

#include <SFML/Network/Packet.hpp>

SubSocket::SubSocket(std::shared_ptr<sf::TcpSocket> socket) :
    socket(socket)
{
    //We don't want the game or server to wait for us.
    socket->setBlocking(false);
}

bool SubSocket::hasPackets()
{
    //Grab packets off the socket.
    queuePackets();
    return queue.size() > 0;
}

void SubSocket::queuePackets()
{
    Message* messageRaw = NULL;
    sf::Packet packet;

    while (socket->receive(packet) == sf::Socket::Status::Done)
    {
        //Grab the bytes from the packet.
        std::string str = std::string((const char*)packet.getData(), packet.getDataSize());
        std::stringstream stream(str);

        boost::archive::text_iarchive inArchive(stream);
        inArchive >> messageRaw; //Deserialize the packet.
        //Boost creates the appropriate object and gives us a pointer to it.

        //Process it later.
        queue.push_back(std::shared_ptr<Message>(messageRaw));
    }
}

SubSocket& operator<<(SubSocket& socket, std::shared_ptr<Message> message)
{
    std::stringstream stream;
    boost::archive::text_oarchive outArchive(stream);

    //We want the raw pointer for this.
    Message* messageRaw = message.get();

    //Boost serializes the object we're pointing at, preserving polymorphic behavior,
    outArchive << messageRaw;

    sf::Packet packet;
    packet.append(stream.str().c_str(), stream.str().size());

    subDebug << "Sending packet: " << socket.socket->send(packet) << std::endl;

    return socket;
}

bool operator>>(SubSocket& socket, std::shared_ptr<Message>& message)
{
    if (socket.queue.size() > 0)
    {
        message = socket.queue.front();
        socket.queue.pop_front();
        return true;
    }
    return false; //Give the user some indication that their shared_ptr isn't what they want
}
