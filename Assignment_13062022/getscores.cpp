#include <winsock2.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <map>

#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)

using namespace std;


// Numbers of clients
SOCKET clients[64];
int numClients = 0;
map<int, string> listUser;

void RemoveClient(SOCKET client)
{
    // Find the position of client in array
    int i = 0;
    for (; i < numClients; i++)
        if (clients[i] == client) break;
    // Delete client out of array
    if (i < numClients - 1)
        clients[i] = clients[numClients - 1];
    numClients--;


}
DWORD WINAPI ClientThread(LPVOID lpParam)
{
    SOCKET client = *(SOCKET*)lpParam;
    int ret;
    char buf[256];
    // 32 bit for name, 32 bit for password, 5 bit for scores
    char user[32], pass[32], scores[5];

    // Sign in processing
    while (1)
    {
        ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0)
            return 0;
        buf[ret] = 0;
        printf("Data receive: %s\n", buf);
        ret = sscanf(buf, "%s %s %s", user, pass, scores);
        if (ret != 2)
        {
            const char* msg = "Wrong command. Please try again!\n";
            send(client, msg, strlen(msg), 0);
        }
        else
        {
            FILE* f = fopen("D:\\Winsock32 code\\1(1)\\Assignment_16052022\\userDB.txt", "r");

            int found = 0;
            while (!feof(f))
            {
                char line[256];
                fgets(line, sizeof(line), f);
                char userf[32], passf[32];
                sscanf(line, "%s %s", userf, passf);
                if (strcmp(user, userf) == 0 && strcmp(pass, passf) == 0)
                {
                    found = 1;
                    break;
                }
            }
            fclose(f);
            if (found == 1)
            {
                const char* msg = "Sign in successfully\n";

                char line[256];
                fgets(line, sizeof(line), f);
                char userf[32], scoresf[32];
                sscanf(line, "%s %s", userf, scoresf);
                if (strcmp(user, userf) == 0 && strcmp(scores, scoresf) == 0)
                {
                    printf("user + mark ")
                }
            }
            fclose(f);

                send(client, msg, strlen(msg), 0);
                // Notify when new client connected
                const char* newMsg = "New client connected";
                char newBuf[256];
                sprintf(newBuf, "%s: %s\n", newMsg, user);
                for (int i = 0; i < numClients; i++) {
                    send(clients[i], newBuf, strlen(newBuf), 0);
                }
                // Add new client to array
                clients[numClients] = client;

                listUser.insert({ client, user });

                numClients++;
                break;
            }
            else
            {
                const char* msg = "User not found. Please try again! \n";
                send(client, msg, strlen(msg), 0);
            }
        }
    }

    // Sending msg
    while (1)
    {
        char cmd[32];
        ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0)
        {
            RemoveClient(client);
            return 0;
        }
        buf[ret] = 0;
        printf("Message data from client %s\n", buf);
        // Data processing
        // Read the first word and save to cmd
        sscanf(buf, "%s", cmd);     //Read every consecutive word until character space appear 
        char sbuf[256];
        int id;
        sprintf(sbuf, "%s: %s", user, buf + 4);      
        // Log out when type "exit"
        if (strcmp(cmd, "/exit") == 0) {
            const char* msg = "You logged out! \n";
            send(client, msg, strlen(msg), 0);
            //Notify when client log out
            const char* newMsg = "Client log out";
            char newBuf[256];
            sprintf(newBuf, "%s: %s\n", newMsg, user);
            for (int i = 0; i < numClients; i++) {
                if (clients[i] != client) {
                    send(clients[i], newBuf, strlen(newBuf), 0);
                }
            }

            auto client_out = listUser.find(client);

            listUser.erase(client_out);

            RemoveClient(client);
            return 0;
        }     
        else { // Private chat
            id = atoi(cmd); // chuyen chuoi cmd sang so
            // send to client with id = ...
            for (int i = 0; i < numClients; i++)
                if (clients[i] == id)
                {
                    char* msg = buf + strlen(cmd) + 1;
                    send(clients[i], sbuf, strlen(sbuf), 0);
                }
        }

    }


    closesocket(client);
    WSACleanup();
}
int main()
{

    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
    // Tao socket
    SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(8000);

    bind(listener, (SOCKADDR*)&addr, sizeof(addr));

    listen(listener, 5);
    while (1)
    {
        SOCKET client = accept(listener, NULL, NULL);
        printf("New client connected: %d\n", client);
        CreateThread(0, 0, ClientThread, &client, 0, 0);
    }
}
