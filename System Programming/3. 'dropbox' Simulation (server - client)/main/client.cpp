#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <poll.h>
#include <dirent.h>
#include <pthread.h>

#include "readCmd.hpp"
#include "list2.hpp"
#include "threadFunctions.hpp"
#include "buffer.hpp"
#include "basicFunctions.hpp"

#define PERMS 0777

//pthread_mutex_t mtx;
//pthread_cond_t nonempty;
//pthread_cond_t nonfull;
//bufferCirc circularBuffer;
//listNode *clientList = NULL;

void connectToServerLogOnAndGetClientList(char * serverIP, int serverPort, int portNum, int bufferSize, struct sockaddr_in my_addr1){
	int sock, enable = 1;
	/* Create Socket */  /* Reset */
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) perror_exit("socket");
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) perror("setsockopt");


	/* Find server address */
	struct in_addr myaddress;
	inet_aton(serverIP, &myaddress);
	struct hostent *rem;
	if ((rem = gethostbyaddr((const char*)&myaddress, sizeof(myaddress), AF_INET)) == NULL) { herror("gethostbyname"); exit(1); }


	/* Bind */
	if (bind(sock, (struct sockaddr*) &my_addr1, sizeof(my_addr1)) < 0) { perror("bind"); exit(-1); };


	/* Connect */
	struct sockaddr_in server;
	struct sockaddr *serverptr = (struct sockaddr*)&server;
	server.sin_family = AF_INET;       /* Internet domain */
	memcpy(&server.sin_addr, rem->h_addr, rem->h_length);
	server.sin_port = htons(serverPort);         /* Server port */
	if (connect(sock, serverptr, sizeof(server)) < 0) perror_exit("connect");


	/* Write LOG_ON to server */
	char hostbuffer[256];
	char *IPbuffer;
	struct hostent *host_entry;
	int hostname;
	hostname 	= gethostname(hostbuffer, sizeof(hostbuffer));
	host_entry 	= gethostbyname(hostbuffer);
	IPbuffer 	= inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0]));
	uint32_t ipBin = inet_addr(IPbuffer);
	in_port_t portBin = htons(portNum);
	sIP = server.sin_addr.s_addr;
	sP  = server.sin_port;
	cIP = ipBin;
	cP  = portBin;

	char buf1[256];
	snprintf(buf1, sizeof(buf1), "LOG_ON <%lu, %u>", ipBin, portBin);
	if(write(sock, buf1, 256) == -1) return ;


	/* Read CLIENT_LIST */
	char clientListStr[256];
	if (read(sock, clientListStr, 256) < 0) return ;
	close(sock);

	/* Create Client List & Circular Buffer */
	pthread_mutex_lock(&mtx);
	initializeBuffer(&circularBuffer, bufferSize);
	pthread_mutex_unlock(&mtx);

	pthread_mutex_lock(&mtxStrtok);
	char *chunks;
	chunks = strtok(clientListStr, " \n\t");
	if(!strcmp(chunks, "CLIENT_LIST")){
		chunks = strtok(NULL, " \n\t");
		int num = strtol(chunks, NULL, 10);
		if(num<=0) perror_exit("NUM");

		for(int i = 0; i < num; i++){
			chunks = strtok(NULL, " ,");
			uint32_t a = strtoul(chunks+1, NULL, 10);

			chunks = strtok(NULL, " >");
			uint16_t b = strtoul(chunks, NULL, 10);

			pthread_mutex_lock(&mtxLL);
			addNode2(a, b);
			pthread_mutex_unlock(&mtxLL);

			pthread_mutex_lock(&mtx);
			if(circularBuffer.sum>=circularBuffer.bufferSize)
				pthread_cond_wait(&nonfull, &mtx);
			bufferAdd(&circularBuffer, a, b);
			pthread_mutex_unlock(&mtx);
			pthread_cond_signal(&nonempty);
		}
		chunks = strtok(NULL, " \n\t");
		cout << chunks << endl;
	}else if(!strcmp(chunks, "EMPTY")){
		//SKIP adding anything
	}else{
		 perror("Received incorrect message from server.");
	}
	pthread_mutex_unlock(&mtxStrtok);
}

void createFileList(char *name, char *prpath, char *ongoing, int *count){
	DIR *dir;
	struct dirent *direntInDir;
	if(!(dir = opendir(name))) return ;

	while( (direntInDir = readdir(dir)) != NULL ){
		char fullPath[1024];
		snprintf(fullPath, sizeof(fullPath), "%s/%s", name, direntInDir->d_name);

		char pathForReceiver[1024];
		snprintf(pathForReceiver, sizeof(pathForReceiver), "%s/%s", prpath, direntInDir->d_name);

		if(direntInDir->d_type != DT_DIR){
			char temp[1024]; strcpy(temp, ongoing);
			snprintf(ongoing, 1024, "%s <%s, %d>", temp, pathForReceiver, 0);
			(*count)++;
		}else{
			if(strcmp(direntInDir->d_name, ".") == 0 || strcmp(direntInDir->d_name, "..") == 0) continue;

			createFileList(fullPath, pathForReceiver, ongoing,count);
		}
	}
}

void writeFileFunction(FILE *fp, int sock){
	while(1){
		char buff[256] = {0};
		int nread = fread(buff, 1, 256, fp);
		if(nread > 0)
			write(sock, buff, nread);
		if(nread < 256){
			if(feof(fp)){
//				printf("(%ld)|End of file\n", pthread_self()%1000);
			}
			if(ferror(fp))
				printf("(%ld)|Error reading", pthread_self()%1000);
			break;
		}
	}
}

void incomingRequests(char *message, char *dirName, listNode *nodeHead, int sock){
	int opt = -1, version;
	char pathName[128];
	uint32_t value1;
	uint16_t value2;

	pthread_mutex_lock(&mtxStrtok);
	char *chunks = strtok(message, " \n\t");
	if(!strcmp(chunks, "GET_FILE_LIST")){
		opt = 1;
	}else if(!strcmp(chunks, "GET_FILE")){
			opt = 2;

		chunks = strtok(NULL, ",");
			if(chunks==NULL){ pthread_mutex_unlock(&mtxStrtok); return ; }
		strcpy(pathName, chunks+2);

		chunks = strtok(NULL, ">");
		version = strtol(chunks, NULL, 10);
	}else if(!strcmp(chunks, "USER_ON")){
		opt = 3;

		chunks = strtok(NULL, ","); //IP
		value1 = strtoul(chunks+1, NULL, 10);

		chunks = strtok(NULL, ">"); //Port
		value2 = strtoul(chunks+1, NULL, 10);
	}else if(!strcmp(chunks, "USER_OFF")){
		opt = 4;
		chunks = strtok(NULL, ","); //IP
		value1 = strtoul(chunks+1, NULL, 10);

		chunks = strtok(NULL, ">"); //Port
		value2 = strtoul(chunks+1, NULL, 10);
	}
	pthread_mutex_unlock(&mtxStrtok);



	if(opt==1){ 			//GET_FILE_LIST
		int count = 0;
		char tuples[256] = "";
		createFileList(dirName, ".", tuples, &count);

		char fileList[256];
		if(count!=0){
			snprintf(fileList, 256, "FILE_LIST %d %s", count, tuples);
		}else{
			snprintf(fileList, 256, "EMPTY");
		}
//		printf("(%ld)|w1: %s\n", pthread_self()%1000 , fileList);
		write(sock, fileList, 256);
	}else if(opt==2){		//GET_FILE
		FILE *fp = NULL;
		char buff[256];
		snprintf(buff, 256, "%s%s", dirName, pathName);
		fp = fopen(buff, "r");
		if(fp==NULL){
			char buf[256];
			snprintf(buf, sizeof(buf), "FILE_NOT_FOUND");
			write(sock, buf, 256);
		}else{
			struct stat statbuf;
			if( stat(buff, &statbuf) == -1){ perror("Failed to get file status: "); exit(2); }
			char buf[256];
			snprintf(buf, sizeof(buf), "FILE_SIZE %d %d", version, statbuf.st_size);
//			printf("(%ld)|w2: %s\n", pthread_self()%1000 , buf);
			write(sock, buf, 256);

//			printf("(%ld)|Let the process start ww.\n", pthread_self()%1000);
			writeFileFunction(fp, sock);
			shutdown(sock, SHUT_WR);
		}
	}else if(opt==3){		//USER_ON
		pthread_mutex_lock(&mtxLL);
		addNode2(value1, value2);
		pthread_mutex_unlock(&mtxLL);

		pthread_mutex_lock(&mtx);
		if(circularBuffer.sum>=circularBuffer.bufferSize)
			pthread_cond_wait(&nonfull, &mtx);

		bufferAdd(&circularBuffer, value1, value2);
		pthread_mutex_unlock(&mtx);
		pthread_cond_signal(&nonempty);
	}else if(opt==4){		//USER_OFF
		pthread_mutex_lock(&mtxLL);
//		removeNode2(value1, value2);
		pthread_mutex_unlock(&mtxLL);
	}else{
		printf("Error.\n");
	}
}


int main(int argc, char **argv){
	signal(SIGTSTP, sighan);
	signal(SIGINT, sighan2);

	int port, sock, newsock, recSock, enable = 1;

	char *dirName, *serverIP;
	int workerThreadsSum, bufferSize,portNum, serverPort;
	int err = readCmdCl(argc, argv, &dirName, &portNum, &workerThreadsSum, &bufferSize, &serverPort, &serverIP);

	if(err){
		cout << "Incorrect structure for command line arguments." << endl;
		return -1;
	}else{
		struct sockaddr_in my_addr1;
		my_addr1.sin_family		 = AF_INET;
		my_addr1.sin_addr.s_addr = INADDR_ANY;
		my_addr1.sin_port		 = htons(portNum);

		connectToServerLogOnAndGetClientList(serverIP, serverPort, portNum, bufferSize, my_addr1);

		struct sockaddr_in client;
		struct sockaddr *clientptr = (struct sockaddr *) &client;
		unsigned int clientlen = sizeof client;
		struct pollfd pollSet[10];
		int fds = 0;
		if((recSock = socket(AF_INET, SOCK_STREAM, 0)) < 0) perror_exit("socket");
	    if(setsockopt(recSock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) perror("setsockopt");
		if((recSock = socket(AF_INET, SOCK_STREAM, 0)) < 0) perror_exit("socket");
		if(setsockopt(recSock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) perror("setsockopt");
		if(bind(recSock, (struct sockaddr*) &my_addr1, sizeof(my_addr1)) < 0) { perror("bind"); exit(-1); };
		if(listen(recSock, 10) < 0) perror_exit("Listen");
		pollSet[0].fd 		= recSock;
		pollSet[0].events	= POLLIN;
		fds++;

		pthread_mutex_init(&mtx, NULL);
		pthread_mutex_init(&mtxLL, NULL);
		pthread_mutex_init(&mtxStrtok, NULL);
		pthread_cond_init(&nonempty, 0);
		pthread_cond_init(&nonfull,  0);

		struct argStruct args;
		args.arg1 = bufferSize;
		args.arg2 = portNum;

		// Threads //
		pthread_t *workerThreads;
		if((workerThreads = (pthread_t *)malloc(workerThreadsSum * sizeof(pthread_t))) == NULL) perror_exit("Malloc");
		for(int i = 0; i < workerThreadsSum; i++)
			if(int err = pthread_create(workerThreads+i, NULL, threadFunction, (void *) &args))
				perror_exit("Pthread_create", err);


		//CLIENT ACTING AS SERVER FUNCTION
		while(1){
			int fdIndex, nread = 0;
			poll(pollSet, fds, -1);
			for(fdIndex = 0; fdIndex < fds; fdIndex++){
				if( pollSet[fdIndex].revents & POLLIN ){
					if( pollSet[fdIndex].fd == recSock ){
						//This reacts to connect!
						newsock = accept(recSock, clientptr, &clientlen);

						pollSet[fds].fd 	= newsock;
						pollSet[fds].events = POLLIN;
						fds++;
						printf("Adding client on fd %d\n", newsock);
					}else{
						//This reacts to anything else that comes in the socket
						ioctl(pollSet[fdIndex].fd, FIONREAD, &nread);
						if(nread != 0){
							printf("Message on fd %d %d\n", pollSet[fdIndex].fd, nread );
							char buf[256];
							read(pollSet[fdIndex].fd, buf, 256);
//							printf("(%ld)|rG: %s\n" , pthread_self()%1000, buf);
							incomingRequests(buf, dirName, clientList, pollSet[fdIndex].fd);
						}else{
							close(pollSet[fdIndex].fd);
							pollSet[fdIndex].events = 0;
							for(int i = fdIndex; i < fds; i++)
								pollSet[i] = pollSet[i+1];
							fds--;
						}
					}
				}
			}
		}

		for(int i = 0; i < workerThreadsSum; i++)
			if(int err = pthread_join(*(workerThreads+i), NULL)) perror_exit("Pthread_join", err);
	}
}

