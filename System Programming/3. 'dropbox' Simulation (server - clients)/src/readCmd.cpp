#include "readCmd.hpp"

int readCmdSe( int argc, char **argv , int *portNum ){
	/* CMD parser */
	if(argc==3){
		if( !strcmp(argv[1], "-p") ){
			*portNum = strtol(argv[2], NULL, 10);
		}else{
			return -1;
		}
	}else{
		return -1;
	}

	return 0;
}

int readCmdCl( int argc, char **argv , char **dirName, int *portNum,
		int *workerThreads, int *bufferSize, int *serverPort, char **serverIP ){
	/* CMD parser */
	if(argc==13){
		if( !strcmp(argv[1], "-d") && !strcmp(argv[3], "-p") && !strcmp(argv[5], "-w") && !strcmp(argv[7], "-b") && !strcmp(argv[9], "-sp") && !strcmp(argv[11], "-sip")){
			*dirName		= argv[2];
			*portNum		= strtol(argv[4], NULL, 10);

			*workerThreads		= strtol(argv[6], NULL, 10);

			*bufferSize		= strtol(argv[8], NULL, 10);

			*serverPort		= strtol(argv[10], NULL, 10);

			*serverIP		= argv[12];
		}else{
			return -1;
		}
	}else{
		return -1;
	}

	return 0;
}



