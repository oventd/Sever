#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32")
#pragma once
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <cstring>
#include <string>
#include <sstream>
#include <conio.h>
#include <queue>
using namespace std;

#define req_con 'C'       
#define ack_con 'A'       
#define req_move 'M'     
#define ack_move 'K'      
#define chat_string 'S'


#define SERVERPORT 9000
#define ClientMax 20


class Packet {
public:
    int length;
    char header;
    char* data;
    unsigned short endMarker;

    float x;
    float y;
    float z;

    Packet();
    Packet(int len, char hdr, char* dat, float x, float y, float z);
    ~Packet();

    void setHeader(char hdr);
    void getClength(char* len);

    void importXYZ(float f1, float f2, float f3);
    void dataToXYZ();
    void importChat(const char* chat);

    void packing(char* buffer);
    void unpacking(const char* buffer);

    void clear();
};

class Player {
private:
    char* id;
    int id_len;
    float x;
    float y;
    float z;
public:
    Player() : id(nullptr), id_len(0), x(0.0f), y(0.0f), z(0.0f) 
    {
    };
    Player(float x, float y, float z) : id(NULL), id_len(0), x(x), y(y), z(z) {};
    ~Player() {     }
    //~clientCharacter();
    char* getID() { return id; }
    int getID_len() { return id_len; }
    float getx() { return x; }
    float gety() { return y; }
    float getz() { return z; }

    void addx(float input) { x += input; }
    void addy(float input) { y += input; }
    void addz(float input) { z += input; }

    void addxyz(float inputx, float inputy, float inputz) {
        addx(inputx); addy(inputy);addz(inputz);
    }

    void InsertID(const char* input) {
        if (id != nullptr) {
            free(id);
        }
        id_len = strlen(input) + 1;
        id = (char*)malloc(id_len);
        if (id == nullptr) {
            std::cerr << "Memory allocation failed" << std::endl;
            exit(1);
        }
        strcpy_s(id, id_len, input);
    }
};

class Server
{
private:
    WSADATA wsa;
    SOCKET listen_sock;
    SOCKET client_sock;
    SOCKET socketList[ClientMax];
    SOCKADDR_IN clientaddr;
    int addrlen;
    int clientCount;
    Player playerList[ClientMax];


    HANDLE hMutex;
    HANDLE hEvent;

    std::queue<Packet> requestQueue;

    void error_quit(const char* msg);
    void error_display(const char* msg);
    void setReadyState();
    void communicate();
    SOCKET acceptConnection();
    HANDLE createClientThread(SOCKET client_sock);
    void addClientRequest(Packet pkt);
    static DWORD WINAPI sendRequest2Client(LPVOID arg);
    DWORD WINAPI sendRequest2Client_();
    void receiveLenPacket(SOCKET client_sock, int& length);
    char* receiveDataPacket(SOCKET client_sock, int length);
    void sendPacket(Packet pkt, SOCKET sock);
    void createConnectionMessage(bool isConnected, Packet* pkt, const char* clientId, const char* ipAddress, int port);
    void processPacket(Packet* pkt, Player* player );
    static DWORD WINAPI ProcessClient(LPVOID arg);
    DWORD WINAPI ProcessClient_();
public:
    Server();
    ~Server();

};



