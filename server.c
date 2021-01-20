#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <string.h>

// using -lws2_32 on linker

int main( int argc, char *argv[] ) {

    WSADATA wsaData;

    int res = WSAStartup(MAKEWORD(2,2), &wsaData);

    if (res < 0) {
        printf("Error startup: %d", WSAGetLastError());
        exit(1);
    }

    int sockfd = 0;
    int newsockfd = 0;
    int portno = 0;
    int clilen = 0;

    char buffer[256];
    char message[256] = "I got your message";

    struct sockaddr_in serv_addr, cli_addr;

    // First call to socket() function
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        printf("Error socket: %d", WSAGetLastError());
        exit(2);
    }

    // Initialize socket structure
    memset((char *) &serv_addr, 0x00, sizeof(serv_addr));
    portno = 5001;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    // Now bind the host address using bind() call.
    bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

    // Now start listening for the clients, here process will
    // go in sleep mode and will wait for the incoming connection
    printf("Listening connection...");

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    // Accept actual connection from the client
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

    printf("Connection estabilished...");

    // If connection is established then start communicating
    memset(buffer, 0x00, 256);
    res = fread(&newsockfd, sizeof(buffer), sizeof(char), (FILE*)buffer);

    if (res < 0) {
        printf("Error reading socket: %d", WSAGetLastError());
        exit(3);
    }


    printf("Here is the message: %s\n",buffer);

    // Write a response to the client
    res = fwrite(&newsockfd, sizeof(char), sizeof(message), (FILE*)message);

    if (res < 0) {
        printf("Error writing socket: %d", WSAGetLastError());
        exit(4);
    }

    return 0;
}
