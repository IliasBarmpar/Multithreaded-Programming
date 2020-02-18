#ifndef bitcointree_hpp
#define bitcointree_hpp

#include "transaction.hpp"
#include "bucketNode.hpp"

struct treeNd{
	userID 		*wallPtr;
	myDynamicListNode *mDLN;
	transaction *tra;
	int 		dataVal;
	treeNd 		*left;
	treeNd 		*right;

	void print();

	void printCoin();
};

class treeNode{
public:
	treeNd *treeNdPtr;

	treeNode(){ treeNdPtr = NULL; };
	~treeNode(){ };

	void add(char *user, walletList *wallet, int bitCoinValue);

	void traAdd(transaction *tra, int amm, walletList *wallets, treeNode **curr, treeNode **next);

	void traAdd(transaction *tra, int amm, walletList *wallets, treeNode **curr);

	void printCoin();

	void print();
};

struct leafNode{
	treeNode *curr;
	leafNode *next;

	void print(){
		cout << curr->treeNdPtr->wallPtr->id << " " << curr->treeNdPtr->dataVal << endl;
		if(next!=NULL)
			next->print();
 	}
};

struct treeHead{
	int transactions;
	userID 		*oguser;
	leafNode 	*leaves;
	int 		bitID;
	treeNode	data;
	treeHead 	*next;

	void print(){
		cout << bitID << ": ";
		data.print();
	}
};

class bitcoinTree{
private:
	treeHead *treeHeadPtr;
public:
	bitcoinTree(){ treeHeadPtr = NULL; };
	~bitcoinTree(){};

	int add(char *bitc, char *user, walletList *wallet, int bitCoinValue);

	void traAdd(transaction *tra, walletList *wallets, int id, int amm);

	//Uses the leaves for the ‘/bitCoinStatus’ function
	void printStatus(int id);

	//Parses a bitcoinTree and prints that transactions that took place.
	void printCoin(int id);

	void print();
};



#endif /* BITCOINTREE_HPP_ */
