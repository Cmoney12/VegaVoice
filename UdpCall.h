//
// Created by corey on 8/22/21.
//

#ifndef VOIP_CLIENT3_UDPCALL_H
#define VOIP_CLIENT3_UDPCALL_H
class UdpCall : public SenderUdp, public ReceiverUdp {
public:

std::string ip_address;
int port_receiver;
int port_sender;
//TODO ADD AUDIO
};
#endif //VOIP_CLIENT3_UDPCALL_H