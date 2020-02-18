#ifndef READCMD_HPP_
#define READCMD_HPP_

#include <iostream>
#include <string.h>
#include <string>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>

int readCmd( int argc, char **argv , int *id, char **common_dir, char **input_dir,
		char **mirror_dir, int *buffer_size, char **log_file);

#endif
