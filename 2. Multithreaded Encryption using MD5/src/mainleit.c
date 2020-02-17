#include <sys/wait.h>
#include "functions.h"
#include "sem.h"
#include "shm.h"
#include <openssl/md5.h>

int enable = 1;
int t = 0;
int K = 13;

int main(int argc, char *argv[]){
	char line[256], newline[256], exitMes[256];
	char readStrR[256], copy2[256];

	FILE *fp;
	size_t len = 0; ssize_t read;
	int chNum = 0, i, x, j, N = 0;
	pid_t pid;

	// First make sure we have the correct amount of cmdl arguments (which is 1)
	if(argc!=2) printCmdlErr(argc);

	// Set the amount of P processes
	N = atoi(argv[1]);

	//Semaphores
	getSemaphoreSet(); 		// Get a semaphore set
	initializeSemaphoreSet(); 	// Initialize said set

	//Shared Memory
	in_ds  = getSharedMemory(); // Get shared memory
	out_ds = getSharedMemory(); // Get shared memory

	void * shmemAdd1 = attachSharedMemory(in_ds); 	// Attach shm1
	void * shmemAdd2 = attachSharedMemory(out_ds); 	// Attach shm2


	// P process section
	int pid_match = 0;
	for(i=0; i < N; i++){
		if(fork() == 0){
			srand(getpid());
			while(1){
				//Create message and write it on shared memory segment in-ds
				semdown(psRequest);
					semdown(mutex1);
						readlinefromfile(line);
						snprintf(newline, 256, "%d %s", getpid(), line);
						strcpy((char*)(shmemAdd1), newline);
					sleep(t);
					semup__(mutex1);
				semup__(shm1isSet);

				if(enable){
					int randomNumber = rand()%2;
					sleep(randomNumber);
				}

				//Read message from out-ds and print it
				semdown(shm2isSet);
					semdown(mutex2);
						strcpy(readStrR, (char*)(shmemAdd2)); strcpy(copy2, readStrR);

						char *token = strtok(readStrR, " ");
						// If P reads "Exit" that means that C has stopped and all P processes need to stop as well
						if(!strcmp(token,"Exit")){
							snprintf(exitMes, 256, "Exit %d", pid_match);
							strcpy((char*)(shmemAdd2), exitMes); //sleep(1);
							semup__(mutex2);

							semup__(shmIsSetForParent);
							semdown(parentRead);

							semup__(psRequest);
							semup__(shm2isSet);
							semup__(inRequest);
							break;
						}

						int messagePid = strtol(token, NULL, 10);
						token = strtok(NULL, " ");
						token = strtok(NULL, "\n\t");



						if(messagePid==getpid()){
							pid_match++;
							printf("%d| %s\n", getpid(), token);
						}else{
							printf("%d| %d| %s\n", getpid(), messagePid, token);
						}
					sleep(t);
					semup__(mutex2);
				semup__(inRequest);
			}

			printf("P Exiting %d\n", getpid());
			exit(0);
		}
	}

	// C process section
	pid = fork(); if(pid < 0) { perror("Error in fork function.\n"); exit(1); }
	if(pid == 0){
		int steps = 0;
		char readstr[256], readcopy[256], newrdcp[256];

		//Read message from in-ds and add md5 to it
		for(x = 0; x < K; x++){
			semdown(shm1isSet);
				steps++;
				semdown(mutex1);
					strcpy(readstr, (char *)(shmemAdd1));
					strcpy(readcopy, readstr);

					char *token = strtok(readstr, " ");
					token = strtok(NULL, "-");

					unsigned char digest[MD5_DIGEST_LENGTH];
					char mdStr[33];
					MD5((unsigned char*)token, strlen(token), (unsigned char*)&digest);
					for(int i = 0; i < 16; i++)
						sprintf(&mdStr[i*2], "%02x", (unsigned int)digest[i]);
					snprintf(newrdcp, 256, "%s %s %s", readstr, mdStr, token);
				sleep(t);
				semup__(mutex1);
			semup__(psRequest);

			//Write message on shared memory segment out-ds
			semdown(inRequest);
				semdown(mutex2);
					strcpy((char*)(shmemAdd2), newrdcp);
				sleep(t);
				semup__(mutex2);
			semup__(shm2isSet); // Send to Inspector1
		}

		//C is done
		semdown(shm1isSet);
		semup__(psRequest);

		semdown(inRequest); // Write on Shared Memory 2
			semdown(mutex2);
				char mess[256];
				snprintf(mess, 256, "C %d", steps);
				strcpy((char*)(shmemAdd2), mess);
			semup__(mutex2);

				semup__(shmIsSetForParent);
				semdown(parentRead);

			semdown(mutex2);
				strcpy((char*)(shmemAdd2), "Exit");
			semup__(mutex2);
		semup__(shm2isSet); // Send to Inspector1

		printf("C Exiting %d\n", getpid());
		exit(0);
	}


	// Parent process section
	int pidCount = 0, stepCount = 0;
	while(1){
		//Either reads from C when it's finished or when a P process is finished
		char readPar[256];
		semdown(shmIsSetForParent);
			semdown(mutex2);
				strcpy(readPar, (char *)(shmemAdd2));
				char *tokenS = strtok(readPar, " ");
				if(!strcmp(tokenS, "C")){
					tokenS = strtok(NULL, " ");
					int pp = strtol(tokenS, NULL, 10);
					stepCount=pp;
					tokenS = strtok(NULL, "\n\t");
				}else if(!strcmp(tokenS, "Exit")){
					tokenS = strtok(NULL, " ");
					int pp = strtol(tokenS, NULL, 10);
					pidCount+=pp;
					tokenS = strtok(NULL, "\n\t");
					chNum++;
				}
			sleep(t);
			semup__(mutex2);
		semup__(parentRead); // Open Requests
		if(chNum==N) break;
	}

	int stat_val; pid_t child_pid;
	while ((child_pid = wait(&stat_val)) > 0);

	// Delete Semaphore Set
	deleteSemaphores();

	// Delete Shared Memories
	deleteSharedMemory();

	printf("P process count: %d\n", chNum);
	printf("Step count: %d\n", stepCount);
	printf("pid_match count: %d\n", pidCount);
	exit(0);
}
