#ifndef APP_HPP_
#define APP_HPP_

#include "bitCoinTree.hpp"
#include "senderHashtable.hpp"
#include "receiverHashtable.hpp"
#include "functions.hpp"
#include "transaction.hpp"
#include "reads.hpp"

using namespace std;

class application{
public:

	void requestTransaction(char *word, walletList *wallets, bitcoinTree *btt, senderHashTable *senderHashtable,receiverHashTable *receiverHashtable);

	void findEarnings(walletList *wallets, receiverHashTable *receiverHashtable);

	void findPayments(walletList *wallets, senderHashTable *senderHashtable);

	void walletStatus(walletList *wallets);

	void bitCoinStatus(walletList *wallets, bitcoinTree *btt);

	void traceCoin(bitcoinTree *btt);

	void run(walletList *wallets, bitcoinTree *btt, senderHashTable *senderHashtable,receiverHashTable *receiverHashtable);
};



#endif /* APP_HPP_ */
