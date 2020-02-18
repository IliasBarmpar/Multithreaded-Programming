#include "bitCoinTree.hpp"

void treeNd::print(){
	cout << "|" << wallPtr->id << " " << dataVal << "|" << endl;;
	if(tra!=NULL)
		tra->print();
	if(left!=NULL){
		cout << "l: "<< wallPtr->id << ":";
		left->print();
	}

	if(right!=NULL){
		cout << "r: "<< wallPtr->id << ":";
		right->print();
	}
}

void treeNd::printCoin(){
	if(tra!=NULL)
		tra->print();

	if(left!=NULL)
		left->printCoin();

	if(right!=NULL)
		right->printCoin();
}


void treeNode::add(char *user, walletList *wallet, int bitCoinValue){
	if(treeNdPtr == NULL){
		treeNdPtr = new treeNd;

		treeNdPtr->wallPtr = &wallet->findUser(user)->usID;
		treeNdPtr->dataVal = bitCoinValue;
		treeNdPtr->tra = NULL;
		treeNdPtr->left = NULL;
		treeNdPtr->right = NULL;
	}else{
		cout << "Error:" << endl;
	}
}

void treeNode::traAdd(transaction *tra, int amm, walletList *wallets, treeNode **curr, treeNode **next){
	treeNode *tempA, *tempB;

	tempA = new treeNode;
	tempA->treeNdPtr 			= new treeNd;
	tempA->treeNdPtr->tra		= tra;
	treeNdPtr->left  			= tempA->treeNdPtr;
	tempA->treeNdPtr->wallPtr	= tra->toWalletID;
	tempA->treeNdPtr->tra		= tra;
	tempA->treeNdPtr->dataVal	= amm;
	tempA->treeNdPtr->left 		= NULL;
	tempA->treeNdPtr->right 	= NULL;


	tempB = new treeNode;
	tempB->treeNdPtr 			= new treeNd;
	tempB->treeNdPtr->tra		= NULL;
	treeNdPtr->right 			= tempB->treeNdPtr;
	tempB->treeNdPtr->wallPtr	= tra->fromWalletID;
	tempB->treeNdPtr->dataVal	= treeNdPtr->dataVal - amm;
	tempB->treeNdPtr->left 		= NULL;
	tempB->treeNdPtr->right 	= NULL;

	*curr = tempA;
	*next = tempB;
}

void treeNode::traAdd(transaction *tra, int amm, walletList *wallets, treeNode **curr){
	treeNode *tempA;

	tempA = new treeNode;
	tempA->treeNdPtr 			= new treeNd;
	treeNdPtr->left  			= tempA->treeNdPtr;
	tempA->treeNdPtr->wallPtr	= tra->toWalletID;
	tempA->treeNdPtr->tra		= &(*tra);
	if(treeNdPtr->dataVal>=amm)
		tempA->treeNdPtr->dataVal	= amm;
	else
		tempA->treeNdPtr->dataVal	= treeNdPtr->dataVal;

	tempA->treeNdPtr->left 		= NULL;
	tempA->treeNdPtr->right 	= NULL;

	*curr = tempA;
}

void treeNode::printCoin(){
	if(treeNdPtr->tra!=NULL)
		treeNdPtr->tra->print();

	if(treeNdPtr->left!=NULL)
		treeNdPtr->left->printCoin();

	if(treeNdPtr->right!=NULL)
		treeNdPtr->right->printCoin();

}

void treeNode::print(){
	treeNd *pr = treeNdPtr;
	cout << "" << pr->wallPtr->id << " " << pr->dataVal << " " << endl;;
	if(pr->tra!=NULL)
		pr->tra->print();

	if(pr->left!=NULL){
		cout << ">l:"<< pr->wallPtr->id << ":";
		pr->left->print();
	}

	if(pr->right!=NULL){
		cout << ">r:" << pr->wallPtr->id << ":";
		pr->right->print();
	}
}

int bitcoinTree::add(char *bitc, char *user, walletList *wallet, int bitCoinValue){
	treeHead *tempA, *tempB;

	if( treeHeadPtr == NULL ){
		treeHeadPtr = new treeHead;

		treeHeadPtr->transactions = 0;
		treeHeadPtr->oguser = &wallet->findUser(user)->usID;
		treeHeadPtr->bitID = atoi(bitc);
		treeHeadPtr->data.add(user, wallet, bitCoinValue);
		treeHeadPtr->next = NULL;

		treeHeadPtr->leaves  = new leafNode;
		treeHeadPtr->leaves->curr = &treeHeadPtr->data;
		treeHeadPtr->leaves->next = NULL;
	}else{
		int temp = atoi(bitc);
		//Skip to the last node
		tempA = treeHeadPtr;
		while(tempA->next != NULL){
			if(tempA->bitID == temp)
				return -1;
			tempA = tempA->next;
		}
		if(tempA->bitID == temp)
			return -1;

		//Add new node
		tempB = new treeHead;
		tempB->transactions = 0;
		tempB->oguser = &wallet->findUser(user)->usID;
		tempB->bitID = temp;
		tempB->data.add(user, wallet, bitCoinValue);
		tempB->next = NULL;

		tempB->leaves  = new leafNode;
		tempB->leaves->curr = &tempB->data;
		tempB->leaves->next = NULL;

		tempA->next = tempB;
	}

	return 0;
}

void bitcoinTree::traAdd(transaction *tra, walletList *wallets, int id, int amm){
	int flag = 1;
	treeHead *temp = treeHeadPtr;

	for(temp = treeHeadPtr; temp != NULL; temp = temp->next){
		if( temp->bitID == id ){
			temp->transactions++;
			flag = 0;

			if( temp->leaves->next == NULL ){
				if( amm == temp->leaves->curr->treeNdPtr->dataVal ){
					temp->leaves->curr->traAdd(tra, amm, wallets, &temp->leaves->curr);
				}else{
					temp->leaves->next = new leafNode;
					temp->leaves->next->next = NULL;

					temp->leaves->curr->traAdd(tra, amm, wallets, &temp->leaves->curr, &temp->leaves->next->curr);
				}
			}else{
				leafNode *tempLeaf = temp->leaves;
				if(!strcmp(tra->fromWalletID->id, tempLeaf->curr->treeNdPtr->wallPtr->id)){
					if(tempLeaf->curr->treeNdPtr->dataVal > amm){
						leafNode *temp = tempLeaf->next;
						tempLeaf->next = new leafNode;
						tempLeaf->next->next = temp;
						tempLeaf->curr->traAdd(tra, amm, wallets, &tempLeaf->curr, &tempLeaf->next->curr);

						amm = 0;
					}else if (tempLeaf->curr->treeNdPtr->dataVal == amm){
						tempLeaf->curr->traAdd(tra, amm, wallets, &temp->leaves->curr);

						amm = 0;
					}else{
						tempLeaf->curr->traAdd(tra, amm, wallets, &temp->leaves->curr);
						amm = amm - tempLeaf->curr->treeNdPtr->dataVal;
						cout << amm << endl;

					}
				}


				while(amm){
					tempLeaf = tempLeaf->next;
					if(!strcmp(tra->fromWalletID->id, tempLeaf->curr->treeNdPtr->wallPtr->id)){
						if(tempLeaf->curr->treeNdPtr->dataVal > amm){
							tempLeaf->next = new leafNode;
							tempLeaf->next->next = NULL;
							tempLeaf->curr->traAdd(tra, amm, wallets, &tempLeaf->curr, &tempLeaf->next->curr);

							amm = 0;
						}else if (tempLeaf->curr->treeNdPtr->dataVal == amm){
							tempLeaf->curr->traAdd(tra, amm, wallets, &temp->leaves->curr);

							amm = 0;
						}else{
							tempLeaf->curr->traAdd(tra, amm, wallets, &temp->leaves->curr);

							amm = amm - tempLeaf->curr->treeNdPtr->dataVal;
						}
					}
				}
			}
			break;
		}
	}

	if(flag){
		cout << "Error." << endl;
	}
}

//Uses the leaves for the ‘/bitCoinStatus’ function
void bitcoinTree::printStatus(int id){
	int flag = 1;
	treeHead *temp = treeHeadPtr;

	for(temp = treeHeadPtr; temp != NULL; temp = temp->next){
		if( temp->bitID == id ){
			int unspent = 0;
			for(leafNode *ltemp = temp->leaves; ltemp!=NULL; ltemp = ltemp->next){
				if(!strcmp( temp->oguser->id ,ltemp->curr->treeNdPtr->wallPtr->id)){
					unspent += ltemp->curr->treeNdPtr->dataVal;
				}
			}
			cout << id << " " << temp->transactions << " " << unspent << endl;
			flag = 0;
			break;
		}
	}

	if(flag)
		cout << "BitCoin with such id doesn't exist" << endl;

}

//Parses a bitcoinTree and prints that transactions that took place.
void bitcoinTree::printCoin(int id){
	int flag = 1;

	for(treeHead *temp = treeHeadPtr; temp != NULL; temp = temp->next){
		if( temp->bitID == id ){
			temp->data.printCoin();

			flag = 0;
			break;
		}
	}

	if(flag)
		cout << "BitCoin with such id doesn't exist" << endl;

}

void bitcoinTree::print(){
	treeHead *pr = treeHeadPtr;

	for(pr = treeHeadPtr; pr != NULL; pr = pr->next){
		cout << pr->bitID << " |ogus: " << pr->oguser->id << endl;
		pr->data.print();
		cout << " ";
		cout << endl << endl;
	}
}
