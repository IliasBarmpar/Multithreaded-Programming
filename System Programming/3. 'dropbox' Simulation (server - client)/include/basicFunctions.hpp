#ifndef BASICFUNCTIONS_HPP_
#define BASICFUNCTIONS_HPP_

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
#include "list.hpp"

void sighan(int signum);

void perror_exit(char *message);

void perror_exit(char *message, int err);

int countDigits(int  a);

int countDigits(listNode *temp);

int countTotal(listNode *head, int*clientSum);

#endif /* BASICFUNCTIONS_HPP_ */
