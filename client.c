#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080

int main()
{
    WSADATA wsa;
    SOCKET client_fd;
    struct sockaddr_in server;
    const char *message = "Hello, world!\naaaaaaaaaa"; //Server stops

    HANDLE ready_event = NULL;
    while (ready_event == NULL)
    {
        ready_event = OpenEvent(EVENT_ALL_ACCESS, FALSE, "ServerReady");
        if (ready_event == NULL)
        {
            printf("Client: waiting for server to be ready...\n");
            Sleep(500);
        }
    }
    WaitForSingleObject(ready_event, INFINITE);
    WaitForSingleObject(ready_event, INFINITE);

    WaitForSingleObject(ready_event, INFINITE); // Wait for server to be ready

    WSAStartup(MAKEWORD(2, 2), &wsa);

    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(client_fd, (struct sockaddr *)&server, sizeof(server));
    send(client_fd, message, strlen(message), 0);
    printf("Client sent: %s\n", message);

    closesocket(client_fd);
    WSACleanup();
    CloseHandle(ready_event);
    return 0;
}
