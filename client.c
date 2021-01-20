#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <string.h>

#define PORT "2705"

// using -lws2_32 on linker
// https://docs.microsoft.com/en-us/windows/win32/winsock/windows-sockets-error-codes-2

int main(int argc, char *argv[]) {

    int sockfd = 0;
    int portno = 0;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    char buf[256];

    WSADATA wsaData;

    int res = WSAStartup(MAKEWORD(2,2), &wsaData);

    if (res < 0) {
        printf("Error startup: %d", WSAGetLastError());
        exit(1);
    }

    portno = atoi(PORT);

    // Create a socket point
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        printf("Error opening socket: %d", WSAGetLastError());
        exit(2);
    }

    gethostname(buf, 256);
    server = gethostbyname(buf);

    if (server == NULL) {
        printf("Error host: %d", WSAGetLastError());
        exit(3);
    }

    memset((char *)&serv_addr, 0x00, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    memcpy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    // Now connect to the server
    res = connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    if (res < 0) {
       printf("Error connect socket: %d", WSAGetLastError());
        exit(4);
    }

    // Now ask for a message from the user, this message
    // will be read by server

    printf("Please enter the message: ");
    memset(buffer, 0x00, 256);
    fgets(buffer, 255, stdin);

    printf("Message got it\n");

    // Send message to the server
    res = memcpy(&sockfd, buffer, sizeof(buffer));
    //res = fwrite(&sockfd, sizeof(char), sizeof(buffer), buffer);

    if (res < 0) {
       printf("Error writing socket: %d", WSAGetLastError());
        exit(5);
    }

    printf("Message sent\n");

    // Now read server response
    memset(buffer, 0x00, 256);
    res = fread(buffer, sizeof(char), sizeof(sockfd), (FILE*)sockfd);

    if (res < 0) {
        printf("Error reading socket: %d", WSAGetLastError());
        exit(6);
    }

    printf("%s\n", buffer);
    return 0;
}
