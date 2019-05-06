#include "shm.h"

int shmid1 = 0;
int shmid2 = 0;
int shmid3 = 0;
void * shmemAdd1 = NULL;
void * shmemAdd2 = NULL;
void * shmemAdd3 = NULL;

void setComponent(Component *comp, int id, char type, stateEnum state, double timestamps, double timestampns){
	comp->id		= id;
	comp->type		= type;
	comp->state		= state;
	comp->timestampsec	= timestamps;
	comp->timestampnsec	= timestampns;
}

// Shared Memory //
int getSharedMemory(){
	int id = shmget(IPC_PRIVATE, sizeof(Component), IPC_CREAT | 0600 );
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
	shmctl (shmid1, IPC_RMID, 0);
	shmctl (shmid2, IPC_RMID, 0);
	shmctl (shmid3, IPC_RMID, 0);
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

