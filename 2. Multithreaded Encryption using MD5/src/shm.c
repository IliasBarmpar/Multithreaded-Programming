#include "shm.h"

int in_ds = 0;
int out_ds = 0;
void * shmemAdd1 = NULL;
void * shmemAdd2 = NULL;

// Shared Memory //
int getSharedMemory(){
	int id = shmget(IPC_PRIVATE, 256, IPC_CREAT | 0600 );
		if (id < 0) { perror("Could not create shared memory.\n"); exit(1); }
	return id;
}

void *attachSharedMemory(int id){
	char *tempshmPtr = NULL;
	tempshmPtr = shmat(id, NULL, 0);
		if (tempshmPtr == (char *)-1) { perror("Could not attach shared memory.\n"); exit(1); }
	return (void*)tempshmPtr;
}

void deleteSharedMemory(){
	shmctl (in_ds, IPC_RMID, 0);
	shmctl (out_ds, IPC_RMID, 0);
}

// Shared Memory Setters / Getters //
int getShmId(char * shmemAdd){
	return *((int *)shmemAdd);
}

char getShmType(char *shmemAdd){
	return *((char *)shmemAdd + sizeof(int));
}

void setShmData(char *shmemAdd, char *name, void *value){
	if(!strcmp(name, "id"))
		*((int *)shmemAdd) = *((int *)value) ;
	if(!strcmp(name, "type"))
		*((char *)shmemAdd + sizeof(int)) = *((char *)value) ;
	if(!strcmp(name, "state"))
		return ;
}

