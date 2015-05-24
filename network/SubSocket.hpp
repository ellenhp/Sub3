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
