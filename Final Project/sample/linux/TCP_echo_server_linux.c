#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_SIZE 2048
#define MY_ERROR(s) printf(s); system("PAUSE"); exit(1);

int main(int argc , char *argv[]) {
	int serverSocket = 0, clientSocket = 0;
	struct sockaddr_in serverAddress, clientAddress; // sockaddr_in：IP4 格式使用 ,  sockaddr_in6：IP6 格式使用 , sockaddr：通用格式
	unsigned clientAddressLen;
	int bytesRead;
	char buf[MAX_SIZE];
	int serverPort;

	if (argc == 2) {
		serverPort = atoi(argv[1]);
	}

	// Create socket
	serverSocket = socket(PF_INET , SOCK_STREAM , 0); // (address , type , protocal(0表示不強制) )

	// Set the server information
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(serverPort); //converts a u_short from host to TCP/IP network byte order

	// Bind the socket
	if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
		MY_ERROR("Bind Error\n");
	}

	// Prepare to listen to the incoming clients
	if (listen(serverSocket, 3) < 0) {
		MY_ERROR("Listen Error\n");
	}

	while (1) {
		printf("Waiting...\n");

		// Accept a client
		clientAddressLen = sizeof(clientAddress);
		clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLen);

		// Receive the data from the client, and send it back
		bytesRead = recv(clientSocket, buf, sizeof(buf), 0);
		buf[bytesRead] = '\0';
		send(clientSocket, buf, bytesRead, 0);
		printf("Client IP is : %s \n", inet_ntoa(clientAddress.sin_addr));
		printf("Receive %d byte(s): %s\n", bytesRead, buf);
		close(clientSocket);
	}

  return 0;
}
