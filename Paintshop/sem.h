#ifndef sem_h
#define sem_h

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include <unistd.h>
#include <errno.h>
#include <string.h>

union semun {
	int val;
	struct semid_ds *buf;
	unsigned short *array;
};

//Semaphores
extern int sem_Count;
extern int psRequest;
extern int inRequest;
extern int asRequest;
extern int shm1isSet;
extern int shm2isSet1;
extern int shm2isSet2;
extern int shm2isSet3;
extern int shm3isSet;
extern int mutex1;
extern int mutex2;
extern int mutex3;
extern int semid;

// Semaphores //
void semup__(int);
void semdown(int);
void getSemaphoreSet();
void initializeSemaphoreSet();
void deleteSemaphores();

#endif
