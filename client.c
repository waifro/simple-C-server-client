#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <string.h>

#define PORT "2705"

// using -lws2_32 on linker

int main(int argc, char *argv[]) {

    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    char buf[256];

    WSADATA wsaData;

    int res = WSAStartup(MAKEWORD(2,2), &wsaData);

    gethostname(buf, 256);

    /*
   if (argc < 3) {
      fprintf(stderr,"usage %s hostname port\n", argv[0]);
      exit(0);
   }
    */

    portno = atoi(PORT);

    // Create a socket point
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    server = gethostbyname(NULL);

    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    memset((char *)&serv_addr, 0x00, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    memcpy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    // Now connect to the server
    connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    // Now ask for a message from the user, this message
    // will be read by server

    printf("Please enter the message: ");
    memset(buffer, 0x00, 256);
    fgets(buffer, 255, stdin);

    printf("Message got it\n");

    // Send message to the server
    memcpy(&sockfd, buffer, sizeof(buffer));
    //n = fwrite(&sockfd, sizeof(char), sizeof(buffer), buffer);

    printf("Message sent\n");

    if (n < 0) {
        perror("ERROR writing to socket");
        exit(1);
    }

    // Now read server response
    memset(buffer, 0x00, 256);
    n = fread(buffer, sizeof(char), sizeof(sockfd), (FILE*)sockfd);

    if (n < 0) {
        perror("ERROR reading from socket");
        exit(1);
    }

    printf("%s\n",buffer);
    return 0;
}
