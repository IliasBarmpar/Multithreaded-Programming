#include "senderHashtable.hpp"

senderBucket::senderBucket(int s){
	records = 0;
	bytesPerBucket  = s - sizeof(int)*3 - sizeof(senderBucket *);
	currentSizeLeft = s - sizeof(int)*3 - sizeof(senderBucket *);
	next = NULL;

	if(bytesPerBucket >0 ){
		bytesMB = malloc(bytesPerBucket);
	}else{
		currentSizeLeft = 1;
		bytesPerBucket = -1;
	}
}

int senderBucket::addRecord(transaction *tra, walletList *wallet){
	if(records==0){
		if(currentSizeLeft >= sizeof(bucketNode)){
			bucketNode buc;
			buc.usID = tra->fromWalletID;

			buc.head = new myDynamicListNode;
			buc.head->next = NULL;
			buc.head->transact.copyTransaction(tra);

			memcpy(bytesMB, (void *)(&buc), sizeof(bucketNode));

			records++;
			currentSizeLeft -= sizeof(bucketNode);
		}else{
			return -1;
		}
	}else{
		//Parse users
		int flag = 1;
		bucketNode *bPtr = (bucketNode *)(bytesMB);
		for(int i = 0; i < records ; i++){
			if(!strcmp(bPtr->usID->id, tra->fromWalletID->id)){ //User found
				//add new transaction to user
				myDynamicListNode *temp = bPtr->head;

				while(temp->next != NULL)
					temp = temp->next;

				myDynamicListNode *temp2 = new myDynamicListNode;
				temp2->transact.copyTransaction(tra);

				temp2->next = NULL;

				temp->next = temp2;

				flag = 0;
				break;
			}
			bPtr++;
		}

		//flag
		if(flag){
			if(next == NULL ){
				if( currentSizeLeft >= sizeof(bucketNode) ){
					bucketNode buc;
					buc.usID = tra->fromWalletID;
					buc.head = new myDynamicListNode;
					buc.head->next = NULL;
					buc.head->transact.copyTransaction(tra);

					memcpy((void *)bPtr, (void *)(&buc), sizeof(bucketNode));
					currentSizeLeft -= sizeof(bucketNode);

					records++;
				}else{
					next = new senderBucket(bytesPerBucket + sizeof(int)*3 + sizeof(bucketNode *));
					next->addRecord(tra, wallet);
				}
			}else{
				next->addRecord(tra, wallet);
			}
		}
	}
	return 0;
}

void senderBucket::printPayments(userID *user, walletList *wallets){
	if(records==0){
		cout << "User does not have any payments." << endl;
	}else{
		int flag = 1;
		bucketNode *bPtr = (bucketNode *)(bytesMB);
		for(int i = 0; i < records ; i++){
			if(!strcmp(bPtr->usID->id, user->id)){
				//User found
				bPtr->head->printTotalPayments();
				bPtr->head->print();

				flag = 0;
				break;
			}
			bPtr++;
		}
		if(flag){
			if(next == NULL ){
				cout << "User does not have any payments." << endl;
			}else{
				next->printPayments(user, wallets);
			}
		}
	}
}

void senderBucket::printPayments(userID *user, walletList *wallets, timeStr *time1, timeStr *time2){
	if(records==0){
		cout << "User does not have any payments." << endl;
	}else{
		int flag = 1;
		bucketNode *bPtr = (bucketNode *)(bytesMB);
		for(int i = 0; i < records ; i++){
			if(!strcmp(bPtr->usID->id, user->id)){
				//User found
				bPtr->head->printTotalPayments();
				bPtr->head->print(time1, time2);

				flag = 0;
				break;
			}
			bPtr++;
		}

		if(flag){
			if(next == NULL ){
				cout << "User does not have any payments." << endl;
			}else{
				next->printPayments(user, wallets, time1, time2);
			}
		}
	}
}

void senderBucket::printPayments(userID *user, walletList *wallets, dateStr *date1, dateStr *date2){
	if(records==0){
		cout << "User does not have any payments." << endl;
	}else{
		int flag = 1;
		bucketNode *bPtr = (bucketNode *)(bytesMB);
		for(int i = 0; i < records ; i++){
			if(!strcmp(bPtr->usID->id, user->id)){
				//User found
				bPtr->head->printTotalPayments();
				bPtr->head->print(date1, date2);

				flag = 0;
				break;
			}
			bPtr++;
		}

		if(flag){
			if(next == NULL ){
				cout << "User does not have any payments." << endl;
			}else{
				next->printPayments(user, wallets, date1, date2);
			}
		}
	}
}

void senderBucket::printPayments(userID *user, walletList *wallets, timeStr *time1, timeStr *time2, dateStr *date1, dateStr *date2){
	if(records==0){
		cout << "User does not have any payments." << endl;
	}else{
		int flag = 1;
		bucketNode *bPtr = (bucketNode *)(bytesMB);
		for(int i = 0; i < records ; i++){
			if(!strcmp(bPtr->usID->id, user->id)){
				//User found
				bPtr->head->printTotalPayments();
				bPtr->head->print(time1, time2, date1, date2);

				flag = 0;
				break;
			}
			bPtr++;
		}

		if(flag){
			if(next == NULL ){
				cout << "User does not have any payments." << endl;
			}else{
				next->printPayments(user, wallets, time1, time2, date1, date2);
			}
		}
	}
}

void senderBucket::print(){
	if(records==0){
		cout << "><";
	}else{
		bucketNode *bPtr = (bucketNode *)bytesMB;
		for(int i = 0; i < records ; i++){
			cout << bPtr->usID->id << endl;
			bPtr->head->print();
			cout << endl;
			bPtr++;
		}
		if(next!= NULL){
			cout << " :next: " << endl;
			next->print();
		}
	}
}

senderHashTable::senderHashTable(int s, int bs){
	size = s;
	bucketSize = bs;
	lastRecordedDate.fill(0, 0, 0);
	lastRecordedTime.fill(0, 0);
	map = new senderBucket*[size];
	for(int i = 0; i < size;i++){
		map[i] = new senderBucket(bucketSize);
	}
}

int senderHashTable::addRecord(transaction *tra, walletList *wallets, bitcoinTree *btt){
	if( (lastRecordedDate.d == tra->date.d) && (lastRecordedDate.m == tra->date.m) && (lastRecordedDate.y == tra->date.y)){
		if(compareTimes(&lastRecordedTime, &tra->time)){
			int key = hashFunction(tra->fromWalletID->id);
			transaction *loc;
			int err = map[key]->addRecord(tra, wallets);
			if(!err){
				lastRecordedDate.fill(tra->date.d, tra->date.m, tra->date.y);
				lastRecordedTime.fill(tra->time.h, tra->time.m);

				int flag = 1, amountLeft = 0, amountSpent = 0;
				while(flag){
					int id;
					int note = wallets->findUser(tra->fromWalletID->id)->spendBitCoins( tra->value - amountLeft, wallets->findUser(tra->toWalletID->id) , &id, &amountLeft, &amountSpent);
					if(note==1){
						btt->traAdd(tra, wallets, id, amountSpent);
						flag = 0;
					}else{
						btt->traAdd(tra, wallets, id, amountSpent);
						flag = 1;
					}
				}
				return 0;
			}else{
				cout << "Something went wrong with adding the record." << endl;
				return -1;
			}
		}else{
			cout << "Something went wrong with adding the record." << endl;
			return -1;
		}
	}else{
		if(compareDates(&lastRecordedDate, &tra->date)){
			int key = hashFunction(tra->fromWalletID->id);
			int err = map[key]->addRecord(tra, wallets);
			if(!err){
				lastRecordedDate.fill(tra->date.d, tra->date.m, tra->date.y);
				lastRecordedTime.fill(tra->time.h, tra->time.m);

				int flag = 1, amountLeft = 0, amountSpent = 0;
				while(flag){
					int id;
					int note = wallets->findUser(tra->fromWalletID->id)->spendBitCoins( tra->value - amountSpent, wallets->findUser(tra->toWalletID->id) , &id, &amountLeft, &amountSpent);
					if(note==1){
						btt->traAdd(tra, wallets, id, amountSpent);
						flag = 0;
					}else{
						btt->traAdd(tra, wallets, id, amountSpent);
						flag = 1;
					}
				}
				return 0;
			}else{
				cout << "Something went wrong with adding the record." << endl;
				return -1;
			}
		}else{
			cout << "Something went wrong with adding the record." << endl;
			return -1;
		}
	}
}

int senderHashTable::hashFunction(char *word){
	int count = 0;
	for(int i =0; word[i]!=0;i++)
		count += word[i];
	return (count % size);
}


void senderHashTable::printPayments(userID *user, walletList *wallets){
	int key = hashFunction(user->id);
	map[key]->printPayments(user, wallets);
}

void senderHashTable::printPayments(userID *user, walletList *wallets, timeStr *time1, timeStr *time2){
	int key = hashFunction(user->id);
	map[key]->printPayments(user, wallets, time1, time2);
}

void senderHashTable::printPayments(userID *user, walletList *wallets, dateStr *date1, dateStr *date2){
	int key = hashFunction(user->id);
	map[key]->printPayments(user, wallets, date1, date2);
}

void senderHashTable::printPayments(userID *user, walletList *wallets, timeStr *time1, timeStr *time2, dateStr *date1, dateStr *date2){
	int key = hashFunction(user->id);
	map[key]->printPayments(user, wallets, time1, time2, date1, date2);
}

void senderHashTable::print(){
	for(int i = 0; i < size; i++){
		cout <<"   " << i << endl;
		map[i]->print();
		cout << endl << endl;
	}
}
