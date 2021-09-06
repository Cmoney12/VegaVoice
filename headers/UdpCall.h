//
// Created by corey on 8/22/21.
//

#ifndef VOIP_CLIENT3_UDPCALL_H
#define VOIP_CLIENT3_UDPCALL_H

#include <boost/asio.hpp>

#include "Audio.h"

class UdpCall {
public:
    UdpCall() {
        audio.audio_init();
    }
    virtual void start() = 0;
    virtual void stop() = 0;

    Audio audio;
    std::string ip_address;
    int port_receiver{};
    int port_sender{};
};
#endif //VOIP_CLIENT3_UDPCALL_H
