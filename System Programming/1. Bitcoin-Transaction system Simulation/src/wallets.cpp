#include "wallets.hpp"

void bitcoinList::add(char *word, int bitCoinValue){
	btnode *tempA, *tempB;

	if( btnodePtr == NULL ){
		btnodePtr = new btnode;
		btnodePtr->btID = atoi(word);
		btnodePtr->balance = bitCoinValue;
		btnodePtr->next = NULL;
	}else{
		tempA = btnodePtr;
		while(tempA->next != NULL)
			tempA = tempA->next;

		tempB = new btnode;
		tempB->btID = atoi(word);
		tempB->balance = bitCoinValue;
		tempB->next = NULL;

		tempA->next = tempB;
	}
}

void bitcoinList::print(){
	btnode *br;

	for(br = btnodePtr; br != NULL; br = br->next)
		cout << br->btID << " " << br->balance << " ";
}


int bitcoinList::decreaseBalance(int ammount){
	btnodePtr->balance -= ammount;
	return btnodePtr->btID;
}

void bitcoinList::addBitCoin(int id, int ammount){
	int flag = 1;

	btnode *temp = btnodePtr;
	if(temp != NULL) {
		while(temp->next != NULL){
			if(id == temp->btID){
				temp->balance += ammount;
				flag = 0;
				break;
			}
			temp = temp->next;
		}
		if(flag){
			if(id == temp->btID){
				temp->balance +=ammount;
				flag = 0;
			}

			if(flag){
				temp->next = new btnode;
				temp->next->btID = id;
				temp->next->balance = ammount;
				temp->next->next = NULL;
			}
		}
	}else{
		btnodePtr = new btnode;
		btnodePtr->btID = id;
		btnodePtr->balance = ammount;
		btnodePtr->next = NULL;
	}
}

int bitcoinList::appendFront(){
	int id = btnodePtr->btID;
	if(btnodePtr->next!=NULL){
		btnode *temp = btnodePtr->next;
		delete(btnodePtr);
		btnodePtr = temp;
	}else{
		delete(btnodePtr);
		btnodePtr = NULL;
	}
	return id;
}

int wallet::checkBalance(int ammount){
	if(ammount <= totalBalance)
		return 1;
	else
		return 0;
}

int wallet::spendBitCoins(int ammount, wallet *user, int *id, int *amountLeft, int *amountSpent){
	if(bitcoins.getBalance() == ammount){
		int temp = bitcoins.getBalance();
		*id = bitcoins.appendFront();
		user->receiveBitCoins(*id, temp);
		*amountLeft = ammount - temp;
		*amountSpent = temp;

		return 1;
	}else if(bitcoins.getBalance() > ammount){
		*id = bitcoins.decreaseBalance(ammount);
		totalBalance -= ammount;
		user->receiveBitCoins(*id, ammount);
		*amountLeft = 0;
		*amountSpent = ammount;

		return 1;
	}else{
		int temp = bitcoins.getBalance();
		totalBalance -= temp;
		*id = bitcoins.appendFront();
		user->receiveBitCoins(*id, temp);

		*amountLeft = ammount - temp;
		*amountSpent = temp;

		return 0;
	}
}

void wallet::receiveBitCoins(int id, int ammount){
	totalBalance += ammount;
	bitcoins.addBitCoin(id, ammount);
}

//Given a cstring containing the userID, creates and initializes
//and adds a new wallet/user on the linked list.
void walletList::add(char *word){
	wallet *tempA, *tempB;

	if( nPtr == NULL ){
		nPtr = new wallet;
		nPtr->usID.fillId(word);
		nPtr->totalBalance = 0;
		nPtr->next = NULL;
	}else{
		tempA = nPtr;
		while(tempA->next != NULL)
			tempA = tempA->next;

		tempB = new wallet;
		tempB->usID.fillId(word);
		tempB->totalBalance = 0;
		tempB->next = NULL;

		tempA->next = tempB;
	}
}

//Given a cstring containing the bitcoinID, creates, initializes and adds
//a new bitcoin on the last user. This function only works with the bitCoinBalancesFile
void walletList::addBitcoin(char *word, int bitCoinValue){
	wallet *temp = nPtr;
	while(temp->next != NULL)
		temp = temp->next;

	temp->totalBalance += bitCoinValue;
	temp->bitcoins.add(word, bitCoinValue);
}

wallet *walletList::findUser(char *user){
	for(wallet *pr = nPtr; pr != NULL; pr = pr->next){
		if(!strcmp(user, pr->usID.id)){
			return pr;
		}
	}
	return NULL;
}

void walletList::print(){
	wallet *pr;
	cout << "print" << endl;

	for(pr = nPtr; pr != NULL; pr = pr->next){
		cout << pr->usID.id << " ";
		pr->bitcoins.print();
		cout << endl;
	}
}
