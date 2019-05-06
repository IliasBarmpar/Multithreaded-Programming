#include <sys/wait.h>
#include "functions.h"
#include "sem.h"
#include "shm.h"
#include "myLists.h"

int main(int argc, char *argv[]){
	int j, productSum = 0, productComponents = 0;
	pid_t pid;
	// First make sure you have the correct ammount of cmdl arguments (which is 1)
	if(argc!=2) printCmdlErr(argc);

	// Set the ammount of products and parts per product
	productSum = atoi(argv[1]);
	productComponents = 3;

	//Semaphores
	getSemaphoreSet(); 				// Get a semaphore set
	initializeSemaphoreSet(); 	// Initialize said set

	//Shared Memory
	shmid1 = getSharedMemory(); // Get shared memory
	shmid2 = getSharedMemory(); // Get shared memory
	shmid3 = getSharedMemory(); // Get shared memory

	// Constructor1
	pid = fork(); if(pid < 0) { perror("Error in fork function.\n"); exit(1); }
	if(pid == 0){
		// Initialization
		Component comp;
		srand(getpid());
		void * shmemAdd1 = attachSharedMemory(shmid1);
		struct timespec start, end, timeA = {0, 3*speedup*1000000L};

		// Production
		for(j=0; j < productSum; ++j){
			nanosleep(&timeA, NULL); // Creating Component

			clock_gettime(CLOCK_MONOTONIC_RAW , &start);
			setComponent(&comp,addID(),'a', CREATED, (double)(start.tv_sec), (double)(start.tv_nsec));

			semdown(psRequest); // Request PaintShop
				semdown(mutex1);
					clock_gettime(CLOCK_MONOTONIC_RAW , &end);
					// Write on Shared Memory 1
					*(int *)(shmemAdd1) 													= comp.id;
					*(char*)(shmemAdd1 + sizeof(int)) 										= comp.type;
					*(int *)(shmemAdd1 + sizeof(int) + sizeof(char))						= comp.state;
					*(double *)(shmemAdd1 + 2*sizeof(int) + sizeof(char))					= comp.timestampsec;
					*(double *)(shmemAdd1 + 2*sizeof(int) + sizeof(char)+sizeof(double))	= comp.timestampnsec;
					*(double *)(shmemAdd1 + 2*sizeof(int) + sizeof(char) +2*sizeof(double))	= getTimeDifferenceGivenStructTimespec(&start, &end);
				semup__(mutex1);
			semup__(shm1isSet); // Send to PaintShop
		}
		exit(0);
	}


	// PaintShop
	pid = fork(); if(pid < 0) { perror("Error in fork function.\n"); exit(1); }
	if(pid == 0){
		// Initialization
		void * shmemAdd1 = attachSharedMemory(shmid1);
		void * shmemAdd2 = attachSharedMemory(shmid2);
		struct timespec timeps;
		Component comp;

		// Painting
		for(j=0; j < productComponents*productSum; ++j){
			// Read Item
			semdown(shm1isSet);
				semdown(mutex1);
					// Read Shared Memory 1
					comp.id				= *(int *)(shmemAdd1);
					comp.type			= *(char*)(shmemAdd1 + sizeof(int));
					comp.state			= *(int *)(shmemAdd1 + sizeof(int) + sizeof(char));
					comp.timestampsec	= *(double *)(shmemAdd1 + 2*sizeof(int) + sizeof(char));
					comp.timestampnsec	= *(double *)(shmemAdd1 + 2*sizeof(int) + sizeof(char) + sizeof(double));
					comp.waitForPs		= *(double *)(shmemAdd1 + 2*sizeof(int) + sizeof(char) +2*sizeof(double));
					timeps = getPsTime(comp.type); //For extra safety let's keep it within the semaphores
				semup__(mutex1);
			semup__(psRequest); // Open Requests

			nanosleep(&timeps, NULL); // Painting

			semdown(inRequest); // Request Inspector
				semdown(mutex2);
					// Write on Shared Memory 2
					*(int *)(shmemAdd2) 							 						= comp.id;
					*(char*)(shmemAdd2 + sizeof(int))				 						= comp.type;
					*(int *)(shmemAdd2 + sizeof(int) + sizeof(char)) 						= PAINTED;
					*(double *)(shmemAdd2 + 2*sizeof(int) + sizeof(char))					= comp.timestampsec;
					*(double *)(shmemAdd2 + 2*sizeof(int) + sizeof(char) +sizeof(double))   = comp.timestampnsec;;
					*(double *)(shmemAdd2 + 2*sizeof(int) + sizeof(char) +2*sizeof(double))	= comp.waitForPs;
				semup__(mutex2);
			if(comp.type=='a')
				semup__(shm2isSet1); // Send to Inspector1
			if(comp.type=='b')
				semup__(shm2isSet2); // Send to Inspector1
			if(comp.type=='c')
				semup__(shm2isSet3); // Send to Inspector1
		}
		exit(0);
	}

	// Inspector1
	pid = fork(); if(pid < 0) { perror("Error in fork function.\n"); exit(1); }
	if(pid == 0){
		// Initialization
		void * shmemAdd2 = attachSharedMemory(shmid2);
		void * shmemAdd3 = attachSharedMemory(shmid3);
		struct timespec timein;
		Component comp;

		// Inspecting
		for(j=0; j < productSum; ++j){
			semdown(shm2isSet1); // Get Item
				semdown(mutex2);
					// Read Shared Memory 2
					comp.id			= *(int *)(shmemAdd2);
					comp.type		= *(char*)(shmemAdd2 + sizeof(int));
					comp.state		= *(int *)(shmemAdd2 + sizeof(int) + sizeof(char));
					comp.timestampsec	= *(double *)(shmemAdd2 + 2*sizeof(int) + sizeof(char));
					comp.timestampnsec	= *(double *)(shmemAdd2 + 2*sizeof(int) + sizeof(char) + sizeof(double));
					comp.waitForPs	= *(double *)(shmemAdd2 + 2*sizeof(int) + sizeof(char) +2*sizeof(double));
					timein = getInTime(comp.type); //For extra safety let's keep it within the semaphores
				semup__(mutex2);
			semup__(inRequest); // Open Requests

			nanosleep(&timein, NULL);	// Inspect

			semdown(asRequest); // Request Assembler
				semdown(mutex3);
					// Write on Shared Memory3
					*(int *)(shmemAdd3) 							 						= comp.id;
					*(char*)(shmemAdd3 + sizeof(int)) 				 						= comp.type;
					*(int *)(shmemAdd3 + sizeof(int) + sizeof(char))						= INSPECTED;
					*(double *)(shmemAdd3 + 2*sizeof(int) + sizeof(char))					= comp.timestampsec;
					*(double *)(shmemAdd3 + 2*sizeof(int) + sizeof(char) +sizeof(double))   = comp.timestampnsec;;
					*(double *)(shmemAdd3 + 2*sizeof(int) + sizeof(char) +2*sizeof(double))	= comp.waitForPs;
				semup__(mutex3);
			semup__(shm3isSet); // Send to Assembler
		}
		exit(0);
	}

	// Assembler
	pid = fork(); if(pid < 0) { perror("Error in fork function.\n"); exit(1); }
	if(pid == 0){
		// Initialization
		long double waitSum = 0.0;
		void * shmemAdd3 = attachSharedMemory(shmid3);
		struct timespec stop, timein = {0, 25*speedup*1000000L};
		Component comp;
		myListPtr list1 = NULL;				// Type 'a' part list
		myListPtr list2 = NULL;				// Type 'b' part list
		myListPtr list3 = NULL;				// Type 'c' part list
		myProductListPtr products = NULL;	// Product list

		// Assembling
		for(j=0; j < productComponents*productSum; ++j){
			 // Read Item
			semdown(shm3isSet);
				semdown(mutex3);
					// Read Shared Memory 3
					comp.id			= *(int *)(shmemAdd3);
					comp.type		= *(char*)(shmemAdd3 + sizeof(int));
					comp.state		= *(int *)(shmemAdd3 + sizeof(int) + sizeof(char));
					comp.timestampsec	= *(double *)(shmemAdd3 + 2*sizeof(int) + sizeof(char));
					comp.timestampnsec	= *(double *)(shmemAdd3 + 2*sizeof(int) + sizeof(char) + sizeof(double));
					comp.waitForPs	= *(double *)(shmemAdd3 + 2*sizeof(int) + sizeof(char) +2*sizeof(double));
				semup__(mutex3);
			semup__(asRequest); // Open Requests

			// Add to list
			if(comp.type=='a'){
				list1 = addNode(list1, comp);
			}else if(comp.type=='b'){
				list2 = addNode(list2, comp);
			}else if(comp.type=='c'){
				list3 = addNode(list3, comp);
			}

			// If there is one of each, make a product.
			if(list1 != NULL && list2 != NULL && list3 != NULL){
				char newid[12];
				int id1, id2, id3;
				double difference, tSsec1, tSsec2, tSsec3,  tSnsec1, tSnsec2, tSnsec3,psWait1, psWait2, psWait3;

				// Remove a node from each list in a FIFO fashion while making sure to get info necessary
				// needed for upcoming calculations (we get id, timestamp seconds, timestamp nanoseconds, wait for paint shop)
				list1 = removeNode(list1, &id1, &tSsec1,  &tSnsec1, &psWait1);
				list2 = removeNode(list2, &id2, &tSsec2,  &tSnsec2, &psWait2);
				list3 = removeNode(list3, &id3, &tSsec3,  &tSnsec3, &psWait3);

				// Keep a sum of all waiting periods and we make a new ID for our product
				waitSum = waitSum + psWait1 + psWait2 + psWait3;
				formID(newid, id1, id2, id3);

				// Assemble
				nanosleep(&timein, NULL);
				clock_gettime(CLOCK_MONOTONIC_RAW , &stop);

				// Calculating time difference and adding product to the product list
				findMinTs(&difference, (double)(stop.tv_sec), (double)(stop.tv_nsec), tSsec1, tSsec2, tSsec3, tSnsec1, tSnsec2, tSnsec3);
				products = addPrNode(products, newid, difference);
			}
		}

		//Prints
		printf("Average time for a part to get into the paintshop: %0.1Lfms (%d)\n", waitSum/(productComponents*productSum), productComponents*productSum);
		printPrProductMean(products);
		//printPrList(products);
		deletePrList(products);
		exit(0);
	}

	// Constructor2
	pid = fork(); if(pid < 0) { perror("Error in fork function.\n"); exit(1); }
	if(pid == 0){
		// Initialization
		Component comp;
		srand(getpid());
		void * shmemAdd1 = attachSharedMemory(shmid1);
		struct timespec start, end, timeB = {0, 7*speedup*1000000L};

		// Production
		for(j=0; j < productSum; ++j){
			nanosleep(&timeB, NULL); // Creating Component

			clock_gettime(CLOCK_MONOTONIC_RAW , &start);
			setComponent(&comp,addID(),'b', CREATED, (double)(start.tv_sec), (double)(start.tv_nsec));

			semdown(psRequest); // Request PaintShop
				semdown(mutex1);
					clock_gettime(CLOCK_MONOTONIC_RAW , &end);
					// Write on Shared Memory 1
					*(int *)(shmemAdd1) 													= comp.id;
					*(char*)(shmemAdd1 + sizeof(int)) 										= comp.type;
					*(int *)(shmemAdd1 + sizeof(int) + sizeof(char))						= comp.state;
					*(double *)(shmemAdd1 + 2*sizeof(int) + sizeof(char))					= comp.timestampsec;
					*(double *)(shmemAdd1 + 2*sizeof(int) + sizeof(char)+sizeof(double))	= comp.timestampnsec;
					*(double *)(shmemAdd1 + 2*sizeof(int) + sizeof(char) +2*sizeof(double))	= getTimeDifferenceGivenStructTimespec(&start, &end);
				semup__(mutex1);
			semup__(shm1isSet); // Send to PaintShop
		}
		exit(0);
	}

	// Constructor3
	pid = fork(); if(pid < 0) { perror("Error in fork function.\n"); exit(1); }
	if(pid == 0){
		// Initialization
		Component comp;
		srand(getpid());
		void * shmemAdd1 = attachSharedMemory(shmid1);
		struct timespec start, end, timeC = {0, 5*speedup*1000000L};

		// Production
		for(j=0; j < productSum; ++j){
			nanosleep(&timeC, NULL); // Creating Component

			clock_gettime(CLOCK_MONOTONIC_RAW , &start);
			setComponent(&comp,addID(),'c', CREATED, (double)(start.tv_sec), (double)(start.tv_nsec));

			semdown(psRequest); // Request PaintShop
				semdown(mutex1);
					clock_gettime(CLOCK_MONOTONIC_RAW , &end);
					// Write on Shared Memory 1
					*(int *)(shmemAdd1) 													= comp.id;
					*(char*)(shmemAdd1 + sizeof(int)) 										= comp.type;
					*(int *)(shmemAdd1 + sizeof(int) + sizeof(char))						= comp.state;
					*(double *)(shmemAdd1 + 2*sizeof(int) + sizeof(char))					= comp.timestampsec;
					*(double *)(shmemAdd1 + 2*sizeof(int) + sizeof(char)+sizeof(double))	= comp.timestampnsec;
					*(double *)(shmemAdd1 + 2*sizeof(int) + sizeof(char) +2*sizeof(double))	= getTimeDifferenceGivenStructTimespec(&start, &end);
				semup__(mutex1);
			semup__(shm1isSet);
		}
		exit(0);
	}

	// Inspector2
	pid = fork(); if(pid < 0) { perror("Error in fork function.\n"); exit(1); }
	if(pid == 0){
		// Initialization
		void * shmemAdd2 = attachSharedMemory(shmid2);
		void * shmemAdd3 = attachSharedMemory(shmid3);
		struct timespec timein;
		Component comp;

		// Inspecting
		for(j=0; j < productSum; ++j){
			semdown(shm2isSet2); // Get Item
				semdown(mutex2);
					// Read Shared Memory 2
					comp.id			= *(int *)(shmemAdd2);
					comp.type		= *(char*)(shmemAdd2 + sizeof(int));
					comp.state		= *(int *)(shmemAdd2 + sizeof(int) + sizeof(char));
					comp.timestampsec	= *(double *)(shmemAdd2 + 2*sizeof(int) + sizeof(char));
					comp.timestampnsec	= *(double *)(shmemAdd2 + 2*sizeof(int) + sizeof(char) + sizeof(double));
					comp.waitForPs	= *(double *)(shmemAdd2 + 2*sizeof(int) + sizeof(char) +2*sizeof(double));
					timein = getInTime(comp.type); //For extra safety let's keep it within the semaphores
				semup__(mutex2);
			semup__(inRequest); // Open Requests

			nanosleep(&timein, NULL);	// Inspect

			semdown(asRequest); // Request Assembler
				semdown(mutex3);
					// Write on Shared Memory3
					*(int *)(shmemAdd3) 							 						= comp.id;
					*(char*)(shmemAdd3 + sizeof(int)) 				 						= comp.type;
					*(int *)(shmemAdd3 + sizeof(int) + sizeof(char))						= INSPECTED;
					*(double *)(shmemAdd3 + 2*sizeof(int) + sizeof(char))					= comp.timestampsec;
					*(double *)(shmemAdd3 + 2*sizeof(int) + sizeof(char) +sizeof(double))   = comp.timestampnsec;;
					*(double *)(shmemAdd3 + 2*sizeof(int) + sizeof(char) +2*sizeof(double))	= comp.waitForPs;
				semup__(mutex3);
			semup__(shm3isSet); // Send to Assembler
		}
		exit(0);
	}

	// Inspector3
	pid = fork(); if(pid < 0) { perror("Error in fork function.\n"); exit(1); }
	if(pid == 0){
		// Initialization
		void * shmemAdd2 = attachSharedMemory(shmid2);
		void * shmemAdd3 = attachSharedMemory(shmid3);
		struct timespec timein;
		Component comp;

		// Inspecting
		for(j=0; j < productSum; ++j){
			// Get Item
			semdown(shm2isSet3);
				semdown(mutex2);
					// Read Shared Memory 2
					comp.id			= *(int *)(shmemAdd2);
					comp.type		= *(char*)(shmemAdd2 + sizeof(int));
					comp.state		= *(int *)(shmemAdd2 + sizeof(int) + sizeof(char));
					comp.timestampsec	= *(double *)(shmemAdd2 + 2*sizeof(int) + sizeof(char));
					comp.timestampnsec	= *(double *)(shmemAdd2 + 2*sizeof(int) + sizeof(char) + sizeof(double));
					comp.waitForPs	= *(double *)(shmemAdd2 + 2*sizeof(int) + sizeof(char) +2*sizeof(double));
					timein = getInTime(comp.type); //For extra safety let's keep it within the semaphores
				semup__(mutex2);
			semup__(inRequest); // Open Requests

			// Inspect
			nanosleep(&timein, NULL);

			semdown(asRequest); // Request Assembler
				semdown(mutex3);
					// Write on Shared Memory3
					*(int *)(shmemAdd3) 							 						= comp.id;
					*(char*)(shmemAdd3 + sizeof(int)) 				 						= comp.type;
					*(int *)(shmemAdd3 + sizeof(int) + sizeof(char))						= INSPECTED;
					*(double *)(shmemAdd3 + 2*sizeof(int) + sizeof(char))					= comp.timestampsec;
					*(double *)(shmemAdd3 + 2*sizeof(int) + sizeof(char) +sizeof(double))   = comp.timestampnsec;;
					*(double *)(shmemAdd3 + 2*sizeof(int) + sizeof(char) +2*sizeof(double))	= comp.waitForPs;
				semup__(mutex3);
			semup__(shm3isSet); // Send to Assembler
		}
		exit(0);
	}

	// Parent //
	int i = 0, processSum = 8;
	if(pid != 0){
		int stat_val;
		pid_t child_pid;
		for(; i < processSum; i++){
			child_pid = wait(&stat_val);
			if(!WIFEXITED(stat_val))
				printf("Child terminated abnormally\n");
		}
	}

	// Delete Semaphore Set
	deleteSemaphores();

	// Delete Shared Memories
	deleteSharedMemory();

}
