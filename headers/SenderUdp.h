//
// Created by corey on 8/22/21.
//

#ifndef VOIP_CLIENT3_SENDERUDP_H
#define VOIP_CLIENT3_SENDERUDP_H

#include "UdpCall.h"

class SenderUdp : UdpCall {

    void start() override {

        socket_ = new udp::socket(io_context);
        socket->open(udp::v4());
        remote_endpoint = new udp::endpoint(address::from_string(this->_ipAddress), this->_port);
        socket->connect((*this->_remote_endpoint));

    }

    void stop() override {
        io_context.stop();
        thread->join();
        socket_.close();
    }

    boost::asio::io_context io_context;
    boost::asio::ip::udp::endpoint remote_endpoint;
    std::thread *thread = nullptr;
};

#endif //VOIP_CLIENT3_SENDERUDP_H
