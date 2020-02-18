#ifndef SRC_FUNCTIONS_HPP_
#define SRC_FUNCTIONS_HPP_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "readCmd.hpp"
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>

#define PERMS 0777

using namespace std;

extern int globvar;
extern int sigintquit;

void my_handler(int signum);

int countDigits(int num);

void removeDir(const char *name);

void rmDir(const char *name);

void transferWriter(char *name, char *fifoFile, char *prpath, int buffer_size);

int checkForError(int fd);

void forkChildren(char *common_dir, char *input_dir, char *mirror_dir, int buffer_size, int id, int newid);


#endif /* SRC_FUNCTIONS_HPP_ */
