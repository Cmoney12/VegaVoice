//
// Created by corey on 9/5/21.
//

#ifndef VOIP_CLIENT3_AUDIO_H
#define VOIP_CLIENT3_AUDIO_H

struct Packet {
    int size; //total size of the network packet
    char receiver_number[11];
    char deliverer_number[11];
    uint8_t data_[ENCODED_MAX_BYTES];
};


class Audio {
public:
    Audio() {
        PaError paError = Pa_Initialize();
        if (paError != paNoError)
            throw std::runtime_error(std::string("Pa_ReadStream error: ") + Pa_GetErrorText(paError));
        audio_init();
    }

    ~Audio() {
        if (decoder)
            opus_decoder_destroy(decoder);
        if (encoder)
            opus_encoder_destroy(encoder);

        Pa_Terminate();
    }

    void audio_init() {
        readParams.device = Pa_GetDefaultInputDevice();
        readParams.channelCount = 1;                    /* stereo input */
        readParams.sampleFormat = paInt16;
        readParams.suggestedLatency = Pa_GetDeviceInfo( readParams.device )->defaultLowInputLatency;
        readParams.hostApiSpecificStreamInfo = nullptr;
        writeParams.device = Pa_GetDefaultOutputDevice();
        writeParams.channelCount = 1;
        writeParams.sampleFormat = paInt16;
        writeParams.suggestedLatency = Pa_GetDeviceInfo( writeParams.device )->defaultHighOutputLatency;
        writeParams.hostApiSpecificStreamInfo = nullptr;

        int opusErr;
        encoder = opus_encoder_create(SAMPLE_RATE, CHANNELS, OPUS_APPLICATION_VOIP, &opusErr);
        if (opusErr != OPUS_OK)
            throw std::runtime_error(std::string("opus_encoder_create error: ") + opus_strerror(opusErr));

        decoder = opus_decoder_create(SAMPLE_RATE, CHANNELS, &opusErr);
        if (opusErr != OPUS_OK)
            throw std::runtime_error(std::string("opus_decoder_create error: ") + opus_strerror(opusErr));

        //open read stream
        PaError paError = Pa_OpenStream(&read_stream, &readParams, nullptr,
                                        SAMPLE_RATE, PACKET_SAMPLES, paClipOff, nullptr, nullptr);
        if (paError != paNoError)
            throw std::runtime_error(std::string("Pa_ReadStream error: ") + Pa_GetErrorText(paError));

        //open write stream
        paError = Pa_OpenStream(&write_stream, nullptr,&writeParams,
                                SAMPLE_RATE, PACKET_SAMPLES, paClipOff, nullptr,nullptr);

        if (paError != paNoError)
            throw std::runtime_error(std::string("Pa_WriteStream error: ") + Pa_GetErrorText(paError));

        PaError paErr = Pa_StartStream(read_stream);
        if (paErr)
            throw std::runtime_error(std::string("Start read stream failed ") + Pa_GetErrorText(paErr));

        if (Pa_StartStream(write_stream) != paNoError)
            throw std::runtime_error(std::string("Start write stream failed ") + Pa_GetErrorText(paErr));
    }

    Packet read_message() {

        Packet packet{};
        opus_int16 microphone[PACKET_SAMPLES]{};
        PaError paError = Pa_ReadStream(read_stream, microphone, PACKET_SAMPLES);
        if (paError != paNoError) {
            if (paError == paOutputUnderflowed)
                std::cout << "Pa_ReadStream output underflowed" << std::endl;
            else
                throw std::runtime_error(std::string("Pa_ReadStream failed: ") + Pa_GetErrorText(paError));
        }

        opus_int32 encoded = opus_encode(encoder, microphone, PACKET_SAMPLES, packet.data_, sizeof(packet.data_));
        packet.size = encoded;
        return packet;
    }

    void receive_message(Packet& packet, opus_int size) {
        opus_int16 decoded[PACKET_SAMPLES]{};
        opus_int32 decoded_ret = opus_decode(decoder, packet.data_, packet.size, decoded, PACKET_SAMPLES, 0);
        if (decoded_ret == OPUS_INVALID_PACKET)
        {
            std::cout << "Corrupt Packet " << std::endl;
            // Try again by treating the packet as lost
            decoded_ret = opus_decode(decoder, nullptr, 0, decoded, PACKET_SAMPLES, 0);
        }
        write_message(decoded, PACKET_SAMPLES);
    }

    void write_message(void* buffer, ulong samples) {
        PaError PaErr = Pa_WriteStream(write_stream, buffer, PACKET_SAMPLES);
        std::cout << Pa_GetErrorText(PaErr) << std::endl;
        if (PaErr != paNoError)
        {
            if (PaErr == paOutputUnderflowed)
                std::cout << "Pa_WriteStream output underflowed" << std::endl;
            else
                throw std::runtime_error(std::string("Pa_WriteStream failed: ") + Pa_GetErrorText(PaErr));
        }
    }

private:
    PaStreamParameters readParams{};
    PaStreamParameters writeParams{};
    PaStream *read_stream = nullptr;
    PaStream *write_stream = nullptr;
    OpusEncoder *encoder{};
    OpusDecoder *decoder{};
};

#endif //VOIP_CLIENT3_AUDIO_H
