#include "bucketNode.hpp"

void myDynamicListNode::print(){
	transact.print();
	if(next!=NULL)
		next->print();
}

void myDynamicListNode::print(timeStr *time1, timeStr *time2){
	transact.print(time1, time2);
	if(next!=NULL)
		next->print(time1, time2);
}

void myDynamicListNode::print(dateStr *date1, dateStr *date2){
	transact.print(date1, date2);
	if(next!=NULL)
		next->print(date1, date2);
}

void myDynamicListNode::print(timeStr *time1, timeStr *time2, dateStr *date1, dateStr *date2){
	transact.print(time1, time2, date1, date2);
	if(next!=NULL)
		next->print(time1, time2, date1, date2);
}

void myDynamicListNode::printTotalEarnings(){
	int amount = transact.value;

	myDynamicListNode *temp = next;
	while(temp!=NULL){
		amount += temp->transact.value;
		temp = temp->next;
	}
	cout << "User has receiver a total of: " << amount << "$." << endl;
}

void myDynamicListNode::printTotalPayments(){
	int amount = transact.value;

	myDynamicListNode *temp = next;
	while(temp!=NULL){
		amount += temp->transact.value;
		temp = temp->next;
	}
	cout << "User has spend a total of: " << amount << "$." << endl;
}



