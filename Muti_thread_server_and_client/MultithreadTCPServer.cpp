#include "header.h"


// 소켓 함수 오류 출력 후 종료
void Server::error_quit(const char* msg)
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
void Server::error_display(const char* msg)
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

void Server::setReadyState()
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

void Server::communicate()
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

SOCKET Server::acceptConnection()
{
    addrlen = sizeof(clientaddr);
    SOCKET client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
    return client_sock;
}

HANDLE Server::createClientThread(SOCKET client_sock)
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
void Server::addClientRequest(Packet pkt)
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

DWORD WINAPI Server::sendRequest2Client(LPVOID arg) {
    Server* This = (Server*)arg;
    return This->sendRequest2Client_();
};

DWORD WINAPI Server::sendRequest2Client_()
{
    while (true) {
        WaitForSingleObject(hEvent, INFINITE);

        if (requestQueue.empty()) continue;

        WaitForSingleObject(hMutex, INFINITE);

        // 큐가 비었으면 종료
        

        Packet requestPkt = requestQueue.front();
        requestQueue.pop();


        for (int i = 0; i < clientCount;i++) {

            sendPacket(requestPkt, socketList[i]);

        }
        ReleaseMutex(hMutex);

    }
}

void Server::receiveLenPacket(SOCKET client_sock, int& length)
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

char* Server::receiveDataPacket(SOCKET client_sock, int length)
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
void Server::sendPacket(Packet pkt, SOCKET sock) {
    // 길이 보내기
    char pkt_len[sizeof(int)];
    pkt.getClength(pkt_len);

    int retval = send(sock, pkt_len, sizeof(int), 0);

    // 데이터 보내기
    char* pkt_buf = (char*)malloc(pkt.length);
    pkt.packing(pkt_buf);

    retval = send(sock, pkt_buf, pkt.length, 0);

    free(pkt_buf);

}

void Server::createConnectionMessage(bool isConnected,Packet* pkt ,const char* clientId, const char* ipAddress, int port) {
    // Get current time
    time_t now = time(0);
    tm* localTime = localtime(&now);

    // Buffer for formatted date and time string
    char timeStr[100];
    strftime(timeStr, sizeof(timeStr), "%Y년 %m월 %d일 %H시 %M분 %S초", localTime);

    // Determine connection status
    const char* status = isConnected ? "connected" : "disconnected";

    // Buffer for the final message
    char message[300];
    snprintf(message, sizeof(message), "%s, 클라이언트 %s %s : %s:%d", timeStr, clientId, status, ipAddress, port);
    printf("%s\n", message);
    // Return the message as a string
    pkt->importChat(message);
    pkt->setHeader(chat_string);
    addClientRequest(*pkt);
    SetEvent(hEvent);
    pkt->clear();
}

void Server::processPacket(Packet* pkt, Player* player )
{
    char* msg;
    if (pkt->header == req_con) {
        player->InsertID(pkt->data);
        createConnectionMessage(true, pkt, player->getID(), inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
        return;

    }
    else if (pkt->header == req_move) {
        player->addxyz(pkt->x, pkt->y, pkt->z);
        
        size_t msg_len = player->getID_len() + 30; // Rough estimate for the formatted string
        msg = (char*)malloc(msg_len);
        if (msg != nullptr) {
            sprintf_s(msg, msg_len, "[%s] : %.1f %.1f %.1f", player->getID(),
                player->getx(), player->gety(), player->getz());
            pkt->importChat(msg); // Uncomment if needed

        }
        printf("%s\n", msg);

        pkt->setHeader(ack_move);
        free(msg);

    }
    else if (pkt->header == chat_string) {
        size_t msg_len = strlen(pkt->data) + player->getID_len() + 10;
        msg = (char*)malloc(msg_len);
        if (msg != nullptr) {
            sprintf_s(msg, msg_len, "[%s] chat : %s", player->getID(), pkt->data);
            pkt->importChat(msg); // Uncomment if needed

        }
        printf("%s\n", msg);

        pkt->setHeader(chat_string);
        free(msg);

    }
    addClientRequest(*pkt);
    SetEvent(hEvent);
    pkt->clear();

}


DWORD WINAPI Server::ProcessClient(LPVOID arg) {
    Server* This = (Server*)arg;
    return This->ProcessClient_();
};


DWORD WINAPI Server::ProcessClient_()
{
    int num = clientCount;
    Player player;
    playerList[num] = player;
    int addrlen = sizeof(clientaddr);
    SOCKET sock = client_sock;
    Packet pkt;

    getpeername(sock, (SOCKADDR*)&clientaddr, &addrlen);

    while (1) {
        //recv

        int length = 0;
        receiveLenPacket(sock, length);
        if (length == -1) break;

        char* buf = receiveDataPacket(sock, length);
        if (buf == NULL) break;

        pkt.unpacking(buf);
        free(buf);

        processPacket(&pkt, &player);
        
    }
    createConnectionMessage(false, &pkt, player.getID(), inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
    
    //printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
    //    inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
    closesocket(sock);
    return 0;
}



Server::Server()
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


Server::~Server()
{
    closesocket(listen_sock);
    WSACleanup();

    CloseHandle(hEvent);
    CloseHandle(hMutex);
}


int main()
{
    Server server;
    return 0;
}
