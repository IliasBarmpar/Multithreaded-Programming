#ifndef wallets_hpp
#define wallets_hpp

#include <string.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

struct btnode{
	int btID;
	int balance;
	btnode *next;
};

class bitcoinList{
	btnode *btnodePtr;
public:
	bitcoinList(){ btnodePtr = NULL; }
	~bitcoinList(){
		btnode *head = btnodePtr, *temp;
		while(head != NULL){
			temp = head;
			head = head->next;
			free(temp);
		}
	};

	void add(char *word, int bitCoinValue);

	int getBalance(){ return btnodePtr->balance; }

	int decreaseBalance(int ammount);

	void addBitCoin(int id, int ammount);

	int appendFront();

	void print();
};

struct userID{
	char id[50];
	void fillId(char *word){ strcpy(id, word); }
};

struct wallet{
	userID 			usID;
	int 			totalBalance;
	bitcoinList 	bitcoins;
	wallet 			*next;

	int checkBalance(int ammount);

	int spendBitCoins(int ammount, wallet *user, int *id, int *amountLeft, int *amountSpent);

	void receiveBitCoins(int id, int ammount);
};

class walletList{
	wallet *nPtr;
public:
	walletList(){ nPtr = NULL; };

	~walletList(){
		wallet *head = nPtr, *temp;
		while(head != NULL){
			temp = head;
			head = head->next;
			temp->bitcoins.~bitcoinList();
			free(temp);
		}
	};

	//Given a cstring containing the userID, creates and initializes
	//and adds a new wallet/user on the linked list.
	void add(char *word);

	//Given a cstring containing the bitcoinID, creates, initializes and adds
	//a new bitcoin on the last user. This function only works with the bitCoinBalancesFile
	void addBitcoin(char *word, int bitCoinValue);

	wallet *findUser(char *user);

	void print();
};

#endif
