#include "header.h"
class Server
{
private:
    WSADATA wsa;
    SOCKET listen_sock;
    SOCKET client_sock;
    SOCKET socketList[ClientMax];
    SOCKADDR_IN clientaddr;
    int addrlen;
    Player playerList[ClientMax];
    int clientCount;
    //Packet pkt;

    HANDLE hMutex;
    HANDLE hEvent;

    std::queue<Packet> requestQueue;

    char* msg;

    // 소켓 함수 오류 출력 후 종료
    void error_quit(const char* msg)
    {
        LPVOID lpMsgBuf;
        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
            NULL, WSAGetLastError(),
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR)&lpMsgBuf, 0, NULL);
        MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
        LocalFree(lpMsgBuf);
        exit(1);
    }

    // 소켓 함수 오류 출력
    void error_display(const char* msg)
    {
        LPVOID lpMsgBuf;
        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
            NULL, WSAGetLastError(),
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR)&lpMsgBuf, 0, NULL);
        printf("[%s] %s", msg, (char*)lpMsgBuf);
        LocalFree(lpMsgBuf);
    }

    void setReadyState()
    {
        int retval;

        listen_sock = socket(AF_INET, SOCK_STREAM, 0);
        if (listen_sock == INVALID_SOCKET) error_quit("socket()");

        // bind()
        SOCKADDR_IN serveraddr;
        ZeroMemory(&serveraddr, sizeof(serveraddr));
        serveraddr.sin_family = AF_INET;
        serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
        serveraddr.sin_port = htons(SERVERPORT);
        retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
        if (retval == SOCKET_ERROR) error_quit("bind()");

        // listen()
        retval = listen(listen_sock, SOMAXCONN);
        if (retval == SOCKET_ERROR) error_quit("listen()");
    }

    void communicate()
    {
        while (1) {
            // accept() and handle client connection
            client_sock = acceptConnection();
            if (client_sock == INVALID_SOCKET) {
                error_display("accept()");
                break;
            }

            socketList[clientCount] = client_sock;
            clientCount++;
            HANDLE hThread = createClientThread(client_sock);
            if (hThread == NULL) {
                closesocket(client_sock);
            }
            else {
                CloseHandle(hThread);
            }


        }
    }

    SOCKET acceptConnection()
    {
        addrlen = sizeof(clientaddr);
        SOCKET client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
        if (client_sock != INVALID_SOCKET) {
            printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
                inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
        }
        return client_sock;
    }

    HANDLE createClientThread(SOCKET client_sock)
    {
        if (clientCount < ClientMax) {




            return CreateThread(NULL, 0, ProcessClient, (void*)this, 0, NULL);
        }
        else {
            printf("Max clients reached, cannot accept more connections.\n");
            closesocket(client_sock);
            return NULL;
        }
    }
    void addClientRequest(Packet pkt)
    {
        DWORD dwWaitCode = WaitForSingleObject(hMutex, INFINITE);
        if (dwWaitCode == WAIT_FAILED)
        {
            cout << "WaitForSingleObject failed : " << GetLastError() << endl;
            return;
        }
        if (dwWaitCode == WAIT_ABANDONED)
            cout << "Abandoned Mutex acquired!!!" << GetLastError() << endl;

        requestQueue.push(pkt);
        ReleaseMutex(hMutex);
    }

    static DWORD WINAPI sendRequest2Client(LPVOID arg) {
        Server* This = (Server*)arg;
        return This->sendRequest2Client_();
    };

    DWORD WINAPI sendRequest2Client_()
    {
        while (true) {
            WaitForSingleObject(hEvent, INFINITE);

            WaitForSingleObject(hMutex, INFINITE);

            // 큐가 비었으면 종료
            if (requestQueue.empty()) continue;

            Packet requestPkt = requestQueue.front();
            requestQueue.pop();


            for (int i = 0; i < clientCount;i++) {

                sendPacket(requestPkt, socketList[i]);

            }
            ReleaseMutex(hMutex);

        }
    }

    void receiveLenPacket(SOCKET client_sock, int& length)
    {
        int retval;
        char lengthBuf[sizeof(int)];
        retval = recv(client_sock, lengthBuf, sizeof(int), 0);
        if (retval == SOCKET_ERROR || retval == 0) {
            error_display("recv() - length");
            length = -1;
            return;
        }

        memcpy(&length, lengthBuf, sizeof(int));
    }

    char* receiveDataPacket(SOCKET client_sock, int length)
    {
        char* buf = (char*)malloc(length);
        if (buf == NULL) {
            std::cerr << "Memory allocation failed" << std::endl;
            return NULL;
        }

        int retval = recv(client_sock, buf, length, 0);
        if (retval == SOCKET_ERROR || retval == 0) {
            error_display("recv() - data");
            free(buf);
            return NULL;
        }

        return buf;
    }
    void sendPacket(Packet pkt, SOCKET sock) {
        // 길이 보내기
        char pkt_len[sizeof(int)];
        pkt.getClength(pkt_len);

        int retval = send(sock, pkt_len, sizeof(int), 0);
        //if (retval == SOCKET_ERROR) {
        //    error_display("send()");
        //    return;
        //}

        // 데이터 보내기
        char* pkt_buf = (char*)malloc(pkt.length);
        pkt.packing(pkt_buf);

        retval = send(sock, pkt_buf, pkt.length, 0);
        if (retval == SOCKET_ERROR) {
            error_display("send()");
            free(pkt_buf);
            return;
        }

        free(pkt_buf);

    }

public:
    Server()
    {
        hMutex = CreateMutex(NULL, FALSE, NULL);
        hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

        if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
            error_quit("WSAStartup()");
        printf("Server is ON\n");

        CreateThread(NULL, 0, sendRequest2Client, (void*)this, 0, NULL);

        setReadyState();

        communicate();
    }





 




    void processPacket(Packet* pkt, Player* player)
    {

        if (pkt->header == req_con) {
            player->InsertID(pkt->data);
            printf("\n[%s] Logged in", player->getID());
            char* msg = (char*)malloc(player->getID_len() + 12);
            if (msg != nullptr) {
                sprintf_s(msg, player->getID_len() + 12, "[%s] Logged in", player->getID());
                pkt->importChat(msg); // Uncomment if needed

            }
            pkt->setHeader(ack_con);

        }

        else if (pkt->header == chat_string) {
            printf("\n[%s] chat : %s", player->getID(), pkt->data);
            size_t msg_len = strlen(pkt->data) + player->getID_len() + 10;
            char* msg = (char*)malloc(msg_len);
            if (msg != nullptr) {
                sprintf_s(msg, msg_len, "[%s] chat : %s", player->getID(), pkt->data);
                pkt->importChat(msg); // Uncomment if needed

            }
            pkt->setHeader(chat_string);

        }

        else if (pkt->header == req_move) {
            player->addxyz(pkt->x, pkt->y, pkt->z);
            printf("\n[%s] : %.1f %.1f %.1f", player->getID(),
                player->getx(), player->gety(), player->getz());

            size_t msg_len = player->getID_len() + 30; // Rough estimate for the formatted string
            char* msg = (char*)malloc(msg_len);
            if (msg != nullptr) {
                sprintf_s(msg, msg_len, "[%s] : %.1f %.1f %.1f", player->getID(),
                    player->getx(), player->gety(), player->getz());
                pkt->importChat(msg); // Uncomment if needed

            }
            pkt->setHeader(ack_move);

        }
        free(msg);
        pkt->clear();

    }


    static DWORD WINAPI ProcessClient(LPVOID arg) {
        Server* This = (Server*)arg;
        return This->ProcessClient_();



    };
    // 클라이언트와 데이터 통신
    DWORD WINAPI ProcessClient_()
    {
        int num = clientCount;
        Player player;
        playerList[clientCount] = player;
        int addrlen = sizeof(clientaddr);
        SOCKET sock = client_sock;
        Packet pkt;

        getpeername(sock, (SOCKADDR*)&clientaddr, &addrlen);

        while (1) {
            //recv
            
            int length;
            receiveLenPacket(sock, length);
            if (length == -1) break;

            char* buf = receiveDataPacket(sock, length);
            if (buf == NULL) break;

            pkt.unpacking(buf);
            free(&buf);

            processPacket(&pkt, &player);
            addClientRequest(pkt);
            SetEvent(hEvent);
        }

        closesocket(sock);
        printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
            inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

        return 0;
    }

    ~Server()
    {
        closesocket(listen_sock);
        WSACleanup();

        CloseHandle(hEvent);
        CloseHandle(hMutex);
    }
};

int main()
{
    Server server;
    return 0;
}
