#ifndef READS_HPP_
#define READS_HPP_

#include <cstdio>
#include "wallets.hpp"
#include "bitCoinTree.hpp"
#include "senderHashtable.hpp"
#include "receiverHashtable.hpp"
#include "functions.hpp"

using namespace std;

//Simple one on one assignments from cmd to static variables
int readCmd( int , char **, char **, char **, int *, int *, int *, int * );

//Reads the balance file line by line and splits into tokens using strtok.
//Each line contains a userID and all bitoinIDs this user owns.
int readBalanceFile( char [], bitcoinTree *, walletList * , int );

//Reads the transactions file line by line
//If the structure of each line is correct, it proceeds to add the transaction
//to the senderHashtable, receiverHashtable and make the necessary adjustments on the bitcoinTree
int readTransactionsFile( char [], bitcoinTree *, senderHashTable *, receiverHashTable *,walletList *);

//Reads an input file line by line
//If the structure of each line is correct, it proceeds to add the transaction
//to the senderHashtable, receiverHashtable and make the necessary adjustments on the bitcoinTree
int readInputFile( char [], bitcoinTree *, senderHashTable *, receiverHashTable *,walletList *);
#endif /* READS_HPP_ */
