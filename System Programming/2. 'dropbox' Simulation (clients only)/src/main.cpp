#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

#include "functions.hpp"
#include "readCmd.hpp"
using namespace std;

int globvar = 0;
int sigintquit = 0;

struct nameArr{
	char *name;
	struct nameArr *next;
};

void addNewName(char* name, nameArr *nameArray){
	nameArr *temp = nameArray;
	while(temp->next!=NULL)
		temp = temp->next;

	temp->next = new nameArr;
	temp->next->name = (char *)malloc(sizeof(name));
	temp->next->next = NULL;
	strcpy(temp->next->name, name);
}

int checkForNewId(char *ptr, nameArr *nameArray){
	nameArr *temp = nameArray;
	while(temp!=NULL){
		if(!strcmp(ptr,temp->name))
			return 0;
		temp = temp->next;
	}
	return 1;
}

int main( int argc, char* argv[] ){
	int id, buffer_size;
	char *common_dir, *input_dir, *mirror_dir, *log_file;
	int err = readCmd(argc, argv, &id, &common_dir, &input_dir, &mirror_dir, &buffer_size, &log_file);

	if(err){
		cout << "Incorrect structure for command line arguments." << endl;
		return -1;
	}else{
		nameArr *nameArray;
		nameArray = NULL;

		// ./mirror_client -n 1 -c ./common -i ./1_input -m ./1_mirror -b 100 -l log_file1
		DIR *inputDir = opendir(input_dir);
		if(inputDir){
			DIR *mirrDir = opendir(mirror_dir);
			if(mirrDir){
				perror("Mirror exists");
				exit(1);
			}else if( ENOENT == errno){
				// Make a mirror directory and a common directory if it doesn't already exist
				int r = mkdir( mirror_dir, 0777 );

				DIR *commDir = opendir(common_dir);
				if(!commDir)
					int r2 = mkdir( common_dir, 0777);

				// Create the id file by creating the path as a string and then adding it to the namelist
				char idCommPath[sizeof(common_dir) + sizeof(id) + 4];
				snprintf(idCommPath, sizeof(common_dir) +countDigits(id) + 5, "%s/%d.id", common_dir, id);

				int filedes;
				if(( filedes = open( idCommPath , O_CREAT | O_EXCL | O_RDWR , PERMS ) ) == -1){
					perror("creating");
					exit(1);
				}else{
					//Get pid, write it on the file, then add the id to the namelist.
					int pid = getpid();
					write(filedes, (void *)&pid, sizeof(pid));

					nameArray = new nameArr;
					nameArray->name = (char *)malloc(countDigits(id)+4);
					nameArray->next = NULL;
					snprintf(nameArray->name, sizeof(id) + 5, "%d.id" ,id);

					close(filedes);
				}
				signal(SIGINT, my_handler);
				signal(SIGQUIT, my_handler);
				while(1){
					if(sigintquit){
						rmdir(mirror_dir);
						unlink(idCommPath);
						exit(0);
					}
					// Endlessly checking for new id on common_dir
					commDir = opendir(common_dir);
					struct dirent *direntp;
					while( (direntp=readdir(commDir)) != NULL){

						// Read common_dir
						int len = strlen(direntp->d_name);
						if(len > 3 && direntp->d_name[len-3] == '.' && direntp->d_name[len-2] == 'i' && direntp->d_name[len-1] == 'd'){
							if(checkForNewId(direntp->d_name, nameArray)){
								int newid = atoi(direntp->d_name);
								int numOfTries = 3;

								signal(SIGUSR1, my_handler);
								signal(SIGUSR2, my_handler);
								signal(SIGPIPE, my_handler);
								while(numOfTries){
									forkChildren(common_dir, input_dir, mirror_dir, buffer_size, id, newid);

									pause();
									if(globvar==1){
										// Print error
										fprintf(stderr, "Something went wrong with the children.\n");

										// Clear everything and start again
										globvar = 0;
										char newPath[1024];
										snprintf(newPath, sizeof(newPath), "%s/%d", mirror_dir, newid);
										rmDir(newPath);
										numOfTries--;
									}else{
										numOfTries = 0;
									}

									int status;
									pid_t temp;
									while((temp = wait(&status)) > 0);
								}
								//Add it to the list
								addNewName(direntp->d_name, nameArray);
							}
						}
					}
					closedir(commDir);
					sleep(2);
				}
			}else{
				perror("Misc error occured");
				exit(1);
			}
		}else if( ENOENT == errno){
			perror("Input file doesn't exist");
			exit(1);
		}else{
			perror("Misc error occured");
			exit(1);
		}
	}
}
