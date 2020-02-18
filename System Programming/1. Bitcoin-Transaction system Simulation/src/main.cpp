#include <iostream>
#include <string.h>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "transaction.hpp"
#include "wallets.hpp"
#include "bitCoinTree.hpp"
#include "senderHashtable.hpp"
#include "receiverHashtable.hpp"
#include "reads.hpp"
#include "functions.hpp"
#include "app.hpp"

using namespace std;

//Reads necessary info from cmd then proceeds to read the input files and then the application starts
int main( int argc, char* argv[] ){
	char *bitCoinBalancesFile, *transactionsFile;
	int bitCoinValue, senderHashtable1NumOfEntries, receiverHashtable1NumOfEntries, bucketSize;

	int err = readCmd(argc, argv, &bitCoinBalancesFile, &transactionsFile, &bitCoinValue, &senderHashtable1NumOfEntries, &receiverHashtable1NumOfEntries, &bucketSize);

	if(err){
		cout << "Incorrect structure for command line arguments." << endl;
		return -1;
	}else{
		walletList wl;
		bitcoinTree bitcoinTree;
		senderHashTable senderHashtable(senderHashtable1NumOfEntries, bucketSize);
		receiverHashTable receiverHashtable(receiverHashtable1NumOfEntries, bucketSize);

		int err1 = readBalanceFile( bitCoinBalancesFile, &bitcoinTree, &wl, bitCoinValue );
		if(err1){
			cout << "Error duplicate bitcoinID." << endl;
			return -1;
		}else{
			readTransactionsFile( transactionsFile , &bitcoinTree, &senderHashtable, &receiverHashtable, &wl);

			application app;
			app.run( &wl, &bitcoinTree, &senderHashtable, &receiverHashtable);
		}
		return 0;
	}
}

