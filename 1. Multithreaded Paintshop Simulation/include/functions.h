#ifndef guf_h
#define guf_h

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

extern int speedup;

// Finds the earliest timestamp from the three provided (from the parts),
// and then proceeds to calculate the time difference of the
// earliest timestamp with the products timestamp
void findMinTs(double *, double , double , double , double , double , double , double , double );


// Used by constructors to calculate time waiting for paint shop
// works exactly like findMinTs but with two timestamps instead of three.
double getTimeDifferenceGivenStructTimespec(struct timespec *, struct timespec *);

// We form an id by getting each integer one by one and placing it in the correct spot
// in our char array (newid) (type ‘c’ id, followed by, type ‘b’ id, followed by type ‘a’ id)
void formID(char *, int , int , int );

// Simply return a random 4 digit number
int addID();

// A simple function that returns nanoseconds depending on the part type for the paint shop to use for it's nanosleep
struct timespec getPsTime(char );

// A simple function that returns nanoseconds depending on the part type for an inspector to use for his nanosleep
struct timespec getInTime(char );

// Error printer for cmdl arguments
void printCmdlErr(int );

#endif
