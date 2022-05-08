#include <stdio.h>
#include <WinSock2.h>
#include <string.h> 
#include <stdlib.h> 
#include <string>
#include <iostream>
using namespace std;

#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)

#define INFO_BUFFER_SIZE 32767

int main()
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(8000);

    system("pause");

    //tao ket noi toi server
    int ret = connect(client, (SOCKADDR*)&addr, sizeof(addr));
    if (ret == SOCKET_ERROR)
    {
        ret = WSAGetLastError();
        printf("Ket noi khong thanh cong - %d\n", ret);
        return 1;
    }

    int action = 1;

    char buf[256];

    char buffer[2048];

    //nhan file welcome tu server
    while (true) {
        int welcomeFromServer = recv(client, buffer, sizeof(buffer), 0);

        if (welcomeFromServer <= 0)
        {
            if (welcomeFromServer == -1) {
                welcomeFromServer = WSAGetLastError();
            }
            break;
        }

        if (welcomeFromServer < sizeof(buffer)) {
            buffer[welcomeFromServer] = 0;
        }

        printf("%s \n", buffer);

        break;
    }

    //gui thong tin den server
    while (action == 1)
    {

        printf("Nhap noi dung tin nhan gui den server\n");

        string input;

        getline(cin, input);    

        const char* output = input.c_str();

        send(client, output, strlen(output), 0);

        int valread = recv(client, buf, sizeof(buf), 0);

        if (valread <= 0)
        {
            if (valread == -1) {
                valread = WSAGetLastError();
            }
            break;
        }

        if (valread < sizeof(buf)) {
            buf[valread] = 0;
        }

        printf("Tin nhan nhan gui toi server: %s\n", buf);

        printf("---------------------------- \n");

        if (strcmp((char*)&input, "bye") == 0)
        {
            action = 0;
        }
        fflush(stdin);
    }

    closesocket(client);

    WSACleanup();
}
