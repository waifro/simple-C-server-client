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

// CLIENT
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

    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) { perror("Errid: "); exit(1); }

    char buf[256];

    result = gethostname(buf, sizeof(buf));
    if (result < 0) { perror("Errid: "); exit(1); }

    printf("found: %s\n", buf);

    struct hostent *host;
    host = gethostbyname(buf);
    if (host == NULL) { perror("Errid: "); exit(1); }

    struct sockaddr_in server;

    server.sin_family = AF_INET;
    server.sin_addr = *(struct in_addr*)*host->h_addr_list;
    server.sin_port = htons(PORT);

    result = connect(sockfd, (struct sockaddr*)&server, sizeof(server));
    if (result < 0) { perror("Errid: "); exit(1); }

    printf("Connection established: %s\n", inet_ntoa(server.sin_addr));

    char text[256] = "hello everyone!";

    result = send(sockfd, text, strlen(text), 0);
    if (result < 0) { perror("Errno: "); exit(1); }

    printf("msg sent: %s\n", text);

    close(sockfd);
	
	#ifdef _WIN32
		WSACleanup();
	#endif
	
    return 0;

}
