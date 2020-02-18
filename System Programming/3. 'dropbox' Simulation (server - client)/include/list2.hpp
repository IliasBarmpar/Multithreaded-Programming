#ifndef LIST2_HPP_
#define LIST2_HPP_

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

struct listNode2{
	uint32_t ipAddress;
	uint16_t portNum;
	listNode2 *next;
};

extern struct listNode2 *head;
extern struct listNode2 *curr;
void printList2();

int addNode2(uint32_t val1, uint16_t val2);

int removeNode2(uint32_t val1, uint16_t val2);

int foundOnList2(uint32_t val1, uint16_t val2);

void printList2(uint32_t val1, uint16_t val2);

int connectTot2(int *sock, uint32_t val1, uint16_t val2);

void writeUserOn2(uint32_t val1, uint16_t val2);

#endif /* LIST2_HPP_ */
