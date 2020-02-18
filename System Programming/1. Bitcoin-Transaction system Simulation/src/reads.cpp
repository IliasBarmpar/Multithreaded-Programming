#include "reads.hpp"

using namespace std;

//Simple one on one assignments from cmd to static variables
int readCmd( int argc, char **argv, char **bitCoinBalancesFile, char **transactionsFile, int *bitCoinValue,
		int *senderHashtableNumOfEntries, int *receiverHashtable1NumOfEntries, int *bucketSize ){
	/* CMD parser */
	
	char *pEnd;
	if(argc==13){
		if( !strcmp(argv[1], "-a") && !strcmp(argv[3], "-t") && !strcmp(argv[5], "-v") && !strcmp(argv[7], "-h1") && !strcmp(argv[9], "-h2") && !strcmp(argv[11], "-b")){
			*bitCoinBalancesFile			= argv[2];
			*transactionsFile			= argv[4];
			*bitCoinValue				= atoi(argv[6]); //http://www.cplusplus.com/reference/string/stod/
			*senderHashtableNumOfEntries	= atoi(argv[8]);
			*receiverHashtable1NumOfEntries	= atoi(argv[10]);
			*bucketSize						= atoi(argv[12]);
		}else{
			return -1;
		}
	}else{
		return -1;
	}

	return 0;
}

//Reads the balance file line by line and splits into tokens using strtok.
//Each line contains a userID and all bitoinIDs this user owns.
int readBalanceFile( char filename[], bitcoinTree *bitcoinTree, walletList *wl , int bitCoinValue){
	char copy[50];
	char *word = NULL, *line = NULL;
	size_t len = 0;
	FILE *fp = NULL;

	if( (fp=fopen(filename, "r")) != NULL){
		while((getline(&line, &len, fp))!=-1){
			word = strtok(line, " \n\r");
			wl->add(word);
			strcpy(copy, word);

			word = strtok(NULL, " \n\r");
			while( word != NULL ){
				wl->addBitcoin(word, bitCoinValue);
				int err = bitcoinTree->add(word, copy, wl, bitCoinValue);
				if(err)
					return -1;
				word = strtok(NULL, " \n\r");
			}
			word = NULL;
			line = NULL;
		}
		fclose(fp);
	}else{
		cout << "Could not open the Balances File." << endl;
		return -1;
	}
	return 0;
}

//Reads the transactions file line by line
//If the structure of each line is correct, it proceeds to add the transaction
//to the senderHashtable, receiverHashtable and make the necessary adjustments on the bitcoinTree
int readTransactionsFile( char filename[], bitcoinTree *bitcoinTree, senderHashTable *senderHashtable, receiverHashTable *receiverHashtable,walletList *wallets){
	char *line = NULL;
	size_t len = 0;
	FILE *fp = NULL;

	if( (fp=fopen(filename, "r")) != NULL){
		while((getline(&line, &len, fp))!=-1){
			char *word = strtok(line, "\r\n");
			if(word!=NULL){
				if(word[0]==' '){
					cout << "Incorrect line structure." << endl;
				}else{
					int spaceCount = 0; calcSpaceCount(&spaceCount, word);
					int wordCount  = 0; calcWordCount(&wordCount, word);

					if( (wordCount-1 == spaceCount) && wordCount == 6){
						transaction *tra = new transaction;
//						cout << "word:" << word << endl;
						int err = tra->fill(word, wallets, 0);
						if(!err){
							senderHashtable->addRecord(tra, wallets, bitcoinTree);
							receiverHashtable->addRecord(tra, wallets);
						}else{

							cout << "Incorrect line structure." << endl;
							tra->print();
						}
					}else{
						cout << "Incorrect line structure." << endl;
					}
				}
			}
		}
		fclose(fp);
	}else{
		return -1;
	}
	return 0;
}

//Reads an input file line by line
//If the structure of each line is correct, it proceeds to add the transaction
//to the senderHashtable, receiverHashtable and make the necessary adjustments on the bitcoinTree
int readInputFile( char filename[], bitcoinTree *bitcoinTree, senderHashTable *senderHashtable, receiverHashTable *receiverHashtable,walletList *wallets){
	char *line = NULL;
	size_t len = 0;
	FILE *fp = NULL;
	if( (fp=fopen(filename, "r")) != NULL){
		while((getline(&line, &len, fp))!=-1){
			char *word = strtok(line, "\r\n");
			if(word!=NULL){
				if(word[0]==' '){
					cout << "Incorrect line structure." << endl;
				}else{
					int spaceCount = 0; calcSpaceCount(&spaceCount, word);
					int wordCount  = 0; calcWordCount(&wordCount, word);

					if( (wordCount-1 == spaceCount) && wordCount == 5){
						transaction *tra = new transaction;

						int err = tra->fill(word, wallets, 1);
						if(!err){
							senderHashtable->addRecord(tra, wallets, bitcoinTree);
							receiverHashtable->addRecord(tra, wallets);
						}else{
							cout << "Incorrect line structure." << endl;
						}
					}else if((wordCount-1 == spaceCount) && wordCount == 3){
						transaction *tra = new transaction;

						int err = tra->fill(word, wallets, 2);
						if(!err){
							senderHashtable->addRecord(tra, wallets, bitcoinTree);
							receiverHashtable->addRecord(tra, wallets);
						}else{
							cout << "Incorrect line structure." << endl;
						}
					}else{
						cout << "Incorrect line structure." << endl;
					}
				}
			}
		}
		fclose(fp);
	}
}
