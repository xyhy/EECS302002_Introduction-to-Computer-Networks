#include <stdio.h>
#include <stdlib.h>
#include <string.h>	// for bzero()
#include <unistd.h>	// for close()
/*
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
*/
#include <errno.h>
#include <winsock2.h>

int main(int argc, char *argv[])
{
	WSADATA wsaData;
	WSAStartup( MAKEWORD(2, 1),  &wsaData);
	struct sockaddr_in serverAddress, clientAddress;
	int server_addr_length = sizeof(serverAddress);
        int client_addr_length = sizeof(clientAddress);
    	int serverSocket, clientSocket;
	int ServerPortNumber;

	if(argc == 2){
		ServerPortNumber = atoi(argv[1]);
	}

   	serverSocket = socket(PF_INET, SOCK_STREAM, 0);
	if(serverSocket < 0){
		fprintf(stderr, "Error creating socket : %s\n", strerror(errno));
		exit(0);
	}

 	//bzero(&serverAddress, server_addr_length);
	serverAddress.sin_family = AF_INET;
  	serverAddress.sin_port = htons(ServerPortNumber);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	if(bind(serverSocket,(struct sockaddr *) &serverAddress, server_addr_length) == -1){
		fprintf(stderr, "Error binding : %s\n", strerror(errno));
		close(serverSocket);
		exit(0);
	}

	if(listen(serverSocket, 3) == -1){
		fprintf(stderr, "Error listening : %s\n", strerror(errno));
		close(serverSocket);
		exit(0);
	}

	printf("Waiting...\n");
	if((clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &client_addr_length)) == -1){
		printf("accept failed\n");
		close(serverSocket);
		exit(0);
	}
	printf("Client connect successfully\n");

	int i;
	int bytesRecv, bytesSend;
	int itemID, itemNumber;
	int itemcount[3] = {0};
        char send_buf[500];
	char recv_buf[500];
	char str[100];
        char *menu = "\
\n\n------------------Menu------------------\n\
1. Show items\n\
2. Add new item\n\
3. Summarize result(item name and price)\n\
Please enter your operation : \0";

	char *itemlist = "\
\n\n----------------ItemList----------------\n\
1. Apple    50   dollars\n\
2. Banana   30   dollars\n\
3. Cat	    100  dollars\0";

	char *whichitem = "\
\n\n----------------------------------------\n\
Which item do you want?\n\
1. Apple\n\
2. Banana\n\
3. Cat\n\
Please enter your operation : \0";

	char *howmany = "\
\n\n----------------------------------------\n\
Quantity?\0";

	// Send menu to client
	send_buf[0] = '\0';
	strcat(send_buf, menu);
	bytesSend = send(clientSocket, send_buf, sizeof(send_buf), 0);
	if(bytesSend < 0) printf("Error sending packet\n");


  	while(1){
		bytesRecv = recv(clientSocket, recv_buf, sizeof(recv_buf), 0);
		if(bytesRecv < 0) printf("Error receiving packet\n");

		printf("%s\n", recv_buf);

		if(!strncmp(recv_buf, "1", 1)){
			send_buf[0] = '\0';
			strcat(send_buf, itemlist);
			strcat(send_buf, menu);
			bytesSend = send(clientSocket, send_buf, sizeof(send_buf), 0);
			if(bytesSend < 0) printf("Error sending packet\n");
		}
		else if(!strncmp(recv_buf, "2", 1)){
			send_buf[0] = '\0';
			strcat(send_buf, whichitem);
			bytesSend = send(clientSocket, send_buf, sizeof(send_buf), 0);
			if(bytesSend < 0) printf("Error sending packet\n");

			bytesRecv = recv(clientSocket, recv_buf, sizeof(recv_buf), 0);
			if(bytesRecv < 0) printf("Error receiving packet\n");
			itemID = atoi(recv_buf);
			printf("%d\n", itemID);

			send_buf[0] = '\0';
			strcat(send_buf, howmany);
			bytesSend = send(clientSocket, send_buf, sizeof(send_buf), 0);
			if(bytesSend < 0) printf("Error sending packet\n");

			bytesRecv = recv(clientSocket, recv_buf, sizeof(recv_buf), 0);
			if(bytesRecv < 0) printf("Error receiving packet\n");
			itemNumber = atoi(recv_buf);
			printf("%d\n", itemNumber);

			itemcount[itemID-1] += itemNumber;

			send_buf[0] = '\0';
			strcat(send_buf, menu);
			bytesSend = send(clientSocket, send_buf, sizeof(send_buf), 0);
			if(bytesSend < 0) printf("Error sending packet\n");
		}
		else if(!strncmp(recv_buf, "3", 1)){
			send_buf[0] = '\0';
			strcat(send_buf, "\n\n----------------------------------------\n\0");
			sprintf(str, "Apple * %d\n", itemcount[0]);
			strcat(send_buf, str);
			sprintf(str, "Banana * %d\n", itemcount[1]);
			strcat(send_buf, str);
			sprintf(str, "Cat * %d\n", itemcount[2]);
			strcat(send_buf, str);
			sprintf(str, "Total price = %d\n", 50*itemcount[0]+30*itemcount[1]+100*itemcount[2]);
			strcat(send_buf, str);
			strcat(send_buf, menu);
			bytesSend = send(clientSocket, send_buf, sizeof(send_buf), 0);
			if(bytesSend < 0) printf("Error sending packet\n");
		}
		else{
			bytesSend = send(clientSocket, menu, strlen(menu), 0);
			if(bytesSend < 0) printf("Error sending packet\n");
		}


	}
	WSACleanup();
	return 0;
}



