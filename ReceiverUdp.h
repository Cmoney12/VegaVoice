//
// Created by corey on 9/5/21.
//

#ifndef VOIP_CLIENT3_RECEIVERUDP_H
#define VOIP_CLIENT3_RECEIVERUDP_H

#include "UdpCall.h"


class ReceiverUdp : UdpCall {
    void start() {

        socket.open(udp::v4());
        this->socket.bind(udp::endpoint(address::from_string("0.0.0.0"), port_));
        socket.async_receive_from(boost::asio::buffer(this->recv_buffer), remote_endpoint,
                                  boost::bind(&ReceiverUDP::handleReceive, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
        thread = new std::thread([&] { io_service.run(); } );
        std::cout << "Server launched" << std::endl;

    }

    int port_;
    boost::asio::io_context io_context;
    boost::asio::ip::udp::socket{io_context};
    boost::asio::ip::udp::endpoint remote_endpoint;
    std::thread *thread = nullptr;
};


#endif //VOIP_CLIENT3_RECEIVERUDP_H
