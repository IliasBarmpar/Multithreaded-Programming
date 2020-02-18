#include "list.hpp"

void printList(listNode *head){
	listNode *temp = head;
	if(temp==NULL){
		printf("list is empty!\n");
	}
	while(temp!=NULL){
		printf("<%lu, %u>\n", temp->ipAddress, temp->portNum);
		temp = temp->next;
	}
}

int foundOnList(listNode *head, uint32_t val1, uint16_t val2){
	listNode *temp = head;
	while(temp!=NULL){
		if(temp->ipAddress==val1 && temp->portNum==val2){
			return 1;
		}
		temp = temp->next;
	}
	return 0;
}

int addNode(listNode **head, uint32_t ipAdd, uint16_t port){
	struct listNode *newNode = (struct listNode *)malloc(sizeof(struct listNode));
	newNode->ipAddress = ipAdd;
	newNode->portNum = port;
	newNode->next = NULL;

	if(*head==NULL){
		*head = newNode;
	}else{
		struct listNode *temp = *head;
		while(temp->next!=NULL){
			if(temp->ipAddress==ipAdd && temp->portNum==port)
				return 0;
			temp = temp->next;
		}
		temp->next = newNode;
	}
	return 1;
}

int removeNode(listNode **head, uint32_t ipAdd, uint16_t port){
	listNode *temp = *head, *prev;

	if(temp!=NULL){
		if(ipAdd==temp->ipAddress && port==temp->portNum){
			*head = temp->next;
			free(temp);
			return 1;
		}
	}

	while(temp!=NULL && (temp->ipAddress!=ipAdd || temp->portNum!=port)){
		prev = temp;
		temp = temp->next;
	}

	if(temp == NULL)
		return 0;

	prev->next = temp->next;
	free(temp);
	return 1;
}

int connectTot(int *sock, uint32_t val1, uint16_t val2){
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

void writeUserOn(const listNode *head, uint32_t val1, uint16_t val2){
	while(head!=NULL){
		if(!(head->ipAddress==val1 && head->portNum==val2)){
			int sock;
			if(connectTot(&sock, head->ipAddress, head->portNum)){
//				printf("Sent User On <%lu, %u>\n", val1, val2);
				char buf[256];
				snprintf(buf, sizeof(buf), "USER_ON <%lu, %u>", val1, val2);
				write(sock, buf, sizeof buf);
			}else{
				printf("Couldn't connect and send USER_ON <%lu, %u>\n", val1, val2);
			}
		}
		head = head->next;
	}
}

void writeUserOff(const listNode *head, uint32_t val1, uint16_t val2){
	while(head!=NULL){
		if(!(head->ipAddress==val1 && head->portNum==val2)){
			int sock;
			if(connectTot(&sock, head->ipAddress, head->portNum)){
//				printf("Sent User Off <%lu, %u>\n", val1, val2);
				char buf[256];
				snprintf(buf, sizeof(buf), "USER_OFF <%lu, %u>", val1, val2);
				write(sock, buf, sizeof buf);
			}else{
				printf("Couldn't connect and send USER_OFF <%lu, %u>\n", val1, val2);
			}
		}
		head = head->next;
	}
}



