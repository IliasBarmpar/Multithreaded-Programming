#include "list2.hpp"

struct listNode2 *head = NULL;
struct listNode2 *curr = NULL;

void printList2(){
	listNode2 *ptr = head;
	while(ptr!=NULL){
		printf("");
		ptr = ptr->next;
	}
}

int addNode2(uint32_t val1, uint16_t val2){
	listNode2 *link = (listNode2 *) malloc(sizeof(listNode2));
	link->ipAddress = val1;
	link->portNum = val2;
	link->next = head;

	head = link;

	return 1;
}

int removeNode2(uint32_t val1, uint16_t val2){
	listNode2 *temp = head, *prev;

	if(temp!=NULL){
		if(val1==temp->ipAddress && val2==temp->portNum){
			head = temp->next;
			free(temp);
			return 1;
		}
	}

	while(temp!=NULL && (temp->ipAddress!=val1 || temp->portNum!=val2)){
		prev = temp;
		temp = temp->next;
	}

	if(temp == NULL)
		return 0;

	prev->next = temp->next;
	free(temp);
	return 1;
}

int foundOnList2(uint32_t val1, uint16_t val2){
	listNode2 *temp = head;
	while(temp!=NULL){
		if(temp->ipAddress==val1 && temp->portNum==val2){
			return 1;
		}
		temp = temp->next;
	}
	return 0;
}

void printList2(uint32_t val1, uint16_t val2){
	listNode2 *temp = head;
	if(temp==NULL){
		printf("list is empty\n");
	}
	while(temp!=NULL){
		printf("<%lu, %u>\n", temp->ipAddress, temp->portNum);
		temp = temp->next;
	}
}

int connectTot2(int *sock, uint32_t val1, uint16_t val2){
	if ((*sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("connectTot>sock: ");
		return 0;
	}

	struct sockaddr_in server;
	struct sockaddr *serverptr = (struct sockaddr*)&server;
	server.sin_family = AF_INET;       /* Internet domain */
	memcpy(&server.sin_addr, &val1, sizeof(uint32_t));
	server.sin_port = val2;         /* Server port */
	if (connect(*sock, serverptr, sizeof(server)) < 0){
		perror("connectTot>connect: ");
		return 0;
	}

	return 1;
}

void writeUserOn2(uint32_t val1, uint16_t val2){
	while(head!=NULL){
		if(!(head->ipAddress==val1 && head->portNum==val2)){
			int sock;
			if(connectTot2(&sock, head->ipAddress, head->portNum)){
				printf("Sent User On to <%lu, %u>\n", head->ipAddress, head->portNum);
				char buf[256];
				snprintf(buf, sizeof(buf), "USER_ON <%lu, %u>", head->ipAddress, head->portNum);
				write(sock, buf, sizeof buf);
			}else{
				printf("Couldn't connect and send USER_ON <%lu, %u>\n", head->ipAddress, head->portNum);
			}
		}
		head = head->next;
	}
}



