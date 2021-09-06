//
// Created by corey on 9/5/21.
//

#ifndef VOIP_CLIENT3_RECEIVERUDP_H
#define VOIP_CLIENT3_RECEIVERUDP_H

class ReceiverUdp {
public:

    ReceiverUdp(const int &port_, Audio *audio) {
        socket_.open(boost::asio::ip::udp::v4());
        socket_.bind(boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string("0.0.0.0"), port_));
        audio_ = audio;
    }

    void start() {
        thread = new std::thread([&] { io_context.run(); } );

        read_header();

    }

    void read_header() {
        socket_.async_receive_from(boost::asio::buffer(&packet.size, sizeof(packet.size)), remote_endpoint,
                                   [this](boost::system::error_code ec, std::size_t) {
                                       if (!ec) {
                                           read_body();
                                       }

                                       else {
                                           socket_.close();
                                       }
                                   });
    }

    void read_body() {
        socket_.async_receive_from(boost::asio::buffer(&packet.data_, packet.size), remote_endpoint,
                                   [this](boost::system::error_code ec, std::size_t) {
                                       if (!ec) {
                                           audio_->receive_message(packet, packet.size);
                                       }

                                       else {
                                           socket_.close();
                                       }
                                   });
    }

    void stop() {
        io_context.stop();
        thread->join();
        socket_.close();
    }

    Audio *audio_{};
    Packet packet{};
    boost::asio::io_context io_context{};
    boost::asio::ip::udp::socket socket_{io_context};
    boost::asio::ip::udp::endpoint remote_endpoint{};
    std::thread *thread = nullptr;
};

#endif //VOIP_CLIENT3_RECEIVERUDP_H
