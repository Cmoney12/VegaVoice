//
// Created by corey on 9/5/21.
//

#ifndef VEGAVOICE_RECEIVERUDP_H
#define VEGAVOICE_RECEIVERUDP_H

#include "Audio.h"

class ReceiverUdp {
public:

    ReceiverUdp(const int &port_, Audio* audio) {
        audio_ = audio;
        socket_.open(boost::asio::ip::udp::v4());
        read_packet();
        socket_.bind(boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string("0.0.0.0"), port_));
        thread_ = new std::thread([&] { io_service.run(); });
    }

    void read_packet() {
        socket_.async_receive_from(boost::asio::buffer(&packet_, sizeof(packet_)), remote_endpoint,
                                   [this](boost::system::error_code ec, std::size_t) {
                                       if (!ec) {
                                           audio_->receive_message(packet_, packet_.size);
                                           read_packet();
                                       }
                                       else {
                                           socket_.close();
                                       }

                                   });
    }

    /**void read_body() {
        socket_.async_receive_from(boost::asio::buffer(&packet_.data_, packet_.size), remote_endpoint,
                                   [this](boost::system::error_code ec, std::size_t) {
            if (!ec) {
                audio_->receive_message(packet_, packet_.size);
                read_packet();
            }

            else {
                std::cout << ec << std::endl;
                socket_.close();
            }
        });
    }**/

    void stop() {
        io_service.stop();
        thread_->join();
        socket_.close();
    }

private:
    Packet packet_{};
    Audio* audio_ = nullptr;
    std::size_t header_size_ = sizeof(packet_.size);
    boost::asio::io_service io_service;
    boost::asio::ip::udp::socket socket_{io_service};
    boost::asio::ip::udp::endpoint remote_endpoint;
    std::thread *thread_ = nullptr;
};


#endif //VOIP_CLIENT3_RECEIVERUDP_H
