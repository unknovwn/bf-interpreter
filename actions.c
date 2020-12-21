#include "bf_interpreter.h"

void inc_ptr(char **instructions, char **memory)
{
	(*memory)++;
	(*instructions)++;
}

void dec_ptr(char **instructions, char **memory)
{
	(*memory)--;
	(*instructions)++;
}

void inc_data(char **instructions, char **memory)
{
	(**memory)++;
	(*instructions)++;
}

void dec_data(char **instructions, char **memory)
{
	(**memory)--;
	(*instructions)++;
}

void output_data(char **instructions, char **memory)
{
	write(1, *memory, 1);
	(*instructions)++;
}

void input_data(char **instructions, char **memory)
{
	if ((read(0, *memory, 1)) == -1)
	{
		free(*instructions);
		perror("Error encountered while reading input");
		exit(0);
	}
	(*instructions)++;
}

void block_begin(char **instructions, char **memory)
{
	int brackets_counter;

	brackets_counter = 1;
	(*instructions)++;
	if (!(**memory))
	{
		while (brackets_counter)
		{
			if ((**instructions) == '[')
				brackets_counter++;
			else if ((**instructions) == ']')
				brackets_counter--;
			(*instructions)++;
		}
	}
}

void block_end(char **instructions, char **memory)
{
	int brackets_counter;

	brackets_counter = 1;
	if (**memory)
	{
		(*instructions)--;
		while (brackets_counter)
		{
			if ((**instructions) == ']')
				brackets_counter++;
			else if ((**instructions) == '[')
				brackets_counter--;
			(*instructions)--;
		}
		(*instructions)++;
	}
	(*instructions)++;
}

void do_nothing(char **instructions, char **memory)
{
	// Getting rid of unused variables warning (:
	(void)memory;

	(*instructions)++;
	return ;
}
