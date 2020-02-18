#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <inttypes.h>
#include <signal.h>
#include <errno.h>
#include "readCmd.hpp"
#include "list.hpp"
#include "basicFunctions.hpp"
#define BUFFSIZE 256
int maxClients = 10;

void createBindAndListenToSocket(int *mainSocket, int portNum){
	int enable = 1;
	/* Create Socket with Reset */
	if ((*mainSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) { perror("socket"); exit(-1); }
	if (setsockopt(*mainSocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) perror("setsockopt");

	/* Bind Socket */
	struct sockaddr_in server;
	struct sockaddr *serverptr = (struct sockaddr *) &server;
	server.sin_family = AF_INET;				/* Internet domain 	*/
	server.sin_addr.s_addr = htonl(INADDR_ANY);	/* Address 			*/
	server.sin_port = htons(portNum);			/* The given port 	*/

	if (bind(*mainSocket, serverptr, sizeof(server)) < 0) { perror("bind"); exit(-1); }

	if (listen(*mainSocket, maxClients) < 0) { perror("listen"); exit(-1); }
}


void recursiveTupleCreation(int messSize, listNode *head, char *prev){
	char curr[messSize]; strcpy(curr, prev);
	snprintf(prev, messSize, "%s <%lu, %u>", curr, head->ipAddress, head->portNum);

	if(head->next!=NULL)
		recursiveTupleCreation(messSize, head->next, prev);
	return ;
}


void createClientList(int newsock, listNode *head, int prev){
	char message[256];
	int clientSum = 0, clDigitSum = 0;
	countTotal(head, &clientSum);

	if(clientSum!=0){
		snprintf(message, sizeof(message), "CLIENT_LIST %d", clientSum);
		recursiveTupleCreation(256, head, message);
		write(newsock, message, sizeof message);
	}else{
		snprintf(message, sizeof(message), "EMPTY", clientSum);
		write(newsock, message, sizeof message);
	}
}


void incomingRequests(char *message, listNode **nodeHead, int newsock, struct sockaddr_in client){
	char *chunks = strtok(message, " \n\t");
	if(!strcmp(chunks, "LOG_ON")){
		chunks = strtok(NULL, ","); if(chunks[0]!='<' || chunks==NULL) { perror("."); return ; }
		uint32_t val1 = strtoul(chunks+1, NULL, 10);

		chunks= strtok(NULL, ">"); if(chunks==NULL) { perror("."); return ; }
		uint16_t val2 = strtoul(chunks+1, NULL, 10);

		chunks= strtok(NULL, " \n\t");
		if(chunks!=NULL) { perror("."); return ; }

		//Add Node
		createClientList(newsock, *nodeHead, 1);
		if(addNode(nodeHead, val1, val2))
			writeUserOn(*nodeHead, val1, val2);

	}else if(!strcmp(chunks, "LOG_OFF")){
		chunks = strtok(NULL, ","); if(chunks[0]!='<' || chunks==NULL) { perror("."); return ; }
		uint32_t val1 = strtoul(chunks+1, NULL, 10);

		chunks= strtok(NULL, ">"); if(chunks==NULL) { perror("."); return ; }
		uint16_t val2 = strtoul(chunks+1, NULL, 10);

		chunks= strtok(NULL, " \n\t");
		if(chunks!=NULL) { perror("."); return ; }

		//Do actions
		printf("User (%lu, %u) sent a LOG_OFF\n", val1, val2);
		if(removeNode(nodeHead, val1, val2))
			writeUserOff(*nodeHead, val1, val2);

	}else{
		if(chunks!=NULL) { perror("Incomming request: "); return ; }
	}
}

int main(int argc, char **argv){
	signal(SIGTSTP, sighan);
	fd_set fds;
	listNode *nodeHead = NULL;
	int mainSocket, newsock, portNum, enable = 1, action, maxSd, tempSd, clientSocket[maxClients], err = readCmdSe(argc, argv, &portNum);

	if(err){
		cout << "Incorrect structure for command line arguments." << endl;
		return -1;
	}else{
		char message[BUFFSIZE];
		struct sockaddr_in client;
		struct sockaddr *clientptr = (struct sockaddr *) &client;
		unsigned int clientlen = sizeof client;
		for(int i = 0; i < maxClients; i++) clientSocket[i] = 0; /* Initialise clientSockets */

		createBindAndListenToSocket(&mainSocket, portNum);

		char hostbuffer[256];
		char *IPbuffer;
		struct hostent *host_entry;
		int hostname;
		hostname 	= gethostname(hostbuffer, sizeof(hostbuffer));
		host_entry 	= gethostbyname(hostbuffer);
		IPbuffer 	= inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0]));
		printf("Hostname: %s\n", hostbuffer);
		printf("Host IP: %s\n", IPbuffer);

		/* Select */
		while(1){
			FD_ZERO(&fds);
			FD_SET(mainSocket, &fds);
			maxSd = mainSocket;

			for(int i = 0; i < maxClients; i++){
				tempSd = clientSocket[i];
				if(tempSd > 0) 		FD_SET(tempSd, &fds);
				if(tempSd > maxSd) 	maxSd = tempSd;
			}

			if((action = select(maxSd + 1, &fds, NULL, NULL, NULL)) < 0 && (errno!=EINTR)) perror_exit("select");
			if(FD_ISSET(mainSocket,&fds)){
				//Accept new Connection
				if ((newsock = accept(mainSocket, clientptr, &clientlen)) < 0) perror_exit("accept");

				//Get Request
				read(newsock, message, BUFFSIZE);
				printf("Incomming message: %s\n", message);
				incomingRequests(message, &nodeHead, newsock, client);

				//Add new socket
				for(int i = 0; i < maxClients; i++){
					if(clientSocket[i] == 0){
						clientSocket[i] = newsock;
						break;
					}
				}
			}

			for(int i = 0; i < maxClients; i++){
				tempSd = clientSocket[i];
				if(FD_ISSET(tempSd,&fds)){
					//Close socket with client
					close(tempSd);
					clientSocket[i] = 0;
				}
			}
		}
		close(mainSocket);
		close(newsock);
	}
}





















































































/*

			// Read message and print it //
			read(newsock, message, BUFFSIZE);
			printf("message b4: %s\n", message);
			chunks = strtok(message, " \n\t");
			while( chunks != NULL ){
				printf("%s\n", chunks);
				if(!strcmp(chunks, "LOG_ON")){
					chunks = strtok(NULL, ",");
					if(chunks[0]!='<' || chunks==NULL) { perror("."); break; }
					uint32_t value = strtoul(chunks+1, NULL, 10);

					chunks= strtok(NULL, ">");
					if(chunks==NULL) { perror("."); break; }
					uint16_t valu2 = strtoul(chunks+1, NULL, 10);

					chunks= strtok(NULL, " \n\t");
					if(chunks!=NULL) { perror("."); break; }

					//Do actions
					addNode(&nodeHead, value, valu2);


					printList(nodeHead);
				}else if(!strcmp(chunks, "GET_CLIENTS")){
					printList(nodeHead);
					chunks= strtok(NULL, " \n\t");
						if(chunks!=NULL) { perror("."); break; }

					//Do actions
					char *buf = ".";
					function(buf, newsock, nodeHead, 1);

				}else if(!strcmp(chunks, "LOG_OFF")){
					chunks= strtok(NULL, " \n\t");
						if(chunks!=NULL) { perror("."); break; }

					//Do actions
					removeNode(&nodeHead, client.sin_addr.s_addr, client.sin_port);
					printList(nodeHead);
				}else{
					if(chunks!=NULL) { perror("."); break; }
				}
			}
			cout << "out" << endl;
*/


//					if(nodeHead==NULL){
//						nodeHead = new listNode;
//						nodeHead->ipAddress = value;
//						nodeHead->portNum 	= valu2;
//						nodeHead->next 		= NULL;
//					}else{
//						listNode *temp = nodeHead;
//						int same = 0;
//						while(temp->next!=NULL){
//							if(temp->ipAddress==value && temp->portNum==valu2){
//								same = 1;
//								break;
//							}
//							temp = nodeHead->next;
//						}
//						if(same)
//							break;
//						if(temp->ipAddress==value && temp->portNum==valu2)
//							break;
//						temp->next = new listNode;
//						temp->next->ipAddress = value;
//						temp->next->portNum = valu2;
//						temp->next->next = NULL;
//					}
