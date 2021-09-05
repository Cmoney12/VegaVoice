//
// Created by corey on 8/22/21.
//

#ifndef VOIP_CLIENT3_UDPCALL_H
#define VOIP_CLIENT3_UDPCALL_H

#include <boost/asio.hpp>

class UdpCall {
public:
    virtual void start() = 0;

    virtual void stop() = 0;

std::string ip_address;
int port_receiver;
int port_sender;
//TODO ADD AUDIO
};
#endif //VOIP_CLIENT3_UDPCALL_H
