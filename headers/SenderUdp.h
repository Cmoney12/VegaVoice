//
// Created by corey on 8/22/21.
//

#ifndef VOIP_CLIENT3_SENDERUDP_H
#define VOIP_CLIENT3_SENDERUDP_H

#include "UdpCall.h"

class SenderUdp : UdpCall {
    void start() {

    }

    void stop() {
        io_context.stop();
        thread->join();
        socket_.close();
    }

    Audio& audio;
    boost::asio::io_context io_context;
    boost::asio::ip::udp::socket socket_{io_context};
    boost::asio::ip::udp::endpoint remote_endpoint;
    std::thread *thread = nullptr;
};

#endif //VOIP_CLIENT3_SENDERUDP_H
