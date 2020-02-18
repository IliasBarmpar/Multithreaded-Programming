#include "transaction.hpp"

//Check if the character given is a digit
int isD(char ch){
	if(ch >= '0' && ch <= '9')
		return 1;
	return 0;
}

//Check if the given word is compatible with our date structure
int checkIsDate(char *word){
	if(strlen(word)==10){
		if(!(isD(word[6]) && isD(word[7]) && isD(word[8]) && isD(word[9]))){
			return -1;
		}

		if(!(word[2] == '-' && word[5] == '-')){
			return -1;
		}

		if(word[0] == '0' && (word[1] >= '1' && word[1] <= '9') ){
			if(word[3] == '0' && (word[4] >= '1' && word[4] <= '9')){
				return 1;
			}else if(word[3] == '1' && (word[4] >= '0' && word[4] <= '2')){
				return 1;
			}else{
				return 0;
			}
		}else if( (word[0] == '1' || word[0] == '2') && isD(word[1])){
			if(word[3] == '0' && (word[4] >= '1' && word[4] <= '9')){
				return 1;
			}else if(word[3] == '1' && (word[4] >= '0' && word[4] <= '2')){
				return 1;
			}else{
				return 0;
			}
		}else if( word[0] == '3' && (word[1] == '0' || word[1] == '1') ){
			if(word[3] == '0' && (word[4] >= '1' && word[4] <= '9')){
				return 1;
			}else if(word[3] == '1' && (word[4] >= '0' && word[4] <= '2')){
				return 1;
			}else{
				return 0;
			}
		}else{
			return 0;
		}
	}else{
		return 0;
	}
}

//Check if the given word is compatible with our time structure
int checkIsTime(char *word){
	if(strlen(word)==5){
		if( word[2] == ':' && (word[3] >= '0' && word[3] <= '5') && (word[4] >= '0' && word[4] <= '9') ){
			if( word[0]=='0'||word[0]=='1' ){
				if( word[1] >= '0' && word[1] <= '9' ){
					return 1;
				}else{
					return 0;
				}
			}else if(word[0]=='2'){
				if( word[1] >= '0' && word[1] <= '3' ){
					return 1;
				}else if(word[1] == '4'){
					if( word[3] == '0' && word[4]== '0'){
						return 1;
					}else{
						return 0;
					}
				}else{
					return 0;
				}
			}else{
				return 0;
			}
		}else if( word[2]==':' && (word[3] == '6' && word[4] == '0') ){
			if(word[0]=='0'||word[0]=='1'){
				if( word[1] >= '0' && word[1] <= '9' ){
					return 1;
				}else{
					return 0;
				}
			}else if(word[0]=='2'){
				if( word[1] >= '0' && word[1] <= '4' ){
					return 1;
				}else{
					return 0;
				}
			}else{
				return 0;
			}
		}else{
			return 0;
		}
	}else{
		return 0;
	}
}


void dateStr::fill(int a, int b, int c){
	d = a;
	m = b;
	y = c;
}

int dateStr::addDate(char *word){
	if(checkIsDate(word)){
		word = strtok(word, "-\n\r");
		d = atoi(word);

		word = strtok(NULL, "-\n\r");
		m = atoi(word);

		word = strtok(NULL, "-\n\r");
		y = atoi(word);

		return 0;
	}else{
		return -1;
	}
}

//If date2 >= date1 return 1, else return 0
int compareDates(dateStr *date1, dateStr *date2){
	if(date2->y > date1->y){
		return 1;
	}else if(date2->y == date1->y){
		if(date2->m > date1->m){
			return 1;
		}else if(date2->m == date1->m){
			if(date2->d >= date1->d){
				return 1;
			}else{
				return 0;
			}
		}else{
			return 0;
		}
	}else{
		return 0;
	}
}


void timeStr::fill(int a, int b){
	h = a;
	m = b;
}

int timeStr::addTime(char *word){
	if(checkIsTime(word)){
		word = strtok(word, ":\n\r");
		h = atoi(word);

		word = strtok(NULL, ":\n\r");
		m = atoi(word);

		return 0;
	}else{
		return -1;
	}

}

//If time2 >= time1 return 1, else return 0
int compareTimes(timeStr *time1, timeStr *time2){
	if(time2->h > time1->h){
		return 1;
	}else if(time2->h == time1->h){
		if(time2->m >= time1->m){
			return 1;
		}else{
			return 0;
		}
	}else{
		return 0;
	}
}


int  transaction::addDate(char *word){
	return date.addDate(word);
}

int  transaction::addTime(char *word){
	return time.addTime(word);
}

int transaction::fill(char *line, walletList *wallet, int flagTr){
	char *temp  = NULL;
	char *temp2 = NULL;
	char *temp3 = NULL;

	//ID
	temp  = strtok(line, " \n\r");
	if(flagTr == 1 || flagTr ==2){
		//generate random id
		transactionID = 0;

	}else{
		transactionID = atoi(temp);
		temp = strtok(NULL, " \n\r");
	}


	//senderID
	fromWalletID = &wallet->findUser(temp)->usID;
	if(fromWalletID == NULL)
		return -1;


	//receiverID
	temp = strtok(NULL, " \n\r");
	toWalletID = &wallet->findUser(temp)->usID;
	if(toWalletID == NULL)
		return -1;


	//value
	temp = strtok(NULL, " ;\n\r");
	value = atoi(temp);
	if( !wallet->findUser( fromWalletID->id )->checkBalance( value ) )
		return -1;

	if(flagTr != 2){
		//date and time
		temp   = strtok(NULL, " ;\n\r");
		temp2  = strtok(NULL, " ;\n\r");
		temp3  = strtok(NULL, ";\n\r");

		if(addDate(temp))
			return -1;

		if(addTime(temp2))
			return -1;
	}

	return 0;
}

void transaction::copyTransaction(transaction *des){
	transactionID = des->transactionID;
	fromWalletID = des->fromWalletID;
	toWalletID = des->toWalletID;
	value = des->value;
	date.d = des->date.d;
	date.m = des->date.m;
	date.y = des->date.y;
	time.h = des->time.h;
	time.m = des->time.m;
}

void transaction::print(){
	cout << transactionID << " ";
	cout << fromWalletID->id << " ";
	cout << toWalletID->id << " ";
	cout << value << " ";
	cout << date.d << "/";
	cout << date.m << "/";
	cout << date.y << " ";
	cout << time.h << ":";
	cout << time.m << endl;
}

void transaction::print(timeStr *time1, timeStr *time2){
	if(compareTimes(time1, &time) && compareTimes(&time, time2)){
		cout << transactionID << " ";
		cout << fromWalletID->id << " ";
		cout << toWalletID->id << " ";
		cout << value << " ";
		cout << date.d << "/";
		cout << date.m << "/";
		cout << date.y << " ";
		cout << time.h << ":";
		cout << time.m << endl;
	}
}

void transaction::print(dateStr *date1, dateStr *date2){
	if(compareDates(date1, &date) && compareDates(&date, date2)){
		cout << transactionID << " ";
		cout << fromWalletID->id << " ";
		cout << toWalletID->id << " ";
		cout << value << " ";
		cout << date.d << "/";
		cout << date.m << "/";
		cout << date.y << " ";
		cout << time.h << ":";
		cout << time.m << endl;
	}
}

void transaction::print(timeStr *time1, timeStr *time2, dateStr *date1, dateStr *date2){
	if(compareTimes(time1, &time) && compareTimes(&time, time2) &&
	   compareDates(date1, &date) && compareDates(&date, date2)){
		cout << transactionID << " ";
		cout << fromWalletID->id << " ";
		cout << toWalletID->id << " ";
		cout << value << " ";
		cout << date.d << "/";
		cout << date.m << "/";
		cout << date.y << " ";
		cout << time.h << ":";
		cout << time.m << endl;
	}
}


