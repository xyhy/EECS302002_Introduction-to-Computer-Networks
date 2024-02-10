#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define MAX_SIZE 2048
#define MY_ERROR(s) printf(s); system("PAUSE"); exit(1);

int main(int argc, char* argv[]) {
    SOCKET serverSocket, clientSocket; // create a socket
	struct sockaddr_in serverAddress, clientAddress; // sockaddr_in：IP4 格式使用 ,  sockaddr_in6：IP6 格式使用 , sockaddr：通用格式
	int clientAddressLen = sizeof(clientAddress);
	int bytesRead, bytesSend;
	char rec_buf[MAX_SIZE], send_buf[MAX_SIZE];
	int serverPort;
    // call WSAStartup first for Winsock
    WSADATA wsadata;
    if( WSAStartup(MAKEWORD(2,2),(LPWSADATA)&wsadata) != 0) { // ( version of winsock )
        MY_ERROR("Winsock Error\n");
	}
    // Create socket
	serverSocket = socket(PF_INET, SOCK_STREAM, 0); // (address , type , protocal(0表示不強制) )
	if(serverSocket < 0){
		MY_ERROR("Create Server socket failed");
	}

    if (argc == 2) {
		serverPort = atoi(argv[1]);
	}

    // Set the server information
	memset(&serverAddress, 0, sizeof(serverAddress));
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

    printf("Waiting...\n");

    //Accept a client
	clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLen);

	printf("Client connect successfully\n");

    char menu[MAX_SIZE] = "\
    \n\n--------Menu--------\n\
    1. Read all messages.\n\
    2. Write a new message.\n\
    3. Key in \"exit\" to quit\n\
    Please type \"1\" , \"2\" or \"3\" to select an option.\n";

    char all_message[8192];
    all_message[0] = '\0';

    // Send menu to client
	send(clientSocket, menu, strlen(menu), 0);

    char message_num[20] = {'\0'};
    message_num[0] = 'a';

    while(1){
        recv(clientSocket, rec_buf, sizeof(rec_buf), 0);

        if(!strncmp(rec_buf, "1", 1)){ //if choose 1 in menu -> show all message.
			send_buf[0] = '\0';
			strcat(send_buf, "All messages:\n");
			strcat(send_buf, all_message);
	        strcat(send_buf, menu);
	        send(clientSocket, send_buf, sizeof(send_buf), 0);
            // memset(rec_buf, '\0', sizeof(rec_buf));
            // memset(send_buf, '\0', sizeof(send_buf));
		}
        else if(!strncmp(rec_buf, "2", 1)){
            send_buf[0] = '\0';
            strcat(send_buf, "Type a new message:\n");
            send(clientSocket, send_buf, sizeof(send_buf), 0);
            // memset(rec_buf, '\0', sizeof(rec_buf));
            // memset(send_buf, '\0', sizeof(send_buf));

            recv(clientSocket, rec_buf, sizeof(rec_buf), 0);

            strcat(all_message, message_num);
            strcat(all_message, ". ");
            strncat(all_message, rec_buf, sizeof(rec_buf));
            strcat(all_message, "\n");
            // memset(rec_buf, '\0', sizeof(rec_buf));

            send(clientSocket, menu, sizeof(menu), 0);
            message_num[0] = (char)(message_num[0]+1);
        }
        else if(!strncmp(rec_buf, "exit", 4)){
            break;
        }
        else{
            send(clientSocket, menu, sizeof(menu), 0);
        }
    }
    closesocket(clientSocket);
    WSACleanup();
	return 0;
}