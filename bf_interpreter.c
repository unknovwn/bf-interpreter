#include "bf_interpreter.h"

int check_brackets(char *instructions)
{
	int brackets;

	brackets = 0;
	for (int i = 0; instructions[i]; i++)
	{
		if (!(isspace(instructions[i])))
		{
			if (instructions[i] == '[')
				brackets++;
			else if (instructions[i] == ']')
			{
				brackets--;
				if (brackets < 0)
					return (0);
			}
		}
	}
	return (brackets == 0);
}

char *parse_file(int fd)
{
	char    *instructions;
	char    buf[BUFFER_SIZE + 1];
	ssize_t count;

	if (!(instructions = strdup("")))
		return (0);
	while ((count = read(fd, buf, BUFFER_SIZE)))
	{
		if (count == -1)
		{
			free(instructions);
			return (0);
		}
		buf[count] = 0;
		if (!(instructions = (char*)realloc(instructions,
						strlen(instructions) + count + 1)))
			return (0);
		instructions = strcat(instructions, buf);
	}
	if (!(check_brackets(instructions)))
	{
		free(instructions);
		write(2, "Brackets are not balanced\n", 26);
		return (0);
	}
	return (instructions);
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
