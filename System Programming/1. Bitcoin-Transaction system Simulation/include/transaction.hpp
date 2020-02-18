#ifndef transaction_hpp
#define transaction_hpp

#include "wallets.hpp"

using namespace std;

int isD(char );
int checkIsDate(char *);
int checkIsTime(char *);

//A simple date structure
struct dateStr{
	int d;
	int m;
	int y;

	void fill(int a, int b, int c);

	int addDate(char *word);
};

int compareDates(dateStr *, dateStr *);


//A simple time structure
struct timeStr{
	int h;
	int m;

	void fill(int a, int b);

	int addTime(char *word);
};

int compareTimes(timeStr *, timeStr *);


struct transaction{
	int 	transactionID;
	userID 	*fromWalletID;
	userID 	*toWalletID;
	int 	value;
	dateStr date;
	timeStr time;

	int  addDate(char *word);

	int  addTime(char *word);

	int fill(char *line, walletList *wallet, int flagTr);

	void copyTransaction(transaction *des);

	void print();

	void print(timeStr *time1, timeStr *time2);

	void print(dateStr *date1, dateStr *date2);

	void print(timeStr *time1, timeStr *time2, dateStr *date1, dateStr *date2);

};

#endif /* TRANSACTION_HPP_ */
