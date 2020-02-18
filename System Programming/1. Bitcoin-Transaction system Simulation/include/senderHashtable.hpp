#ifndef senderHashtable_hpp
#define senderHashtable_hpp

#include "bitCoinTree.hpp"
#include "bucketNode.hpp"

class senderBucket{
  public:
	int records;
	int bytesPerBucket;
	int currentSizeLeft;
	senderBucket *next;
	void *bytesMB;

	senderBucket(int s);

	int addRecord(transaction *tra, walletList *wallet);

	void printPayments(userID *user, walletList *wallets);

	void printPayments(userID *user, walletList *wallets, timeStr *time1, timeStr *time2);

	void printPayments(userID *user, walletList *wallets, dateStr *date1, dateStr *date2);

	void printPayments(userID *user, walletList *wallets, timeStr *time1, timeStr *time2, dateStr *date1, dateStr *date2);

	void print();
};

class senderHashTable{
	int bucketSize;
	int size;
	dateStr lastRecordedDate;
	timeStr lastRecordedTime;
	senderBucket **map;
public:
	senderHashTable(int s, int bs);

	~senderHashTable(){};

	int hashFunction(char *word);

	int addRecord(transaction *tra, walletList *wallets, bitcoinTree *btt);

	void printPayments(userID *user, walletList *wallets);

	void printPayments(userID *user, walletList *wallets, timeStr *time1, timeStr *time2);

	void printPayments(userID *user, walletList *wallets, dateStr *date1, dateStr *date2);

	void printPayments(userID *user, walletList *wallets, timeStr *time1, timeStr *time2, dateStr *date1, dateStr *date2);

	void print();
};


#endif /* TEMPFILE_HPP_ */
