#include "functions.hpp"

void printError1(){
	cout << "Error: Command needs to follow a specific structure." << endl;
//	cout << "'/requestTransaction senderWalletID receiverWalletID' or '/requestTransaction senderWalletID receiverWalletID amount date time'." <<endl;
}

//Calculates how many words there are in a given string
void calcWordCount(int *wordCount, char *word){
	int i = 0, flag = 0;

	for(; word[i] != '\0'; i++){
		if(word[i]!=' ')
			flag = 1;

		if(word[i]==' '){
			if(flag){
				(*wordCount)++;
				flag = 0;
			}
		}
	}
	if(word[i-1]==' ') (*wordCount)--;

	(*wordCount)++;
}

//Calculates how many spaces there are in a given string
void calcSpaceCount(int *spaceCount, char *word){
	int i = 0;
	for(; word[i] != '\0'; i++)
		if(word[i]==' ')
			(*spaceCount)++;
	for(int j = i-1; word[j]==' '; j--)
		(*spaceCount)--;
}
