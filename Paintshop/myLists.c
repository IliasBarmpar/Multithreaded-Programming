#include "myLists.h"

// Part List //
myListPtr createNode(Component value){
	myListPtr temp;
	temp = (myList *)malloc(sizeof(struct mylist));
	temp->data = value;
	temp->next = NULL;
	return temp;
}

// FI..
myListPtr addNode(myListPtr head, Component value){ //FI..
	if(head == NULL){
		head = createNode(value);
	}else{
		myListPtr temp = head;
		while(temp->next != NULL)
			temp = temp->next;
		temp->next = createNode(value);
	}
	return head;
}

// ..FO
myListPtr removeNode(myListPtr head, int *id, double *tssec, double *tsnsec, double *wps){ //..FO
	if(head == NULL){
		return NULL;
	}else{
		myListPtr temp = head;
		head 	= head->next;
		*id  	= (temp->data).id;
		*tssec  = (temp->data).timestampsec;
		*tsnsec = (temp->data).timestampnsec;
		*wps 	= (temp->data).waitForPs;
		free(temp);
	}
	return head;
}

void printList(myListPtr head){
	if(head == NULL) printf("List is empty\n");
	myListPtr temp;
	temp = head;
	while(temp!= NULL){
		printf("%d - %c - %d\n", (temp->data).id, (temp->data).type, (temp->data).state);
		temp = temp->next;
	}
	printf("_______\n");
}


// Product List //
myProductListPtr createPrNode(char *value, double value2){
	myProductListPtr temp;
	temp = (myProductListPtr)malloc(sizeof(struct myPrlist));
	temp->time = value2;
	int i = 0;
	for(; i < 12; i++)
		temp->id[i] = value[i];
	temp->next = NULL;
	return temp;
}

myProductListPtr addPrNode(myProductListPtr head, char *value, double value2){
	if(head == NULL){
		head = createPrNode(value, value2);
	}else{
		myProductListPtr temp = head;
		while(temp->next != NULL)
			temp = temp->next;
		temp->next = createPrNode(value, value2);
	}
	return head;
}


void printPrProductMean(myProductListPtr head){
	long double tempSum = 0.0;
	if(head == NULL) printf("List is empty\n");
	myProductListPtr temp;
	temp = head;
	int i = 0;
	while(temp!= NULL){
		tempSum = tempSum + temp->time;
		i++;
		temp = temp->next;
	}
	tempSum = tempSum/i;
	printf("Average time for a product to be made: %0.1Lfms\n", tempSum);
}

myProductListPtr removePrNode(myProductListPtr head){
	if(head == NULL){
		return NULL;
	}else{
		myProductListPtr temp = head;
		head = head->next;
		free(temp);
	}
	return head;
}

void printPrList(myProductListPtr head){
	if(head == NULL) printf("List is empty\n");
	myProductListPtr temp;
	temp = head;
	while(temp!= NULL){
		printf("%s | %0.1lf \n", temp->id, temp->time);
		temp = temp->next;
	}
	printf("_______\n");
}

void deletePrList(myProductListPtr head){
	if(head == NULL) printf("List is empty\n");
	myProductListPtr temp;
	temp = head;
	while(temp!= NULL){
		temp = removePrNode(temp);
	}
}
