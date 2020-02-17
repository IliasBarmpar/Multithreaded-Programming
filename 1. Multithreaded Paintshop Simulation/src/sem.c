#include "sem.h"

//Semaphores
int sem_Count = 11;
int psRequest = 0;
int inRequest = 1;
int asRequest = 2;
int shm1isSet = 3;
int shm2isSet1= 4;
int shm2isSet2= 5;
int shm2isSet3= 6;
int shm3isSet = 7;
int mutex1 = 8;
int mutex2 = 9;
int mutex3 = 10;
int semid = 0;

// Semaphores //
void semup__(int loc){
	struct sembuf semup = {loc, 1, 0};
	if( (semop(semid, &semup, 1)) < 0 )
		{ perror("Error in semup.\n"); exit(1); }
}

void semdown(int loc){
	struct sembuf semdown = {loc, -1, 0};
	if( (semop(semid, &semdown, 1)) < 0 )
		{ perror("Error in semdown.\n"); exit(1); }
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
	if( (semctl(semid, asRequest, SETVAL, setSem)) < 0)
			{ perror("Could not set value of semaphore.\n"); exit(1); }
	if( (semctl(semid, mutex1, SETVAL, setSem)) < 0)
		{ perror("Could not set value of semaphore.\n"); exit(1); }
	if( (semctl(semid, mutex2, SETVAL, setSem)) < 0)
			{ perror("Could not set value of semaphore.\n"); exit(1); }
	if( (semctl(semid, mutex3, SETVAL, setSem)) < 0)
			{ perror("Could not set value of semaphore.\n"); exit(1); }

	setSem.val = 0;
	if( (semctl(semid, shm1isSet, SETVAL, setSem)) < 0)
			{ perror("Could not set value of semaphore.\n"); exit(1); }
	if( (semctl(semid, shm2isSet1, SETVAL, setSem)) < 0)
			{ perror("Could not set value of semaphore.\n"); exit(1); }
	if( (semctl(semid, shm2isSet2, SETVAL, setSem)) < 0)
			{ perror("Could not set value of semaphore.\n"); exit(1); }
	if( (semctl(semid, shm2isSet3, SETVAL, setSem)) < 0)
			{ perror("Could not set value of semaphore.\n"); exit(1); }
	if( (semctl(semid, shm3isSet, SETVAL, setSem)) < 0)
			{ perror("Could not set value of semaphore.\n"); exit(1); }
}

void deleteSemaphores(){
	if (semctl(semid, 0, IPC_RMID) < 0)
		fprintf(stderr, "Failed to delete semaphore with id:%d\n", semid);
}

