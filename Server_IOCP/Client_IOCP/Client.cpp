//#define _WINSOCK_DEPRECATED_NO_WARNINGS
//#define _CRT_SECURE_NO_WARNINGS
//#pragma comment(lib, "ws2_32")
//#include <winsock2.h>
//#include <stdlib.h>
//#include <stdio.h>
//#include <time.h>
//
//#define SERVERIP   "127.0.0.1"
//#define SERVERPORT 9000
//#define BUFSIZE    512
//
//// ���� ��� �Լ�
//void err_quit(const char* msg);
//void err_display(const char* msg);
//
//int main(int argc, char* argv[])
//{
//    int retval;
//
//    // ���� �ʱ�ȭ
//    WSADATA wsa;
//    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;
//
//    // socket()
//    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
//    if (sock == INVALID_SOCKET) err_quit("socket()");
//
//    // ���� �ּ� ����
//    SOCKADDR_IN serveraddr;
//    ZeroMemory(&serveraddr, sizeof(serveraddr));
//    serveraddr.sin_family = AF_INET;
//    serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
//    serveraddr.sin_port = htons(SERVERPORT);
//
//    // ���� ����
//    retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
//    if (retval == SOCKET_ERROR) err_quit("connect()");
//    printf("[TCP Ŭ���̾�Ʈ] ������ ����Ǿ����ϴ�.\n");
//
//    // Ÿ���ڵ� ����
//    char sendbuf[BUFSIZE];
//    time_t currentTime;
//    struct tm* localTime;
//    // ���� �ð� ���ϱ�
//    time(&currentTime);
//    localTime = localtime(&currentTime);
//
//    // Ÿ���ڵ� �������� �ð� ���ڿ� ����
//    snprintf(sendbuf, BUFSIZE, "%d�� %d�� %02d�� %02d�� %02d�� %02d��",
//        localTime->tm_year + 1900, // tm_year�� 1900������� ����
//        localTime->tm_mon + 1,     // tm_mon�� 0���� �����ϹǷ� +1
//        localTime->tm_mday,
//        localTime->tm_hour,
//        localTime->tm_min,
//        localTime->tm_sec);
//
//    // ������ ����
//    retval = send(sock, sendbuf, (int)strlen(sendbuf), 0);
//    if (retval == SOCKET_ERROR) {
//        err_display("send()");
//    }
//
//    printf("[Ÿ���ڵ� ����] %s\n", sendbuf);
//    while (1) {
//        
//    }
//
//    // ���� �ݱ�
//    closesocket(sock);
//
//    // ���� ����
//    WSACleanup();
//    return 0;
//}
//
//// ���� �Լ� ���� ��� �� ����
//void err_quit(const char* msg)
//{
//    LPVOID lpMsgBuf;
//    FormatMessage(
//        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
//        NULL, WSAGetLastError(),
//        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
//        (LPTSTR)&lpMsgBuf, 0, NULL);
//    MessageBox(NULL, (LPCSTR)lpMsgBuf, msg, MB_ICONERROR);
//    LocalFree(lpMsgBuf);
//    exit(1);
//}
//
//// ���� �Լ� ���� ���
//void err_display(const char* msg)
//{
//    LPVOID lpMsgBuf;
//    FormatMessage(
//        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
//        NULL, WSAGetLastError(),
//        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
//        (LPTSTR)&lpMsgBuf, 0, NULL);
//    printf("[%s] %s\n", msg, (char*)lpMsgBuf);
//    LocalFree(lpMsgBuf);
//}
