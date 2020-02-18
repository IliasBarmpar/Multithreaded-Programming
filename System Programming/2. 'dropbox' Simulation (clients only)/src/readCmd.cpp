#include "readCmd.hpp"

int readCmd( int argc, char **argv , int *id, char **common_dir, char **input_dir,
		char **mirror_dir, int *buffer_size, char **log_file){
	/* CMD parser */
	if(argc==13){
		if( !strcmp(argv[1], "-n") && !strcmp(argv[3], "-c") && !strcmp(argv[5], "-i") && !strcmp(argv[7], "-m") && !strcmp(argv[9], "-b") && !strcmp(argv[11], "-l")){
			*id				=atoi(argv[2]);
			*common_dir		=argv[4];
			*input_dir		=argv[6];
			*mirror_dir		=argv[8];
			*buffer_size	=atoi(argv[10]);
			*log_file		=argv[12];
		}else{
			return -1;
		}
	}else{
		return -1;
	}

	return 0;
}
