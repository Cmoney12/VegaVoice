//
// Created by corey on 8/22/21.
//

#ifndef VEGAVOICE_UDPCALL_H
#define VEGAVOICE_UDPCALL_H

#include <boost/asio.hpp>
#include <thread>
#include <iostream>

#include "Audio.h"
#include "ReceiverUdp.h"
#include "SenderUdp.h"


class UdpCall : public ReceiverUdp, public SenderUdp {
public:

    UdpCall(const std::string& ip_address, const int& port_receiver, const int& port_sender, Audio* audio)
            : SenderUdp(ip_address, port_sender),
              ReceiverUdp(port_receiver, audio) {
        audio_ = audio;
    }

    void start() {
        this->connect();
        call_active_ = true;
        while(call_active_) {
            try {
                Packet packet = audio_->read_message();
                this->write_packet(packet);
            } catch (std::error_code& ec) {
                std::cerr << ec << std::endl;
            }
        }
    }

    void stop() {
        call_active_ = false;
        SenderUdp::stop();
        ReceiverUdp::stop();
    }

private:
    Audio *audio_;
    bool call_active_{};
};

#endif //VEGAVOICE_UDPCALL_H
