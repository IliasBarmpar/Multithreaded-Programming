#ifndef shm_h
#define shm_h

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <unistd.h>
#include <errno.h>
#include <string.h>

typedef enum { CREATED, PAINTED, INSPECTED, ASSEMBLED} stateEnum;

typedef struct component{
	int id;
	char type;
	stateEnum state;
	double timestampsec;
	double timestampnsec;
	double waitForPs;
}Component;

extern int shmid1;
extern int shmid2;
extern int shmid3;
extern void * shmemAdd1;
extern void * shmemAdd2;
extern void * shmemAdd3;

void setComponent(Component *, int , char , stateEnum , double , double );

// Shared Memory //
int getSharedMemory();
void *attachSharedMemory(int );
void deleteSharedMemory();

// Shared Memory Setters / Getters //
int getShmId(char * shmemAdd);
char getShmType(char *shmemAdd);
void setShmData(char *shmemAdd, char *name, void *value);



#endif
