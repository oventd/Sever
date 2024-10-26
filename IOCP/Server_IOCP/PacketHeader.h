#pragma once
#define PLAYER_CONNECTED		0
#define PLAYER_DISCONNECTED		1
#define PLAYER_GAMERESULT		2
#define PLAYER_MOVE				3
#define PLAYER_HIT				4
#define GAME_START				5
#define GAME_END				6
#define GAME_INFO				7
#define FALL_CREATE				8

class Packet {
public:
    int length;
    int header;
    char* data;
    unsigned short endMarker;


    Packet();
    Packet(int len, char hdr, char* dat);
    ~Packet();

    void setHeader(char hdr);
    void getClength(char* len);

    void importXYZ(float f1, float f2, float f3);
    void dataToXYZ();
    void importChat(const char* chat);

    void packing(char* buffer);
    void unpacking(const char* buffer);

    void clear();

    void processHeader();
};