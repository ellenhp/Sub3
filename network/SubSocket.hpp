#pragma once

#include <memory>
#include <deque>
#include <future>

#include <SFML/Network/TcpSocket.hpp>

#include "network/Message.hpp"

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
    friend std::future<bool> operator<<(SubSocket& socket, std::shared_ptr<Message> message);
    friend bool operator>>(SubSocket& socket, std::shared_ptr<Message>& message);

private:
    std::shared_ptr<sf::TcpSocket> mSocket;

    std::deque<std::shared_ptr<Message> > mQueue;

    void queuePackets();

    //Returns true if the packet was successfully sent. False otherwise.
    bool sendPacketAsync(std::shared_ptr<Message> message);

};
