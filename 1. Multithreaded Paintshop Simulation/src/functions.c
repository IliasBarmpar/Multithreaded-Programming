#include "functions.h"

int speedup = 100;

// Finds the earliest timestamp from the three provided (from the parts),
// and then proceeds to calculate the time difference of the
// earliest timestamp with the products timestamp
void findMinTs(double *temp, double fs, double fns, double tss1, double tss2, double tss3, double tsns1, double tsns2, double tsns3){
	double m, min, ts1, ts2, ts3;
	// Each timestamp has seconds and nanoseconds, our goal is to lose as little
	// nanosecond precision as possible, so...

	// First find the timestamp with the least seconds and subtract it from all other timestamps
	m = tss1;
	if(m > tss2)
		m = tss2;
	if(m > tss3)
		m = tss3;
	tss1 -= m;
	tss2 -= m;
	tss3 -= m;
	fs   -= m;

	// Now that we've lowered the seconds significantly (we'll have 0 or 1 second)
	// we multiply to get our timestamps in nanoseconds
	ts1 = tss1*1000000.0f + tsns1/1000.0f;
	ts2 = tss2*1000000.0f + tsns2/1000.0f;
	ts3 = tss3*1000000.0f + tsns3/1000.0f;

	// Find the earliest timestamp, then calculcate the time difference with
	// the product's timestamp, cast to milliseconds and return
	min = ts1;
	if(min < ts2 && min < ts3){
		*temp = (double)(fs*1000000000.0f + fns) - (double)(tss1*1000000000.0f + tsns1);
		*temp = (*temp)/1000000.0f;
	}else if(ts2 < ts3){
		*temp = (double)(fs*1000000000.0f + fns) - (double)(tss2*1000000000.0f + tsns2);
		*temp = (*temp)/1000000.0f;
	}else{
		*temp = (double)(fs*1000000000.0f + fns) - (double)(tss3*1000000000.0f + tsns3);
		*temp = (*temp)/1000000.0f;
	}
}

// Used by constructors to calculate time waiting for paint shop
// works exactly like findMinTs but with two timestamps instead of three.
double getTimeDifferenceGivenStructTimespec(struct timespec *start, struct timespec *end){
	double temps1, temps2, ts1, ts2;

	// Find the least seconds (of the two timestamps) and then subtruct that from both
	temps1 = (double)(start->tv_sec);
	temps2 = (double)(end->tv_sec);
	if(temps1 < temps2){
		temps2 = temps2 - temps1;
		temps1 = temps1 - temps1;
	}else{
		temps1 = temps1 - temps2;
		temps2 = temps2 - temps2;
	}

	// Cast the timestamp to nanoseconds, subtruct, cast our double to milliseconds and return
	ts1 = temps1*1000000000.0f + (double)(start->tv_nsec);
	ts2 = temps2*1000000000.0f + (double)(end->tv_nsec);
	ts2 = ts2 - ts1;
	ts2 = ts2/1000000.0f;
	return ts2;
}

// We form an id by getting each integer one by one and placing it in the correct spot
// in our char array (newid) (type ‘c’ id, followed by, type ‘b’ id, followed by type ‘a’ id)
void formID(char *newid, int id1, int id2, int id3){
	int i = 0;
	int a, b, c, mod = 10;
	for(; i < 4; i++){
		a = (id1 % mod)/(mod/10);
		b = (id2 % mod)/(mod/10);
		c = (id3 % mod)/(mod/10);
		id1 -= a;
		id2 -= b;
		id3 -= c;
		mod = mod*10;
		newid[11-i] = a + '0';
		newid[7-i]  = b + '0';
	    newid[3-i]  = c + '0';
	}
}

// Simply return a random 4 digit number
int addID(){
	return rand()%9000+1000;
}

// A simple function that returns nanoseconds depending on the part type for the paint shop to use for it's nanosleep
struct timespec getPsTime(char type){
	struct timespec temp;
	temp.tv_sec = 0;
	if(type == 'a'){
		temp.tv_nsec = 12*speedup*1000000L;
	}else if(type == 'b'){
		temp.tv_nsec = 8*speedup*1000000L;
	}else if(type == 'c'){
		temp.tv_nsec = 16*speedup*1000000L;
	}else{
		//error
	}
	return temp;
}

// A simple function that returns nanoseconds depending on the part type for an inspector to use for his nanosleep
struct timespec getInTime(char type){
	struct timespec temp;
	temp.tv_sec = 0;
	if(type == 'a'){
		temp.tv_nsec = 20*speedup*1000000L;
	}else if(type == 'b'){
		temp.tv_nsec = 5*speedup*1000000L;
	}else if(type == 'c'){
		temp.tv_nsec = 13*speedup*1000000L;
	}else{
		//error
	}
	return temp;
}

// Error printer for cmdl arguments
void printCmdlErr(int argc){
	if(argc > 2)
		printf("Please make sure you only pass the number of components as a command line argument and nothing else.\n");
	else
		printf("Please make sure you pass the number of components as a command line argument and nothing else.\n");
	exit(1);
}

