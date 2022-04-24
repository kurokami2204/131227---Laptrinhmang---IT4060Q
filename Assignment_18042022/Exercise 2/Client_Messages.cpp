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
    addr.sin_port = htons(1234);

    system("pause");

    int ret = connect(client, (SOCKADDR*)&addr, sizeof(addr));
    if (ret == SOCKET_ERROR)
    {
        ret = WSAGetLastError();
        printf("Ket noi khong thanh cong - %d\n", ret);
        return 1;
    }

    int action = 1;

    while (action == 1)
    {
        printf("Nhap noi dung tin nhan gui den server\n");

        string input;

        getline(cin, input);

        string message = "Information by client: " + input;

        const char* output = message.c_str();

        send(client, output, strlen(output), 0);

        if (strcmp((char*)&input, "bye") == 0)
        {
            action = 0;
        }
        fflush(stdin);
    }

    closesocket(client);

    WSACleanup();
}
