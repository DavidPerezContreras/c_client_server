#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsa;
    SOCKET server_fd, client_fd;
    struct sockaddr_in server, client;
    int client_len;
    char buffer[BUFFER_SIZE] = {0};
    int total_received = 0;
    char ch;

    // Create named event for client synchronization
    HANDLE ready_event = CreateEvent(NULL, TRUE, FALSE, "ServerReady");
    if (ready_event == NULL) {
        printf("Failed to create event\n");
        return 1;
    }

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("WSAStartup failed: %d\n", WSAGetLastError());
        return 1;
    }

    // Create server socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET) {
        printf("Socket creation failed: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Bind to port
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed: %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    // Start listening
    if (listen(server_fd, 1) == SOCKET_ERROR) {
        printf("Listen failed: %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    printf("Server listening on port %d...\n", PORT);
    SetEvent(ready_event); // Signal that server is ready

    // Accept client connection
    client_len = sizeof(client);
    client_fd = accept(server_fd, (struct sockaddr*)&client, &client_len);
    if (client_fd == INVALID_SOCKET) {
        printf("Accept failed: %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    // Read until newline
    while (1) {
        int bytes = recv(client_fd, &ch, 1, 0);
        if (bytes <= 0) break;
        buffer[total_received++] = ch;
        if (ch == '\n' || total_received >= BUFFER_SIZE - 1) break;
    }

    buffer[total_received] = '\0';
    printf("Server received: %s", buffer);

    // Cleanup
    closesocket(client_fd);
    closesocket(server_fd);
    WSACleanup();
    CloseHandle(ready_event);
    return 0;
}
