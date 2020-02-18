#ifndef LIST_HPP_
#define LIST_HPP_

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

struct listNode{
	uint32_t ipAddress;
	uint16_t portNum;
	listNode *next;
};

void printList(listNode *head);

int foundOnList(listNode *head, uint32_t val1, uint16_t val2);

int addNode(listNode **head, uint32_t ipAdd, uint16_t port);

int removeNode(listNode **head, uint32_t ipAdd, uint16_t port);

int connectTot(int *sock, uint32_t val1, uint16_t val2);

void writeUserOn(const listNode *head, uint32_t val1, uint16_t val2);

void writeUserOff(const listNode *head, uint32_t val1, uint16_t val2);

#endif /* LIST_HPP_ */
