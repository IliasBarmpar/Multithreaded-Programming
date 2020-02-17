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
extern int semid;

extern int psRequest;
extern int inRequest;
extern int shm1isSet;
extern int shm2isSet;
extern int shmIsSetForParent;
extern int parentRead;
extern int mutex1;
extern int mutex2;


// Semaphores //
void semup__(int);
void semdown(int);
void getSemaphoreSet();
void initializeSemaphoreSet();
void deleteSemaphores();

#endif
