#ifndef ml_h
#define ml_h

#include <string.h>
#include "shm.h"

// Part List //
typedef struct mylist{
	Component data;
	struct mylist *next;
}myList, *myListPtr;

myListPtr createNode(Component );

// FI..
myListPtr addNode(myListPtr , Component );
// ..FO
myListPtr removeNode(myListPtr , int *, double *, double *, double *);
void printList(myListPtr );


// Product List //
typedef struct myPrlist{
	char id[12];
	double time;
	struct myPrlist *next;
}myProductList, *myProductListPtr;

myProductListPtr createPrNode(char *, double );
myProductListPtr addPrNode(myProductListPtr , char *, double );
void printPrProductMean(myProductListPtr );
myProductListPtr removePrNode(myProductListPtr );
void printPrList(myProductListPtr );
void deletePrList(myProductListPtr );

#endif

