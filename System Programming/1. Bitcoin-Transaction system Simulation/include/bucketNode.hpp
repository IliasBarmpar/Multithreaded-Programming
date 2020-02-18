#ifndef bucketnode_hpp
#define bucketnode_hpp

#include "transaction.hpp"

//The print functions interact with printEarnings and printPayments functions.
struct myDynamicListNode{
	transaction transact;
	myDynamicListNode *next;

	void print();

	void print(timeStr *, timeStr *);

	void print(dateStr *, dateStr *);

	void print(timeStr *, timeStr *, dateStr *, dateStr *);

	void printTotalEarnings();

	void printTotalPayments();
};

struct bucketNode{
	userID 				*usID;
	myDynamicListNode 	*head;
};

#endif /* BUCKETNODE_HPP_ */
