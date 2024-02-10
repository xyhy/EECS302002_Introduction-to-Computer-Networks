#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_SIZE 2048
#define ADDRESS_SIZE 20
#define MY_ERROR(s) printf(s); system("PAUSE"); exit(1);

int main(int argc , char *argv[]) {
	int serverSocket = 0;
	struct sockaddr_in serverAddress;
	int bytesRead;
	char buf[MAX_SIZE];
	char serverAddressStr[ADDRESS_SIZE];
	int serverPort;

	if (argc == 3) {
		strcpy(serverAddressStr, argv[1]);
		serverPort = atoi(argv[2]);
	}

	// Set the server information
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr(serverAddressStr); // transform to 32-bit unsigned integer
	serverAddress.sin_port = htons(serverPort); //converts a u_short from host to TCP/IP network byte order

	while (1) {
		printf("Enter something:\n");
		scanf("%s", buf);

		// Create socket
		serverSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (connect(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
			MY_ERROR("Connect Error\n");
		}
		printf("Server IP is : %s \n", inet_ntoa(serverAddress.sin_addr));

		// Send the data to the server, and receive it back
		send(serverSocket, buf, sizeof(buf), 0);
		bytesRead = recv(serverSocket, buf, sizeof(buf), 0);
		buf[bytesRead] = '\0';
		if (bytesRead > 0) {
			printf("Receive %d byte(s): %s\n", bytesRead, buf);
		}
		close(serverSocket);
	}

  return 0;
}
