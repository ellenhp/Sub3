#pragma once

#include <memory>
#include <future>
#include <deque>

#include "network/Message.hpp"

namespace sf
{
    class TcpSocket;
}

class SubSocket
{
public:
    //The socket should not be tampered with after the SubSocket is constructed
    SubSocket(std::shared_ptr<sf::TcpSocket> socket);

    //Check to see if there are packets available to process
    bool hasPackets();

    //Check to see if it's still connnected
    bool isConnected();

    //Send and recieve entire messages
    friend bool operator<<(SubSocket& socket, std::shared_ptr<Message> message);
    friend bool operator>>(SubSocket& socket, std::shared_ptr<Message>& message);

private:
    std::shared_ptr<sf::TcpSocket> mSocket;

    std::deque<std::shared_ptr<Message> > mQueue;

    void queuePackets();

};
