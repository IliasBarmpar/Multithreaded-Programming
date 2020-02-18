#include "threadFunctions.hpp"

pthread_mutex_t mtx;
pthread_mutex_t mtxLL;
pthread_mutex_t mtxStrtok;
pthread_cond_t nonempty;
pthread_cond_t nonfull;
bufferCirc circularBuffer;
listNode *clientList = NULL;
uint32_t sIP;
uint16_t sP;
uint32_t cIP;
uint32_t cP;

void sighan2(int signum){
	int sock;
	if(connectTo(&sock, sIP, sP)){
		//Get File List
		char buf[256];
		snprintf(buf, sizeof(buf), "LOG_OFF <%lu, %u>", cIP, cP);
		if(write(sock, buf, 256) == -1){ perror("sighan2: "); return ;}

	}else{
		printf("Fail\n");
	}
	exit(1);
}

int connectTo(int *sock, uint32_t val1, uint16_t val2){
	if ((*sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) perror_exit("socket");

	struct sockaddr_in server;
	struct sockaddr *serverptr = (struct sockaddr*)&server;
	server.sin_family = AF_INET;       /* Internet domain */
	memcpy(&server.sin_addr, &val1, sizeof(uint32_t));
	server.sin_port = val2;         /* Server port */
	if (connect(*sock, serverptr, sizeof(server)) < 0) perror_exit("connect");

	return 1;
}

void readFileFromSocket(int fp, int sock){
	int n, count = 0;
	char buff[256] = {0};
	while((n = read(sock, buff, 256)) > 0){
		write(fp, buff, n);
		count += n;
	}
	if(n < 0) printf("Read Error \n");
//	printf("File Ok Completed %d\n", count);
}

void createFolders(char *prev, int *fp){
	char *chunk = strtok(NULL, "/");
	if(chunk!=NULL){
		mkdir(prev, 0777);
		char prevNext[1024];
		snprintf(prevNext, sizeof(prevNext), "%s/%s", prev, chunk);
		createFolders(prevNext, fp);
	}else{
		if(((*fp) = open(prev, O_CREAT | O_EXCL | O_RDWR, PERMS)) == -1){
			perror("Error while opening file.");
			printf("%s\n", prev);
			return ;
		}
		return ;
	}
}


void fillArray(int num, fixedString **stringArray){
	char *chunks;
	*stringArray = (fixedString *)(malloc(sizeof(fixedString)*num));

	for(int i = 0; i < num; i++){
		chunks = strtok(NULL, ","); if(chunks==NULL) { perror("4."); }
		strcpy( ((*stringArray)[i]).pathname, chunks+2);
		chunks = strtok(NULL, ">"); if(chunks==NULL) { perror("5."); }
		((*stringArray)[i]).version = strtol(chunks, NULL, 10);
	}
}

void bufferNodeFuntions(bufferNode *node, int bufferSize, int id){
//	printf(">(%ld)|(%lu, %u, %s, %d)\n", pthread_self()%1000,node->ipAdd, node->port, node->pathname, node->version);
	if(node->version==-1){
		//2slot Element
		pthread_mutex_lock(&mtxLL);
//		if(foundOnList2(node->ipAdd, node->port)){
		if(1){
			pthread_mutex_unlock(&mtxLL);
			int sock;
			connectTo(&sock, node->ipAdd, node->port);

			//Get File List
			char buf[256];
			snprintf(buf, sizeof(buf), "GET_FILE_LIST");
//			printf("(%ld)|w1: %s\n", pthread_self()%1000, buf);
			if(write(sock, buf, 256) == -1) return ;

			char buff[256];
			if (read(sock, buff, 256) < 0) return ;
//			printf("(%ld)|r1: %s\n", pthread_self()%1000, buff);

			//Write on Buffer
			fixedString *stringArray;

			pthread_mutex_lock(&mtxStrtok);
			char *chunks = strtok(buff, " \n\t"); if(chunks==NULL) { perror("2."); pthread_mutex_unlock(&mtxStrtok); return ;}
			if(!strcmp(chunks, "FILE_LIST")){
				chunks = strtok(NULL, " \n\t"); if(chunks==NULL) { perror("3."); pthread_mutex_unlock(&mtxStrtok); return ;}
				int num = strtol(chunks, NULL, 10);
				if(num<=0) perror_exit("NUM");

				fillArray(num, &stringArray);
				pthread_mutex_unlock(&mtxStrtok);

				bufferNode element;
				element.ipAdd	= node->ipAdd;
				element.port	= node->port;

				for(int i = 0; i < num; i++){
//					printf("(%ld)|running for %d\n", pthread_self()%1000, i);
					pthread_mutex_lock(&mtx);
						strcpy(element.pathname, stringArray[i].pathname);

						element.version = stringArray[i].version;

						if(circularBuffer.sum>=bufferSize)
							pthread_cond_wait(&nonfull, &mtx);
						bufferAdd(&circularBuffer, &element);
					pthread_mutex_unlock(&mtx);
					pthread_cond_signal(&nonempty);
					sleep(1);
				}
			}else{
				pthread_mutex_unlock(&mtxStrtok);
			}

			//Disconnect
			close(sock);
		}else{
			pthread_mutex_unlock(&mtxLL);
		}
	}else{
		pthread_mutex_lock(&mtxLL);
//		if(foundOnList2(node->ipAdd, node->port)){
		if(1){
			pthread_mutex_unlock(&mtxLL);
			int sock;
			connectTo(&sock, node->ipAdd, node->port);
			//Get File
			char buf[256];
			snprintf(buf, sizeof(buf), "GET_FILE <%s, %d>", node->pathname, node->version);
//			printf("(%ld)|w2: %s\n", pthread_self()%1000, buf);
			if(write(sock, buf, 256) == -1) return ;

			char buff[256];
			if (read(sock, buff, 256) < 0) return ;
//			printf("(%ld)|r2: %s\n", pthread_self()%1000, buff);

			int opt = -1;
			int a, b;
			pthread_mutex_lock(&mtxStrtok);
			char *chunks = strtok(buff, " \n\t"); if(chunks==NULL) { perror("5."); pthread_mutex_unlock(&mtxStrtok); return ; }
			if(!strcmp(chunks, "FILE_NOT_FOUND")){
				opt = 1;
			}else if(!strcmp(chunks, "FILE_UP_TO_DATE")){
				opt = 2;
			}else if(!strcmp(chunks, "FILE_SIZE")){
				opt = 3;

				chunks = strtok(NULL, " \n\t"); if(chunks==NULL) { perror("6."); pthread_mutex_unlock(&mtxStrtok); return ; }
				a = strtol(chunks, NULL, 10);

				chunks = strtok(NULL, " \n\t"); if(chunks==NULL) { perror("7."); pthread_mutex_unlock(&mtxStrtok); return ; }
				b = strtol(chunks, NULL, 10);
			}
			pthread_mutex_unlock(&mtxStrtok);

			if(opt == 1){
				//SKIP doing anything
			}else if(opt == 2){
				//SKIP doing anything
			}else if(opt == 3){
				char buf[256];
				struct in_addr t; t.s_addr = node->ipAdd;
				snprintf(buf, sizeof(buf), "%d_%s_%d", id, inet_ntoa(t), ntohs(node->port));
				mkdir(buf, 0777);

				pthread_mutex_lock(&mtxStrtok);
				char *chunks = strtok(node->pathname+1, "/");
				char buf2[256];
				snprintf(buf2, sizeof(buf2), "%s/%s", buf, chunks);
				int fp;
				createFolders(buf2, &fp);
				pthread_mutex_unlock(&mtxStrtok);

//				printf("(%ld)|Let the process start rr.\n", pthread_self()%1000);
				readFileFromSocket(fp, sock);
			}

			//Disconnect
			close(sock);
		}else{
			pthread_mutex_unlock(&mtxLL);
		}
	}
}

void *threadFunction(void *argp){
	struct argStruct *args = (struct argStruct *)argp;
	int bufferSize = args->arg1;
	int id =  args->arg2;

	while(1){
		pthread_mutex_lock(&mtx);
			if(circularBuffer.sum<=0)
				pthread_cond_wait(&nonempty, &mtx);
			bufferNode node; getBufferObject(&circularBuffer, &node);
		pthread_mutex_unlock(&mtx);

		pthread_cond_signal(&nonfull);
		sleep(1);
		bufferNodeFuntions(&node, bufferSize, id);
	}

	pthread_exit((void *) 88);
}



