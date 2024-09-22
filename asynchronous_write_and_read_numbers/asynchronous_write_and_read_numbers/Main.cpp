#define _CRT_SECURE_NO_WARNINGS

#include "stdafx.h"
#include "windows.h"
#include "iostream"
#include <cstring>
#include <cstdlib>

using namespace std;

class AsyncFileProcess {
private:
    DWORD dwErrCode;
    HANDLE hSrcFile;
    OVERLAPPED wo;
    OVERLAPPED ro;
    size_t size = 512;        // 버퍼 크기
    int data_count = 0;       // 파싱된 숫자 개수

    char* writeData;          // 파일에 쓸 데이터
    char* readData;           // 파일에서 읽은 데이터
    int* numberList;          // 파싱된 숫자 목록

public:
    AsyncFileProcess() : dwErrCode(0) {
        wo = { 0 };
        ro = { 0 };
        readData = new char[size];  // 읽기 버퍼 초기화
    }

    // 파일 생성 함수
    void createFile(const wchar_t* filePath) {
        hSrcFile = CreateFile(
            filePath, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
            FILE_FLAG_OVERLAPPED, NULL
        );
        if (hSrcFile == INVALID_HANDLE_VALUE) {
            cout << filePath << " open failed, code : " << GetLastError() << endl;
        }
    }

    // 1부터 100까지의 숫자 문자열을 생성
    char* generateNumber() {
        char* returnChar = new char[size];
        returnChar[0] = '\0';  // 빈 문자열로 초기화
        char buffer[4];

        for (int i = 1; i <= 100; ++i) {
            snprintf(buffer, sizeof(buffer), "%d ", i);
            strcat_s(returnChar, size, buffer);
        }
        strcat_s(returnChar, size, "\n");
        return returnChar;
    }

    // 파일 데이터를 파싱하여 숫자 배열로 변환
    void parseData() {
        int count = 0;
        int* result = nullptr;    // 동적으로 할당할 배열을 위한 포인터
        int capacity = 10;        // 초기 배열 크기
        result = (int*)malloc(capacity * sizeof(int));

        // 입력 문자열 복사
        char* dataCopy = new char[strlen(readData) + 1];
        strcpy(dataCopy, readData);

        // 공백을 기준으로 토큰화
        char* token = strtok(dataCopy, " ");

        while (token != NULL) {
            if (count >= capacity) {
                capacity *= 2;  // 배열 크기 확장
                result = (int*)realloc(result, capacity * sizeof(int));
            }
            result[count++] = atoi(token);  // 문자열을 정수로 변환
            token = strtok(NULL, " ");
        }

        delete[] dataCopy;  // 동적으로 할당한 메모리 해제
        result = (int*)realloc(result, count * sizeof(int));  // 배열 크기 조정

        numberList = result;
        data_count = count;  // 숫자 개수 저장
    }

    // 배열에 있는 모든 숫자를 더하는 함수
    int sumArray() {
        int sum = 0;
        for (int i = 0; i < data_count; ++i) {
            sum += numberList[i];
        }
        return sum;
    }

    // 파일에 데이터를 비동기로 쓰기
    void writeFile(const char* writeData) {
        DWORD bytesWritten;
        bool bIsOK = WriteFile(
            hSrcFile, writeData, strlen(writeData), &bytesWritten, &wo
        );
        if (!bIsOK) {
            dwErrCode = GetLastError();
            if (dwErrCode != ERROR_IO_PENDING) {
                printf("Write file error, code: %d\n", dwErrCode);
            }
        }
    }

    // 파일에서 데이터를 비동기로 읽기
    void readFile() {
        BOOL bIsOK = ReadFile(
            hSrcFile, readData, size, NULL, &ro
        );
    }

    // 전체 실행 과정
    void run(const wchar_t* filePath) {
        writeData = generateNumber();     // 데이터 생성
        createFile(filePath);             // 파일 생성
        writeFile(writeData);             // 데이터 쓰기
        DWORD dwWaitRet = WaitForSingleObject(hSrcFile, INFINITE);  // 쓰기 완료 대기

        readFile();                       // 데이터 읽기
        dwWaitRet = WaitForSingleObject(hSrcFile, INFINITE);  // 읽기 완료 대기
        parseData();                      // 읽은 데이터 파싱
        int result = sumArray();          // 숫자 합산

        printf("Data sum result : %d", result);  // 결과 출력
    }

    // 소멸자
    ~AsyncFileProcess() {
        delete[] writeData;
        if (hSrcFile != INVALID_HANDLE_VALUE) {
            CloseHandle(hSrcFile);
        }
    }
};

void _tmain(int argc, _TCHAR* argv[]) {
    if (argc < 2) {
        cout << "Usage: FileCopySync SourceFile" << endl;
        return;
    }
    AsyncFileProcess process;
    process.run(argv[1]);  // 파일 경로 실행
}
