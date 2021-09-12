//
// Created by corey on 9/4/21.
//

#include <bson.h>

#ifndef VEGAVOICE_SERIALIZATION_H
#define VEGAVOICE_SERIALIZATION_H

struct Protocol {
    int32_t status_code;
    char receivers_number[11];
    char senders_number[11];
    char data[30];
};

class Serialization {
public:

    enum { HEADER_LENGTH = 4 };

    Serialization() = default;

    ~Serialization() {
        delete[] data_;
    }

    uint8_t *create_packet() {
        data_ = new uint8_t[body_length_ + HEADER_LENGTH];
        encode_header();
        std::memcpy(data_ + HEADER_LENGTH, bson, body_length_);
        return data_;
    }

    std::size_t length() {
        return body_length_ + 4;
    }

    std::size_t body_length() {
        return body_length_;
    }

    uint8_t* head() {
        return header;
    }

    uint8_t* body() {
        return data_ + HEADER_LENGTH;
    }

    const uint8_t *create_bson(const Protocol& protocol) {
        bson_init(&document);
        bson_append_int32(&document, "Status_Code", -1, protocol.status_code);
        bson_append_utf8(&document, "Receivers_Number", -1, protocol.receivers_number, -1);
        bson_append_utf8(&document, "Senders_Number", -1, protocol.senders_number, -1);
        if (protocol.data)
            bson_append_utf8(&document, "Data", -1, protocol.data, -1);
        body_length_ = (int)document.len;

        bool steal = true;
        uint32_t size;

        bson = bson_destroy_with_steal(&document, steal, &size);

        return bson;
    }

    void set_size(int size) {
        body_length_ = size;
        data_ = new uint8_t[size + HEADER_LENGTH];
    }

    bool decode_header() {
        std::memcpy(&body_length_, header, sizeof body_length_);
        set_size(body_length_);
        //std::memcpy(body_length_, header, HEADER_LENGTH);
        data_[3] = (body_length_>>24) & 0xFF;
        data_[2] = (body_length_>>16) & 0xFF;
        data_[1] = (body_length_>>8) & 0xFF;
        data_[0] = body_length_ & 0xFF;
        if(body_length_ > MAX_MESSAGE_SIZE) {
            body_length_ = 0;
            return false;
        }
        return true;
    }

    bool encode_header() const {
        if (body_length_ <= MAX_MESSAGE_SIZE && body_length_) {
            data_[3] = (body_length_>>24) & 0xFF;
            data_[2] = (body_length_>>16) & 0xFF;
            data_[1] = (body_length_>>8) & 0xFF;
            data_[0] = body_length_ & 0xFF;
            return true;
        }
        return false;
    }


    Protocol parse_bson() {

        const bson_t *received;
        bson_reader_t *reader;
        bson_iter_t iter;
        Protocol protocol{};

        reader = bson_reader_new_from_data(body(), length());

        received = bson_reader_read(reader, nullptr);

        if (bson_iter_init_find(&iter, received, "Status_Code") && BSON_ITER_HOLDS_INT32(&iter)) {
            protocol.status_code = bson_iter_int32(&iter);
        }

        if (bson_iter_init_find(&iter, received, "Receivers_Number") && BSON_ITER_HOLDS_UTF8(&iter)) {
            const char *receiver = bson_iter_utf8(&iter, nullptr);
            std::memcpy(protocol.receivers_number, receiver, std::strlen(receiver));
        }

        if (bson_iter_init_find(&iter, received, "Senders_Number") && BSON_ITER_HOLDS_UTF8(&iter)) {
            const char *deliverer = bson_iter_utf8(&iter, nullptr);
            std::memcpy(protocol.senders_number, deliverer, std::strlen(deliverer));
        }

        if (bson_iter_init_find(&iter, received, "Data") && BSON_ITER_HOLDS_UTF8(&iter)) {
            const char *receiver = bson_iter_utf8(&iter, nullptr);
            std::memcpy(protocol.data, receiver, std::strlen(receiver));
        }

        bson_reader_destroy(reader);

        return protocol;
    }



    uint8_t *data_{};
private:
    const uint8_t *bson{};
    int body_length_{};
    std::size_t full_size = body_length_ + 4;
    enum { MAX_MESSAGE_SIZE = 99999 };
    bson_t document{};
    uint8_t header[HEADER_LENGTH]{};
};

#endif //VEGAVOICE_SERIALIZATION_H
