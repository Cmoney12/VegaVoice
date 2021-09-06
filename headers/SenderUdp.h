//
// Created by corey on 8/22/21.
//

#ifndef VOIP_CLIENT3_SENDERUDP_H
#define VOIP_CLIENT3_SENDERUDP_H

#include "UdpCall.h"

class SenderUdp {
public:

    SenderUdp(const std::string& ip_address, const int& port, Audio* audio) {
        audio_ = audio;
        socket_ = new boost::asio::ip::udp::socket(io_service);
        socket_->open(boost::asio::ip::udp::v4());
        remote_endpoint = boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string(ip_address), port);
    }

    void start() {
        connect();
        //thread = new std::thread([&] { start_send(); } );
        start_send();
        thread = new std::thread([&] { io_service.run(); });
    }

    void connect() {
        socket_->connect(remote_endpoint);
    }

    void start_send() {
        Packet packet = audio_->read_message();
        async_write_packet(packet);
    }

    void async_write_packet(const Packet& packet) {
        //boost::system::error_code ec;
        //socket_->send(boost::asio::buffer(&packet, packet.size), 0, ec);
        //start_send();
        socket_->async_send(boost::asio::buffer(&packet, packet.size),
                            [this](boost::system::error_code ec, std::size_t) {
                                if (!ec) {
                                    start_send();
                                }

                                else {
                                    socket_->close();
                                }

                            });

    }


    void stop() {
        io_service.stop();
        thread->join();
        socket_->close();
    }

private:
    Audio *audio_ = nullptr;
    boost::asio::ip::udp::socket *socket_ = nullptr;
    boost::asio::io_service io_service;
    boost::asio::ip::udp::endpoint remote_endpoint;
    std::thread *thread = nullptr;
};


#endif //VOIP_CLIENT3_SENDERUDP_H
