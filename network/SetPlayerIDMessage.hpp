#pragma once

#include "../Sub3.hpp"
#include "Message.hpp"

class SetPlayerIDMessage : public Message
{
    //Serialization boilerplate
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<Message>(*this);
        ar & newPlayerID;
    }

public:
    SetPlayerIDMessage();
    SetPlayerIDMessage(uint32_t newPlayerID);

    void execute();

private:
    uint32_t newPlayerID;
};
