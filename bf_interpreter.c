#include "bf_interpreter.h"

char *parse_file(int fd)
{
	char    *result;
	char    buf[BUFFER_SIZE + 1];
	ssize_t count;
	int     brackets;

	if (!(result = strdup("")))
		return (0);
	while ((count = read(fd, buf, BUFFER_SIZE)))
	{
		if (count == -1)
		{
			free(result);
			return (0);
		}
		buf[count] = 0;
		if (!(result = (char*)realloc(result, (strlen(result) + count + 1))))
			return (0);
		result = strcat(result, buf);
	}
	brackets = 0;
	for (int i = 0; result[i]; i++)
	{
		if (!(isspace(result[i])))
		{
			if (result[i] == '[')
				brackets++;
			else if (result[i] == ']')
			{
				brackets--;
				if (brackets < 0)
				{
					write(2, "Not matching brackets\n", 22);
					free(result);
					return (0);
				}
			}
		}
	}
	if (brackets > 0)
	{
		write(2, "Not matching brackets\n", 22);
		free(result);
		return (0);
	}
	return (result);
}

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

void fill_actions_arr(actions_array actions[128])
{
	for (int i = 0; i < 128; i++)
		actions[i] = &do_nothing;
	actions['>'] = &inc_ptr;
	actions['<'] = &dec_ptr;
	actions['+'] = &inc_data;
	actions['-'] = &dec_data;
	actions['.'] = &output_data;
	actions[','] = &input_data;
	actions['['] = &block_begin;
	actions[']'] = &block_end;
}

void execute_instructions(char *instructions)
{
	char          *memory;
	char          *memory_start;
	actions_array actions[128];

	if (!(memory = (char*)calloc(MEMORY_SIZE, sizeof(char))))
		return ;
	memory_start = memory;
	fill_actions_arr(actions);
	while (*instructions)
		actions[(int)(*instructions)](&instructions, &memory);
	free(memory_start);
}

int main(int argc, char **argv)
{
	char *instructions;
	int  fd;

	if (argc != 2)
	{
		write(2, "Error: Invalid arguments\n", 25);
		return (0);
	}
	if ((fd = open(argv[1], O_RDONLY)) == -1)
	{
		perror(0);
		return (0);
	}
	if (!(instructions = parse_file(fd)))
	{
		if (errno)
			perror("Error encountered while parsing file");
		return (0);
	}
	execute_instructions(instructions);
	free(instructions);
	return (0);
}
