#include "basicFunctions.hpp"

void sighan(int signum){
	exit(1);
}

void perror_exit(char *message){
    perror(message);
    exit(EXIT_FAILURE);
}

void perror_exit(char *message, int err){
	fprintf(stderr, "%s: %s\n", message, strerror(err));
	exit(EXIT_FAILURE);
}

int countDigits(int  a){
	int count1 = 0;
	while(a != 0){
		a /= 10;
		++count1;
	}

	return count1;
}

int countDigits(listNode *temp){
	uint32_t a = temp->ipAddress;
	uint16_t b = temp->portNum;
	int count1 = 0;
	while(a != 0){
		a /= 10;
		++count1;
	}
	int count2 = 0;
	while(b != 0){
		b /= 10;
		++count2;
	}
	return count1 + count2;
}

int countTotal(listNode *head, int*clientSum){
	int count = 0;
	listNode *temp = head;
	while(temp!=NULL){
		(*clientSum)++;
		count += countDigits(temp) + 5;
		temp = temp->next;
	}
	return count;
}


