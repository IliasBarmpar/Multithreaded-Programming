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

extern int in_ds;
extern int out_ds;
extern void * shmemAdd1;
extern void * shmemAdd2;

// Shared Memory //
int getSharedMemory();
void *attachSharedMemory(int );
void deleteSharedMemory();

// Shared Memory Setters / Getters //
int getShmId(char * shmemAdd);
char getShmType(char *shmemAdd);
void setShmData(char *shmemAdd, char *name, void *value);



#endif
