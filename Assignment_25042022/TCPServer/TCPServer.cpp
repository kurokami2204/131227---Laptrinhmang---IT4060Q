#include <stdio.h>
#include <WinSock2.h>
#include <string.h> 
#include <stdlib.h> 
#include <string>
#include <iostream>
using namespace std;

#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)

void ToUp(char* p);

int main()
{
    // Khoi tao thu vien
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    // Tao socket
    SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Khai bao dia chi server
    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(8000);

    // Gan cau truc dia chi voi socket
    bind(listener, (SOCKADDR*)&addr, sizeof(addr));

    // Chuyen sang trang thai cho ket noi
    listen(listener, 5);

    // Cho ket noi moi
    SOCKET client = accept(listener, NULL, NULL);

    FILE* welcomeClientFile = fopen("D:\\Winsock32 code\\1(1)\\Assignment_25042022\\Exercise 1\\welcome_client.txt", "r");

    FILE* clientMessage = fopen("D:\\Winsock32 code\\1(1)\\Assignment_25042022\\Exercise 1\\client_message.txt", "a");

    int ret;

    char buf[256];

    char buffer[2048];
    //doc roi gui file welcome toi client
    while (!feof(welcomeClientFile)) {
        ret = fread(buffer, 1, sizeof(buffer), welcomeClientFile);

        if (ret > 0)
        {
            send(client, buffer, ret, 0);
        }
    }
    //nhan tin nhan tu client
    while (1)
    {
        ret = recv(client, buf, sizeof(buf), 0);

        if (ret <= 0)
        {
            if (ret == -1) {
                ret = WSAGetLastError();
            }
            break;
        }

        if (ret < sizeof(buf)) {
            buf[ret] = 0;
        }

        fwrite(buf, 1, ret, clientMessage);

        fwrite("\n", sizeof(char), 1, clientMessage);

        printf("Tin nhan nhan duoc tu client: %s\n", buf);

        ToUp(buf);

        send(client, buf, sizeof(buf), 0);
    }

    fclose(welcomeClientFile);

    fclose(clientMessage);

    // Dong ket noi
    closesocket(client);
    closesocket(listener);
    WSACleanup();
}

void ToUp(char* p) {
    while (*p) {
        *p = toupper(*p);
        p++;
    }
}
