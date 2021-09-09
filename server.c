#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef _WIN32
	#include <winsock2.h>
#else
	#include <netinet/in.h>
	#include <sys/socket.h>
	#include <sys/types.h>
	#include <arpa/inet.h>
	#include <netdb.h>
#endif

#define PORT 5001

// SERVER
// using -lws2_32 on linker

int main(void) {
    
    int result = 0;

    #ifdef _WIN32
		// initializing windows socket
		WSADATA WsaData;
		result = WSAStartup(MAKEWORD(2,2), &WsaData);
		if (result < 0) { printf("Errid: %d", WSAGetLastError()); exit(1); }
    #endif

    int sockfd;

    struct sockaddr_in server;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        
        #ifdef _WIN32
	     printf("Errid: %d", WSAGetLastError()); exit(1);
        #else
	     perror("Errid: "); exit(1);
        #endif
	
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    result = bind(sockfd, (struct sockaddr*)&server, sizeof(server));
    if (result < 0) {
        
        #ifdef _WIN32
	     printf("Errid: %d", WSAGetLastError()); exit(1);
        #else
	     perror("Errid: "); exit(1);
        #endif
	
    }

    result = listen(sockfd, 5);
    if (result < 0) {
        
        #ifdef _WIN32
	     printf("Errid: %d", WSAGetLastError()); exit(1);
        #else
	     perror("Errid: "); exit(1);
        #endif
	
    }

    // while the socket sockfd is listening, establish a new connection

    printf("Listening connection...\n");

    int newsockfd;
    newsockfd = accept(sockfd, NULL, NULL);
    if (newsockfd < 0) {
        
        #ifdef _WIN32
	     printf("Errid: %d", WSAGetLastError()); exit(1);
        #else
	     perror("Errid: "); exit(1);
        #endif
	
    }

    printf("Connection from socket id: %d\n", newsockfd);

    char buf[256];

    result = recv(newsockfd, buf, sizeof(buf), 0);
    if (result < 0) {
        
        #ifdef _WIN32
	     printf("Errid: %d", WSAGetLastError()); exit(1);
        #else
	     perror("Errid: "); exit(1);
        #endif
	
    }

    printf("msg received: %s\n", buf);

    close(newsockfd);
    close(sockfd);
    
	#ifdef _WIN32
		WSACleanup();
	#endif

    return 0;

}
