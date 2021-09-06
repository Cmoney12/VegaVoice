//
// Created by corey on 8/22/21.
//

#ifndef VOIP_CLIENT3_UDPCALL_H
#define VOIP_CLIENT3_UDPCALL_H

#include <boost/asio.hpp>

#include "Audio.h"
#include "ReceiverUdp.h"
#include "SenderUdp.h"

class UdpCall : public ReceiverUdp, SenderUdp {
public:

    UdpCall(const std::string& ip_address, const int& port_receiver, const int& port_sender)
            : SenderUdp(ip_address, port_receiver, audio),
              ReceiverUdp(port_sender, audio) { }

    ~UdpCall() {
        delete audio;
    }

    using ReceiverUdp::start;
    using SenderUdp::start;
    void start() { audio->audio_init(); }

    using ReceiverUdp::stop;
    using SenderUdp::stop;
    void stop() {};

    Audio *audio = new Audio;
};

#endif //VOIP_CLIENT3_UDPCALL_H
