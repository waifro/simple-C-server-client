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

    struct sockaddr_in server;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) { printf("Errid: %d", WSAGetLastError()); exit(1); }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    result = bind(sockfd, (struct sockaddr*)&server, sizeof(server));
    if (result < 0) { printf("Errid: %d", WSAGetLastError()); exit(1); }

    result = listen(sockfd, 5);
    if (result < 0) { printf("Errid: %d", WSAGetLastError()); exit(1); }

    // while the socket sockfd is listening, establish a new connection

    printf("Listening connection...\n");

    int newsockfd;
    newsockfd = accept(sockfd, NULL, NULL);
    if (newsockfd < 0) { printf("Errid: %d", WSAGetLastError()); exit(1); }

    printf("Connection from socket id: %d\n", newsockfd);

    char buf[256];

    result = recv(newsockfd, buf, sizeof(buf), 0);
    if (result < 0) { perror("Errno: "); exit(1); }

    printf("msg received: %s\n", buf);

    close(newsockfd);
    close(sockfd);
    WSACleanup();
    return 0;

}
