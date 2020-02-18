#ifndef THREADFUNCTIONS_HPP_
#define THREADFUNCTIONS_HPP_
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

//#include "list.hpp"
#include "list2.hpp"
#include "buffer.hpp"
#include "basicFunctions.hpp"
#define PERMS 0777

extern pthread_mutex_t mtx;
extern pthread_mutex_t mtxLL;
extern pthread_mutex_t mtxStrtok;
extern pthread_cond_t nonempty;
extern pthread_cond_t nonfull;
extern bufferCirc circularBuffer;
extern listNode *clientList;
extern uint32_t sIP;
extern uint16_t sP;
extern uint32_t cIP;
extern uint32_t cP;

struct argStruct{
	int arg1;
	int arg2;
};

struct fixedString{
	char pathname[128];
	int version;
};

int connectTo(int *sock, uint32_t val1, uint16_t val2);

void sighan2(int signum);

void readFileFromSocket(int fp, int sock);

void createFolders(char *prev, int *fp);

void fillArray(int num, fixedString **stringArray);

void bufferNodeFuntions(bufferNode *node, int bufferSize, int id);

void *threadFunction(void *argp);

#endif /* THREADFUNCTIONS_HPP_ */
