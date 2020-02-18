#include "functions.hpp"

void my_handler(int signum){
	if(signum == SIGUSR1){
		globvar = 1;
	}

	if(signum == SIGPIPE){
		globvar = 1;
	}

	if(signum == SIGUSR2){
		//Success, helps stop pause on father process.
	}

	if(signum == SIGINT){
		sigintquit = 1;
	}

	if(signum == SIGQUIT){
		sigintquit = 1;
	}
}

int countDigits(int num){
	int count = 0;
	while(num != 0){
		num /= 10;
		++count;
	}
	return count;
}

void removeDir(const char *name){
	DIR *dir;
	struct dirent *direntInDir;
	if(!(dir = opendir(name)))
		return ;

	while( (direntInDir = readdir(dir)) != NULL ){
		char path[1024];
		snprintf(path, sizeof(path), "%s/%s", name, direntInDir->d_name);

		if(direntInDir->d_type == DT_DIR){
			if(strcmp(direntInDir->d_name, ".") == 0 || strcmp(direntInDir->d_name, "..") == 0)
				continue;
			removeDir(path);
			rmdir(path);
		}else{
			unlink(path);
		}
	}

}

void rmDir(const char *name){
	removeDir(name);
	rmdir(name);
}

void transferWriter(char *name, char *fifoFile, char *prpath, int buffer_size){
	int fd, nwrite;
	DIR *dir;
	struct dirent *direntInDir;
	if(!(dir = opendir(name)))
		return ;

	while( (direntInDir = readdir(dir)) != NULL ){
		char fullPath[1024];
		snprintf(fullPath, sizeof(fullPath), "%s/%s", name, direntInDir->d_name);

		char pathForReceiver[1024];
		snprintf(pathForReceiver, sizeof(pathForReceiver), "%s/%s", prpath, direntInDir->d_name);

		if( ( fd = open( fifoFile, O_WRONLY  ) ) < 0 ) { perror("error:"); kill(getppid(),SIGPIPE); exit(2); }
		if(direntInDir->d_type == DT_DIR){
			if(strcmp(direntInDir->d_name, ".") == 0 || strcmp(direntInDir->d_name, "..") == 0) continue;

			// Writes 0 if it's a folder
			short isFolder = -1;
			if (( nwrite = write(fd, &isFolder, 2) ) == -1) { cout << "xxx" << endl; kill(getppid(),SIGPIPE); exit(2); }


			// Writes the size of the folder's path and name
			short nameSize = (int)strlen(pathForReceiver)-1;
			if (( nwrite = write(fd, &nameSize, 2) ) == -1) { kill(getppid(),SIGPIPE); exit(2); }


			// Writes the folder's path and name
			if (( nwrite = write(fd, pathForReceiver+2, (int)strlen(pathForReceiver)-1) ) == -1) { kill(getppid(),SIGPIPE); exit(2); }

			// Recursive call
			transferWriter(fullPath, fifoFile, pathForReceiver, buffer_size);
		}else{
			// Writes the size of the file's path and name
			short temp = (int)strlen(pathForReceiver)-1;
			if (( nwrite = write(fd, &temp, 2) ) == -1) { kill(getppid(),SIGPIPE); exit(2); }


			// Writes the file's path and name
			if (( nwrite = write(fd, pathForReceiver + 2, temp) ) == -1) { kill(getppid(),SIGPIPE); exit(2); }

			// Writes size of the file
			struct stat statbuf;
			if( stat(fullPath, &statbuf) == -1){ perror("Failed to get file status"); kill(getppid(),SIGPIPE); exit(2); }
			int size = statbuf.st_size;
			if (( nwrite = write(fd, &size, sizeof(int)) ) == -1) { kill(getppid(),SIGPIPE);exit(2); }

			// Open the file, read from it and start writing to the pipe on a 'buffer_size' pace.
			int n, fd2;
			char buf[buffer_size];
			if( ( fd2 = open( fullPath, O_RDONLY ) ) < 0 ) { perror("error:"); kill(getppid(),SIGPIPE); exit(6); }

			while ( ( n = read(fd2, buf, sizeof(buf) ) ) > 0)
				if (( nwrite = write(fd, buf, n) ) == -1) { kill(getppid(),SIGPIPE); exit(2); }
			sleep(1);

		}
	}
}

int checkForError(int fd){
	// Waits for writer for 30 seconds and issues an error if necessary.
	int error = 0, ret = 0;
	struct timeval tv;
	fd_set fds;

	tv.tv_sec = 30;
	tv.tv_usec=0;
	while( ret == 0){
		FD_ZERO(&fds);
		FD_SET( fd, &fds );

		ret = select( fd+1, &fds, (fd_set *)0, (fd_set *)0, &tv);
		if(tv.tv_sec==0&&tv.tv_usec==0){
			ret = 1;
			error = 1;
		}
	}

	return error;
}

void forkChildren(char *common_dir, char *input_dir, char *mirror_dir, int buffer_size, int id, int newid){
	//SENDER
	int pid = fork();
	if( pid == 0 ){
		int fd;
		short bytes = 0;

		//Create fifo file (if it doesn't exist)
		char fifoFile[sizeof(common_dir)+countDigits(id)+countDigits(newid)+15];
		snprintf(fifoFile, sizeof(fifoFile), "%s/%d_to_%d.fifo", common_dir, id, newid);
		if( mkfifo( fifoFile, 0666) == -1 ) if ( errno != EEXIST ) { perror ( "sender: mkfifo" ); kill(getppid(),SIGPIPE); exit (6) ; }

		//Start transfering process
		transferWriter(input_dir, fifoFile, ".", buffer_size);

		cout << "Write the final '00' bytes" << endl;
		//Write the final '00' bytes
		if( ( fd = open( fifoFile, O_WRONLY ) ) < 0 ) { perror("error:"); kill(getppid(),SIGPIPE); exit(6); }
		if ( write(fd, &bytes, 2)  == -1) { kill(getppid(),SIGPIPE); exit(2); }

		cout << "Exiting (writer)." << endl;
		exit(0);
	}


	//RECEIVER
	int pid2 = fork();
	if( pid2 == 0 ){
		//Receiver Kid
		int fd, nwrite;

		// Create fifo name and then use it to create the named pipe and then open it
		char fifoFile[sizeof(common_dir)+countDigits(id)+countDigits(newid)+15];
		snprintf(fifoFile, sizeof(fifoFile), "%s/%d_to_%d.fifo", common_dir, newid, id);
		if( mkfifo( fifoFile, 0666) == -1 ) if ( errno != EEXIST ) { perror ( "sender: mkfifo" ) ; exit (6) ; }
		if( ( fd = open( fifoFile, O_RDWR ) ) < 0 ) { perror("error:"); kill(getppid(),SIGUSR1); exit(6); }


		// Create a folder in mirror to put files in
		char msg1[strlen(mirror_dir)+countDigits(newid)+3];
		snprintf(msg1, sizeof(msg1), "%s/%d", mirror_dir, newid);
		mkdir(msg1, 0777);


		while(1){
			// Reads type of file to be read (Directory / File / Exit case)
			short type;
			if( checkForError(fd) ){
				fprintf(stderr, "Read took too long.\n"); kill(getppid(),SIGUSR1); exit(1);
			}else if ( read(fd, &type, 2) < 0) {
				perror ( "problem in reading" ) ; kill(getppid(),SIGUSR1); exit(2);
			}

			if(type==0){
				cout << "Received 'OO' and I exit" << endl;
				kill(getppid(), SIGUSR2);
				// Received 'OO'
				exit(0);
			}else if(type==-1){
			// It's a directory
				// Reads the size of the folder's path and name
				short nameSize;
				if( checkForError(fd) ){
					fprintf(stderr, "Read took too long.\n"); kill(getppid(),SIGUSR1); exit(1);
				}else if ( read(fd, &nameSize, 2) < 0) {
					perror ( "problem in reading" ); kill(getppid(),SIGUSR1); exit(2);
				}

				// Reads the folder's path and name
				char nameOfFold[nameSize];
				if( checkForError(fd) ){
					fprintf(stderr, "Read took too long.\n"); kill(getppid(),SIGUSR1); exit(1);
				}else if ( read(fd, nameOfFold, nameSize) < 0) {
					perror ( "problem in reading" ); kill(getppid(),SIGUSR1); exit(2);
				}


				char fixNOF[strlen(mirror_dir)+5+strlen(nameOfFold)+countDigits(newid)];
				snprintf(fixNOF, sizeof(fixNOF), "%s/%d/%s", mirror_dir, newid, nameOfFold);
				mkdir( fixNOF, 0777 );
			}else{
			// It's a file
				// Reads the file's path and name
				char msg2[type];
				if( checkForError(fd) ){
					fprintf(stderr, "Read took too long.\n"); kill(getppid(),SIGUSR1); exit(1);
				}else if ( read(fd, msg2, type) < 0) {
					perror ( "problem in reading" );
					exit(5);
				}

				int filedes;
				char aaa[strlen(mirror_dir)+5+strlen(msg2)+countDigits(newid)];
				snprintf(aaa, sizeof(aaa), "%s/%d/%s", mirror_dir, newid, msg2);
				if(( filedes = open( aaa , O_CREAT | O_EXCL | O_RDWR , PERMS ) ) == -1){ kill(getppid(),SIGUSR1); exit(1); };


				// Reads size of the file
				int sizeOfFile;
				if( checkForError(fd) ){
					fprintf(stderr, "Read took too long.\n"); kill(getppid(),SIGUSR1); exit(1);
				}else if ( read(fd, &sizeOfFile, sizeof(int)) < 0) {
					perror ( "problem in reading" ); kill(getppid(),SIGUSR1); exit(5);
				}

				// Open the pipe, read from it and start writing to the new file on a 'buffer_size' pace.
				int n, count = 0;
				char buf[buffer_size];
				while(1){
					if( checkForError(fd) ){
						fprintf(stderr, "Read took too long.\n"); kill(getppid(),SIGUSR1); exit(1);
					}else if ( (n = read(fd, buf, sizeof(buf))) < 0){
						perror ( "problem in reading" ); kill(getppid(),SIGUSR1); exit(5);
					}

					if (( nwrite = write(filedes, buf, n) ) == -1) {
						perror ( "problem in reading" ); kill(getppid(),SIGUSR1); exit(5);
					}
					count +=n;

					if(count>=sizeOfFile)
						break;
				}
			}
		}
		cout << "Exiting (Reader)." << endl;
		exit(0);
	}
}
