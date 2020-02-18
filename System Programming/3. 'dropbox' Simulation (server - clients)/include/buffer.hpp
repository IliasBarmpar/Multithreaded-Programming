#ifndef BUFFER_HPP_
#define BUFFER_HPP_

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

struct bufferNode{
	char pathname[128];
	int version;
	uint32_t ipAdd;
	uint16_t port;
};

struct bufferCirc{
	void *buffer;
	void *endOfBuffer;
	void *head;
	void *tail;
	int bufferSize;
	int sum;
};

void printBuffer(const bufferCirc *buff);

void initializeBuffer(bufferCirc *buff, int buffSize);

void bufferAdd(bufferCirc *buff, uint32_t value1, uint16_t value2);

void bufferAdd(bufferCirc *buff, bufferNode *element);

void getBufferObject(bufferCirc *buff, bufferNode *element);


#endif /* BUFFER_HPP_ */
