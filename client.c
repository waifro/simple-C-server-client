#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <string.h>
#include <unistd.h>

// CLIENT

#define PORT 5001

// using -lws2_32 on linker
// https://docs.microsoft.com/en-us/windows/win32/winsock/windows-sockets-error-codes-2

int main(int argc, char *argv[]) {

    int sockfd;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256] = "ciao!";
    char buf[256];

    WSADATA wsaData;
    int res = WSAStartup(MAKEWORD(2,2), &wsaData);

    if (res < 0) {
        printf("Error startup: %d", WSAGetLastError());
        exit(1);
    }

    // Create a socket point
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    printf("sockfd %d\n", sockfd);

    if (sockfd < 0) {
        printf("Error opening socket: %d", WSAGetLastError());
        exit(2);
    }

    res = gethostname(buf, 256);

    if (res < 0) {
        printf("Error hostname: %d", WSAGetLastError());
        exit(3);
    }

    printf("found: %s\n", buf);
    server = gethostbyname(buf);

    if (server == NULL) {
        printf("Error host: %d", WSAGetLastError());
        exit(4);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr = *(struct in_addr *)*server -> h_addr_list;
    serv_addr.sin_port = htons(PORT);

    // Now connect to the server
    res = connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    printf("serv_addr %d\n", (int)sizeof(serv_addr));

    if (res < 0) {
       printf("Error connect socket: %d", WSAGetLastError());
        exit(5);
    }

    // Now ask for a message from the user, this message
    // will be read by server

    printf("Please enter the message: ciao!\n");
    //memset(buffer, 0x00, 256);
    //fgets(buffer, 255, stdin);

    // Send message to the server
    res = mempcpy(&sockfd, buffer, sizeof(sockfd));

    if (res < 0) {
       printf("Error writing socket: %d", WSAGetLastError());
        exit(6);
    }

    printf("\nMessage sent\n");

    // Now read server response
    //memset(buffer, 0x00, 256);
    //res = mempcpy(buffer, (void*)&sockfd, sizeof(sockfd));
    //res = mempcpy(buffer, sockfd, sizeof(sockfd));

    res = read(sockfd, buffer, sizeof(buffer));

    if (res < 0) {
        printf("Error reading socket: %d", WSAGetLastError());
        exit(7);
    }

    printf("Message received: %s\n", buffer);
    return 0;
}
