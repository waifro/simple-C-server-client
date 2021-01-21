#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <string.h>
#include <unistd.h>
#define PORT 5001

// SERVER
// using -lws2_32 on linker

int main(void) {

    // initializing windows socket
    WSADATA WsaData;
    int result = WSAStartup(MAKEWORD(2,2), &WsaData);
    if (result < 0) { printf("Errid: %d", WSAGetLastError()); exit(1); }

    int sockfd;

    struct sockaddr_in serv_addr;
    struct sockaddr_in  cli_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) { printf("Errid: %d", WSAGetLastError()); exit(1); }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htonl(PORT);

    result = bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (result < 0) { printf("Errid: %d", WSAGetLastError()); exit(1); }

    result = listen(sockfd, 5);
    if (result < 0) { printf("Errid: %d", WSAGetLastError()); exit(1); }

    // while the socket sockfd is listening, establish a new connection

    printf("Listening connection...\n");

    int newsockfd;
    newsockfd = accept(sockfd, NULL, NULL);
    if (newsockfd < 0) { printf("Errid: %d", WSAGetLastError()); exit(1); }

    printf("Connection estabilished...\n");

    WSACleanup();
    return 0;

}
