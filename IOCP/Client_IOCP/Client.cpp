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
//// 오류 출력 함수
//void err_quit(const char* msg);
//void err_display(const char* msg);
//
//int main(int argc, char* argv[])
//{
//    int retval;
//
//    // 윈속 초기화
//    WSADATA wsa;
//    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;
//
//    // socket()
//    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
//    if (sock == INVALID_SOCKET) err_quit("socket()");
//
//    // 서버 주소 설정
//    SOCKADDR_IN serveraddr;
//    ZeroMemory(&serveraddr, sizeof(serveraddr));
//    serveraddr.sin_family = AF_INET;
//    serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
//    serveraddr.sin_port = htons(SERVERPORT);
//
//    // 서버 연결
//    retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
//    if (retval == SOCKET_ERROR) err_quit("connect()");
//    printf("[TCP 클라이언트] 서버에 연결되었습니다.\n");
//
//    // 타임코드 전송
//    char sendbuf[BUFSIZE];
//    time_t currentTime;
//    struct tm* localTime;
//    // 현재 시간 구하기
//    time(&currentTime);
//    localTime = localtime(&currentTime);
//
//    // 타임코드 형식으로 시간 문자열 생성
//    snprintf(sendbuf, BUFSIZE, "%d년 %d월 %02d일 %02d시 %02d분 %02d초",
//        localTime->tm_year + 1900, // tm_year는 1900년부터의 연도
//        localTime->tm_mon + 1,     // tm_mon은 0부터 시작하므로 +1
//        localTime->tm_mday,
//        localTime->tm_hour,
//        localTime->tm_min,
//        localTime->tm_sec);
//
//    // 서버로 전송
//    retval = send(sock, sendbuf, (int)strlen(sendbuf), 0);
//    if (retval == SOCKET_ERROR) {
//        err_display("send()");
//    }
//
//    printf("[타임코드 전송] %s\n", sendbuf);
//    while (1) {
//        
//    }
//
//    // 소켓 닫기
//    closesocket(sock);
//
//    // 윈속 종료
//    WSACleanup();
//    return 0;
//}
//
//// 소켓 함수 오류 출력 후 종료
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
//// 소켓 함수 오류 출력
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
