//
// Created by corey on 8/22/21.
//

#ifndef VEGAVOICE_SENDERUDP_H
#define VEGAVOICE_SENDERUDP_H

#include "UdpCall.h"

class SenderUdp {
public:

    SenderUdp(const std::string& ip_address, const int& port) {
        socket_ = new boost::asio::ip::udp::socket(io_context);
        socket_->open(boost::asio::ip::udp::v4());
        remote_endpoint = new boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string(ip_address), port);
    }

    void connect() {
        socket_->connect(*remote_endpoint);
    }

    void write_packet(const Packet& packet) {
        boost::system::error_code ec;
        socket_->send(boost::asio::buffer(&packet, packet.size + header_size), 0, ec);
        if (ec)
            std::cerr << ec << std::endl;
    }


    void stop() {
        socket_->close();
    }

private:
    enum{header_size = 4};
    boost::asio::ip::udp::socket *socket_ = nullptr;
    boost::asio::io_context io_context;
    boost::asio::ip::udp::endpoint *remote_endpoint;
};


#endif //VEGAVOICE_SENDERUDP_H
