#include "sem.h"

//Semaphores
int sem_Count = 8;
int semid = 0;

int psRequest = 0;
int inRequest = 1;
int shm1isSet = 2;
int shm2isSet= 3;
int shmIsSetForParent = 4;
int parentRead = 5;
int mutex1 = 6;
int mutex2 = 7;



// Semaphores //
void semup__(int loc){
	struct sembuf semup = {loc, 1, 0};
	if( (semop(semid, &semup, 1)) < 0 )
	{
		char message[256];
		snprintf(message, 256, "Error in semup %d %d", getpid(), loc);
		perror(message); exit(1);
	}
}

void semdown(int loc){
	struct sembuf semdown = {loc, -1, 0};
	if( (semop(semid, &semdown, 1)) < 0 )
	{
		char message[256];
		snprintf(message, 256, "Error in semdown %d %d", getpid(), loc);
		perror(message); exit(1);
	}
}

void getSemaphoreSet(){
	semid = semget(IPC_PRIVATE, sem_Count, IPC_CREAT | IPC_EXCL | 0600);
		if (semid < 0) { perror("Could not create semaphore"); exit(1); }
}

void initializeSemaphoreSet(){
	union semun setSem; setSem.val = 1;
	if( (semctl(semid, psRequest, SETVAL, setSem)) < 0)
		{ perror("Could not set value of semaphore.\n"); exit(1); }
	if( (semctl(semid, inRequest, SETVAL, setSem)) < 0)
			{ perror("Could not set value of semaphore.\n"); exit(1); }
	if( (semctl(semid, mutex1, SETVAL, setSem)) < 0)
		{ perror("Could not set value of semaphore.\n"); exit(1); }
	if( (semctl(semid, mutex2, SETVAL, setSem)) < 0)
			{ perror("Could not set value of semaphore.\n"); exit(1); }

	setSem.val = 0;
	if( (semctl(semid, shm1isSet, SETVAL, setSem)) < 0)
			{ perror("Could not set value of semaphore.\n"); exit(1); }
	if( (semctl(semid, shm2isSet, SETVAL, setSem)) < 0)
			{ perror("Could not set value of semaphore.\n"); exit(1); }
	if( (semctl(semid, shmIsSetForParent, SETVAL, setSem)) < 0)
			{ perror("Could not set value of semaphore.\n"); exit(1); }
	if( (semctl(semid, parentRead, SETVAL, setSem)) < 0)
			{ perror("Could not set value of semaphore.\n"); exit(1); }
}

void deleteSemaphores(){
	if (semctl(semid, 0, IPC_RMID) < 0)
		fprintf(stderr, "Failed to delete semaphore with id:%d\n", semid);
}

