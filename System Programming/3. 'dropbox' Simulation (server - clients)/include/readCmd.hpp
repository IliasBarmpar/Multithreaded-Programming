#ifndef READCMD_HPP_
#define READCMD_HPP_

#include <stdio.h>
#include <string>
#include <string.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

int readCmdSe( int argc, char **argv , int *portNum );

int readCmdCl( int argc, char **argv , char **dirName, int *portNum,
		int *workerThreads, int *bufferSize, int *serverPort, char **serverIP );


#endif /* READCMD_HPP_ */
