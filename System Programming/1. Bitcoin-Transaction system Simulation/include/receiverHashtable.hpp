#ifndef receiverHashtable_hpp
#define receiverHashtable_hpp

#include "bucketNode.hpp"

class receiverBucket{
  public:
	int records;
	int bytesPerBucket;
	int currentSizeLeft;
	receiverBucket *next;
	void *bytesMB;

	receiverBucket(int s);

	int addRecord(transaction *tra, walletList *wallet);

	void printEarnings(userID *user, walletList *wallets);

	void printEarnings(userID *user, walletList *wallets, timeStr *time1, timeStr *time2);

	void printEarnings(userID *user, walletList *wallets, dateStr *date1, dateStr *date2);

	void printEarnings(userID *user, walletList *wallets, timeStr *time1, timeStr *time2, dateStr *date1, dateStr *date2);

	void print();
};

class receiverHashTable{
	int bucketSize;
	int size;
	dateStr lastRecordedDate;
	timeStr lastRecordedTime;
	receiverBucket **map;
public:
	receiverHashTable(int s, int bs);

	~receiverHashTable(){};

	int hashFunction(char *word);

	void addRecord(transaction *tra, walletList *wallets);

	void printEarnings(userID *user, walletList *wallets);

	void printEarnings(userID *user, walletList *wallets, timeStr *time1, timeStr *time2);

	void printEarnings(userID *user, walletList *wallets, dateStr *date1, dateStr *date2);

	void printEarnings(userID *user, walletList *wallets, timeStr *time1, timeStr *time2, dateStr *date1, dateStr *date2);

	void print();
};

#endif /* TEMPFILE2_HPP_ */
