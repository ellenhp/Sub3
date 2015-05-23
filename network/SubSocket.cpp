#include "SubSocket.hpp"

#include <SFML/Network/Packet.hpp>

SubSocket::SubSocket(std::shared_ptr<sf::TcpSocket> socket) :
    mSocket(socket)
{
    //We don't want the game or server to wait for us.
    mSocket->setBlocking(false);
}

bool SubSocket::hasPackets()
{
    //Grab packets off the socket.
    queuePackets();
    return mQueue.size() > 0;
}

void SubSocket::queuePackets()
{
    Message* messageRaw = NULL;
    sf::Packet packet;

    while (mSocket->receive(packet) == sf::Socket::Status::Done)
    {
        //Grab the bytes from the packet.
        std::string str = std::string((const char*)packet.getData(), packet.getDataSize());
        std::stringstream stream(str);

        boost::archive::text_iarchive inArchive(stream);
        inArchive >> messageRaw; //Deserialize the packet.
        //Boost creates the appropriate object and gives us a pointer to it.

        //Process it later.
        mQueue.push_back(std::shared_ptr<Message>(messageRaw));
    }
}

bool operator<<(SubSocket& socket, std::shared_ptr<Message> message)
{
    std::stringstream stream;
    boost::archive::text_oarchive outArchive(stream);

    //We want the raw pointer for this.
    Message* messageRaw = message.get();

    //Boost serializes the object we're pointing at, preserving polymorphic behavior.
    outArchive << messageRaw;

    sf::Packet packet;
    packet.append(stream.str().c_str(), stream.str().size());

    return socket.mSocket->send(packet) == sf::TcpSocket::Status::Done;
}

bool operator>>(SubSocket& socket, std::shared_ptr<Message>& message)
{
    if (socket.mQueue.size() > 0)
    {
        message = socket.mQueue.front();
        socket.mQueue.pop_front();
        return true;
    }
    return false; //Give the user some indication that their shared_ptr isn't what they want.
}
