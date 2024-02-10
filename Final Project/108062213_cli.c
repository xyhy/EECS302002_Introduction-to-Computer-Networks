#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_SIZE 2048
#define ADDRESS_SIZE 20
#define MY_ERROR(s) printf(s); system("PAUSE"); exit(1);

int main(int argc,char *argv[]) {
	SOCKET serverSocket;
	struct sockaddr_in serverAddress;
	int bytesRead;
	char rec_buf[MAX_SIZE], send_buf[MAX_SIZE];
	char serverAddressStr[ADDRESS_SIZE];
	int serverPort;

    WSADATA wsadata;
  	if (WSAStartup(MAKEWORD(2,2),&wsadata) != 0) {
    	MY_ERROR("Winsock Error\n");
	}

	if (argc == 3) {
		strcpy(serverAddressStr, argv[1]);
		serverPort = atoi(argv[2]);
	}
  	
	// Set the server information
	memset(&serverAddress, 0, sizeof(serverAddress));
  	serverAddress.sin_family = AF_INET;
  	serverAddress.sin_addr.s_addr = inet_addr(serverAddressStr); // transform to 32-bit unsigned integer
  	serverAddress.sin_port = htons(serverPort); //converts a u_short from host to TCP/IP network byte order

    // Create socket
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (connect(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
    	MY_ERROR("Connect Error\n");
	}
	printf("Server IP is : %s \n", inet_ntoa(serverAddress.sin_addr));

	// call WSAStartup first for Winsock

	while (1) {
		recv(serverSocket, rec_buf, sizeof(rec_buf), 0);
        printf("%s\n", rec_buf);

        fflush(stdin);
		scanf(" %[^\n]", send_buf);
		// Send the data to the server, and receive it back
		send(serverSocket, send_buf, sizeof(send_buf), 0);
		if(!strncmp(send_buf, "exit", 4)) 	break;
		// memset(send_buf, '\0', sizeof(send_buf));
		// memset(rec_buf, '\0', sizeof(rec_buf));
	}
	printf("Disconnected\n");
	closesocket(serverSocket);
	return 0;
}
