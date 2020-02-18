#include "app.hpp"


using namespace std;

void application::requestTransaction(char *word, walletList *wallets, bitcoinTree *btt, senderHashTable *senderHashtable,receiverHashTable *receiverHashtable){
	if(word!=NULL){
		if(word[0]==' '){
			printError1();
		}else{
			int spaceCount = 0; calcSpaceCount(&spaceCount, word);
			int wordCount = 0;  calcWordCount(&wordCount, word);

			if( wordCount-1 == spaceCount ){

				if(wordCount == 3){
					transaction *tra = new transaction;

					int err = tra->fill(word, wallets, 2);
					if(!err){
						{
							time_t t = time(NULL);
							struct tm tm = *localtime(&t);
							tra->time.h = tm.tm_hour;
							tra->time.m = tm.tm_min;
							tra->date.d = tm.tm_mday;
							tra->date.m = tm.tm_mon + 1;
							tra->date.y = tm.tm_year + 1900;
						}

						int err = senderHashtable->addRecord(tra, wallets, btt);
						receiverHashtable->addRecord(tra, wallets);
						if(!err)
							cout << "Transaction was successsful." << endl;
					}else{
						printError1();
					}

				}else if(wordCount == 5){
					transaction *tra = new transaction;

					int err = tra->fill(word, wallets, 1);
					if(!err){
						int err = senderHashtable->addRecord(tra, wallets, btt);
						receiverHashtable->addRecord(tra, wallets);
						if(!err)
							cout << "Transaction was successsful." << endl;
					}else{
						printError1();
					}
				}else{
					printError1();
				}
			}else{
				printError1();
			}
		}
	}else{
		printError1();
	}
}


void application::findEarnings(walletList *wallets, receiverHashTable *receiverHashtable){
	char *word = strtok(NULL, "\n");
	if(word!=NULL){
		if(word[0]==' '){
			cout << "error" <<endl;
		}else{
			int spaceCount = 0; calcSpaceCount(&spaceCount, word);

			if(spaceCount==0){
				word = strtok(word, " ");
				userID *temp = &wallets->findUser(word)->usID;
				if(temp == NULL){
					cout << "Error: User does not exist." << endl;
				}else{
					receiverHashtable->printEarnings( &wallets->findUser(word)->usID, wallets);
				}
			}else if( spaceCount == 2 ){
				word = strtok(word, " ");
				userID *temp = &wallets->findUser(word)->usID;
				if(temp == NULL){
					cout << "Error: User does not exist." << endl;
				}else{
					word = strtok(NULL, " ");
					if(checkIsTime(word)){
						char *word2 = strtok(NULL, " ");
						if(checkIsTime(word2)){
							timeStr time1, time2;
							time1.addTime(word);
							time2.addTime(word2);

							if(compareTimes(&time1, &time2)){
								receiverHashtable->printEarnings( &wallets->findUser(temp->id)->usID, wallets, &time1, &time2);
							}else{
								cout << "Error: Time needs to..." << endl;
							}

						}else{
							cout << "Error." << endl;
						}
					}else if(checkIsDate(word)){
						char *word2 = strtok(NULL, " ");
						if(checkIsDate(word2)){
							dateStr date1, date2;
							date1.addDate(word);
							date2.addDate(word2);

							if(compareDates(&date1, &date2)){
								receiverHashtable->printEarnings( &wallets->findUser(temp->id)->usID, wallets, &date1, &date2);
							}else{
								cout << "Error." << endl;
							}
						}else{
							cout << "Error." << endl;
						}
					}else{
						cout << "Error." << endl;
					}
				}



			}else if( spaceCount == 4 ){
				word = strtok(word, " ");
				userID *temp = &wallets->findUser(word)->usID;
				if(temp == NULL){
					cout << "Error: User does not exist." << endl;
				}else{
					word 		= strtok(NULL, " ");
					char *word2 = strtok(NULL, " ");
					char *word3 = strtok(NULL, " ");
					char *word4 = strtok(NULL, " ");
					if(checkIsTime(word) && checkIsDate(word2) && checkIsTime(word3) && checkIsDate(word4)){
						timeStr time1, time2;
						dateStr date1, date2;
						time1.addTime(word);
						date1.addDate(word2);
						time2.addTime(word3);
						date2.addDate(word4);
						if(compareTimes(&time1, &time2)&&compareDates(&date1, &date2)){
							receiverHashtable->printEarnings( &wallets->findUser(temp->id)->usID, wallets, &time1, &time2, &date1, &date2);
						}else{
							cout << "Error: ." << endl;
						}
					}else{
						cout << "Error: ." << endl;
					}
				}
			}else{
				cout << "Error." << endl;
			}
		}
	}else{
		cout << "Error." << endl;
	}
}


void application::findPayments(walletList *wallets, senderHashTable *senderHashtable){
	char *word = strtok(NULL, "\n");
	if(word!=NULL){
		if(word[0]==' '){
			cout << "Error." <<endl;
		}else{
			int spaceCount = 0;

			{
				int i = 0;
				for(; word[i] != '\0'; i++)
					if(word[i]==' ')
						spaceCount++;
				for(int j = i-1; word[j]==' '; j--)
					spaceCount--;
			}

			if( spaceCount == 0 ){
				word = strtok(word, " ");
				userID *temp = &wallets->findUser(word)->usID;
				if(temp == NULL){
					cout << "Error: User does not exist." << endl;
				}else{
					senderHashtable->printPayments( &wallets->findUser(temp->id)->usID, wallets );
				}
			}else if( spaceCount == 2 ){
				word = strtok(word, " ");
				userID *temp = &wallets->findUser(word)->usID;
				if(temp == NULL){
					cout << "Error: User does not exist." << endl;
				}else{
					word = strtok(NULL, " ");
					if(checkIsTime(word)){
						char *word2 = strtok(NULL, " ");
						if(checkIsTime(word2)){
							timeStr time1, time2;
							time1.addTime(word);
							time2.addTime(word2);

							if(compareTimes(&time1, &time2)){
								senderHashtable->printPayments( &wallets->findUser(temp->id)->usID, wallets, &time1, &time2);
							}else{
								cout << "Error: Time needs to..." << endl;
							}

						}else{
							cout << "Error." << endl;
						}
					}else if(checkIsDate(word)){
						char *word2 = strtok(NULL, " ");
						if(checkIsDate(word2)){
							dateStr date1, date2;
							date1.addDate(word);
							date2.addDate(word2);

							if(compareDates(&date1, &date2)){
								senderHashtable->printPayments( &wallets->findUser(temp->id)->usID, wallets, &date1, &date2);
							}else{
								cout << "Error." << endl;
							}
						}else{
							cout << "Error." << endl;
						}
					}else{
						cout << "Error." << endl;
					}
				}
			}else if( spaceCount == 4 ){
				word = strtok(word, " ");
				userID *temp = &wallets->findUser(word)->usID;
				if(temp == NULL){
					cout << "Error: User does not exist." << endl;
				}else{
					word 		= strtok(NULL, " ");
					char *word2 = strtok(NULL, " ");
					char *word3 = strtok(NULL, " ");
					char *word4 = strtok(NULL, " ");
					if(checkIsTime(word) && checkIsDate(word2) && checkIsTime(word3) && checkIsDate(word4)){
						timeStr time1, time2;
						dateStr date1, date2;
						time1.addTime(word);
						date1.addDate(word2);
						time2.addTime(word3);
						date2.addDate(word4);
						if( compareTimes(&time1, &time2) && compareDates(&date1, &date2) ){
							senderHashtable->printPayments( &wallets->findUser(temp->id)->usID, wallets, &time1, &time2, &date1, &date2);
						}else{
							cout << "Error: ." << endl;
						}
					}else{
						cout << "Error: ." << endl;
					}
				}
			}else{
				cout << "Error." << endl;
			}
		}
	}else{
		cout << "Null." << endl;
	}
}

void application::walletStatus(walletList *wallets){
	char *word = strtok(NULL, "\n");
	if(word!=NULL){
		if(word[0]==' '){
			cout << "error" <<endl;
		}else{
			int spaceCount = 0;	calcSpaceCount(&spaceCount, word);
			if(spaceCount==0){
				word = strtok(word, " ");
				userID *temp = &wallets->findUser(word)->usID;
				if(temp == NULL){
					cout << "Error: User does not exist." << endl;
				}else{
					cout << "'" << word << "' currently has " << wallets->findUser(word)->totalBalance << "$." << endl;
				}
			}else{
				cout << "Error: Incorrect input." << endl;
			}
		}
	}else{
		cout << "Error: Incorrect input." << endl;
	}
}

void application::bitCoinStatus(walletList *wallets, bitcoinTree *btt){
	char *word = strtok(NULL, "\n");
	if(word!=NULL){
		if(word[0]==' '){
			cout << "error" <<endl;
		}else{
			int spaceCount = 0;	calcSpaceCount(&spaceCount, word);
			if(spaceCount==0){
				word = strtok(word, " ");
				int id = atoi(word);
				btt->printStatus(id);
			}else{
				cout << "Error: Incorrect input." << endl;
			}
		}
	}else{
		cout << "Error: Incorrect input." << endl;
	}
}

void application::traceCoin(bitcoinTree *btt){
	char *word = strtok(NULL, "\n");
	if(word!=NULL){
		if(word[0]==' '){
			cout << "Error." <<endl;
		}else{
			int spaceCount = 0;	calcSpaceCount(&spaceCount, word);
			if(spaceCount==0){
				word = strtok(word, " ");
				int id = atoi(word);
				btt->printCoin(id);
			}else{
				cout << "Error: Incorrect input." << endl;
			}
		}
	}else{
		cout << "Error: Incorrect input." << endl;
	}
}

void application::run(walletList *wallets, bitcoinTree *btt, senderHashTable *senderHashtable,receiverHashTable *receiverHashtable){
	int flag = 1;
	while(flag){
		cout << "Enter your command here:" << endl;
		char *line = NULL;
		char *word = NULL;
		size_t len = 0;
		getline(&line, &len, stdin);
		word = strtok(line, " \r\n");

		if(word!=NULL){
			if(!strcmp(word, "/requestTransaction")){
				word = strtok(NULL, "\n");
				requestTransaction(word, wallets, btt, senderHashtable, receiverHashtable);

			}else if(!strcmp(word, "/requestTransactions")){
				word = strtok(NULL, "\n");
				if(word != NULL){
					int wordCount = 0;  calcWordCount(&wordCount, word);
					if(wordCount==1){
						if(word[0]==' '){
							printError1();
						}else{
							readInputFile(word, btt, senderHashtable, receiverHashtable, wallets);
						}
					}else{
						do{
							requestTransaction(word, wallets, btt, senderHashtable, receiverHashtable);
							getline(&line, &len, stdin);
							word = strtok(line, "\r\n");
						}while(word!=NULL);
					}
				}else{
					printError1();
				}

			}else if(!strcmp(word, "/findEarnings")){
				findEarnings(wallets, receiverHashtable);

			}else if(!strcmp(word, "/findPayments")){
				findPayments(wallets, senderHashtable);

			}else if(!strcmp(word, "/walletStatus")){
				walletStatus(wallets);

			}else if(!strcmp(word, "/bitCoinStatus")){
				bitCoinStatus(wallets, btt);

			}else if(!strcmp(word, "/traceCoin")){
				traceCoin(btt);

			}else if(!strcmp(word, "/exit")){
				//TODO free
				flag = 0;
			}else{
				cout << "Error." << endl;
			}
		}
	}
}/*
*/


