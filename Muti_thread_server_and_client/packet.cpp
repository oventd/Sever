#include "Header.h"

Packet::Packet()
    : length(0), header(NULL), data(nullptr), endMarker(0xffff), x(0.0f), y(0.0f), z(0.0f)
{
}

//Packet::Packet(int len, char hdr, char* dat, float x, float y, float z):length(len),header(hdr),data(dat),x(x),y(y),z(z),endMarker(0xffff)
//{}

Packet::~Packet() {

}

void Packet::setHeader(char hdr) {
    header = hdr;
}

void Packet::getClength(char* len) {
    memcpy(len, &length, sizeof(int));
}

void Packet::importXYZ(float f1, float f2, float f3) {
    int dataSize = 3 * sizeof(float);
    if (data != nullptr) {
        free(data);
    }
    data = (char*)malloc(dataSize);
    if (data == nullptr) {
        std::cerr << "Memory allocation failed" << std::endl;
        exit(1);
    }
    memcpy(data, &f1, sizeof(float));
    memcpy(data + sizeof(float), &f2, sizeof(float));
    memcpy(data + 2 * sizeof(float), &f3, sizeof(float));

    x = f1;
    y = f2;
    z = f3;

    length = sizeof(length) + sizeof(header) + dataSize + sizeof(endMarker);
}

void Packet::importChat(const char* chat) {
    if (data != nullptr) {
        free(data);
    }
    int chatLength = strlen(chat) + 1;
    data = (char*)malloc(chatLength);
    if (data == nullptr) {
        std::cerr << "Memory allocation failed" << std::endl;
        exit(1);
    }
    strcpy_s(data, chatLength, chat);
    length = sizeof(length) + sizeof(header) + chatLength + sizeof(endMarker);
}

void Packet::dataToXYZ() {
    if (data != nullptr) {
        memcpy(&x, data, sizeof(float));
        memcpy(&y, data + sizeof(float), sizeof(float));
        memcpy(&z, data + 2 * sizeof(float), sizeof(float));
    }
}

void Packet::packing(char* buffer) {
    if (data == nullptr) {
        std::cerr << "data is empty" << std::endl;
        exit(1);
    }
    memcpy(buffer, &length, sizeof(length));
    memcpy(buffer + sizeof(length), &header, sizeof(header));
    memcpy(buffer + sizeof(length) + sizeof(header), data, length - sizeof(length) - sizeof(header) - sizeof(endMarker));
    memcpy(buffer + length - sizeof(endMarker), &endMarker, sizeof(endMarker));
}

void Packet::unpacking(const char* buffer) {
    memcpy(&length, buffer, sizeof(length));
    memcpy(&header, buffer + sizeof(length), sizeof(header));
    int dataSize = length - sizeof(length) - sizeof(header) - sizeof(endMarker);
    if (data != nullptr) {
        free(data);
    }
    data = (char*)malloc(dataSize);
    if (data == nullptr) {
        std::cerr << "Memory allocation failed" << std::endl;
        exit(1);
    }
    memcpy(data, buffer + sizeof(length) + sizeof(header), dataSize);
    memcpy(&endMarker, buffer + length - sizeof(endMarker), sizeof(endMarker));

    if (endMarker != 0xffff) {
        std::cerr << "End Marker is not exist" << std::endl;
        exit(1);
    }

    // req_move 및 ack_move 상수 정의가 필요합니다.
    // 이 부분을 프로젝트의 요구 사항에 맞게 조정하세요.
    if (header == req_move || header == ack_move) {
        dataToXYZ();
    }
}
void Packet::clear() {
    // Reset all members to their default values
    length = 0;
    header = NULL;
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
    data = nullptr;
}