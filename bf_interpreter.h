#ifndef BF_INTERPRETER_H
# define BF_INTERPRETER_H

#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#define BUFFER_SIZE 1024
#define MEMORY_SIZE 65536

typedef void (*actions_array)(char **instructions, char **memory);

void inc_ptr(char **instructions, char **memory);
void dec_ptr(char **instructions, char **memory);
void inc_data(char **instructions, char **memory);
void dec_data(char **instructions, char **memory);
void output_data(char **instructions, char **memory);
void input_data(char **instructions, char **memory);
void block_begin(char **instructions, char **memory);
void block_end(char **instructions, char **memory);
void do_nothing(char **instructions, char **memory);

#endif
