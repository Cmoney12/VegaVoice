//
// Created by corey on 8/22/21.
//

#ifndef VOIP_CLIENT3_SENDERUDP_H
#define VOIP_CLIENT3_SENDERUDP_H

#include "UdpCall.h"

class SenderUdp : UdpCall {
    void start() {

    }

    boost::asio::io_context io_context;
    boost::asio::ip::udp::socket{io_context};
    boost::asio::ip::udp::endpoint remote_endpoint;
    std::thread *q = nullptr;
};

#endif //VOIP_CLIENT3_SENDERUDP_H
