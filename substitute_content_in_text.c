#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <errno.h>
#include <stddef.h>

#define MAX_SIZE 256

errno_t replace_specially_words(const char* filename, char old_content[MAX_SIZE], char replaced_content[MAX_SIZE]);

int main(void)
{
	errno = 0;

	char old_content[MAX_SIZE] = { 0 };
	char replaced_content[MAX_SIZE] = { 0 };

	puts("plz input the sentence u want to be replaced!");

	int32_t check_first_scanf = scanf_s("%99[^\n]", old_content, (unsigned int)sizeof(old_content));

	if (check_first_scanf < 0)
	{
		perror("the first scanf_s error");
		exit(EXIT_FAILURE);
	}

	(void)getchar();

	puts("plz enter the sentence u want to substitute!");

	int32_t check_second_scanf = scanf_s("%99[^\n]", replaced_content, (unsigned int)sizeof(replaced_content));

	if (check_second_scanf < 0)
	{
		perror("the second scanf_s error");
		exit(EXIT_FAILURE);
	}

	const char* filename = "C:\\Users\\H\\Desktop\\±à³Ì222ÓÃ.txt";

	(void)getchar();

	errno_t err = replace_specially_words(filename, old_content, replaced_content);

	char err_msg[MAX_SIZE] = { 0 };

	if (errno)
	{
		perror("mistake occured");
		exit(EXIT_FAILURE);
	}
	else
	{
		puts("No error happened!");
	}

	int32_t if_fcloseall = _fcloseall();

	exit(if_fcloseall == EOF ? EXIT_FAILURE : EXIT_SUCCESS);
}


errno_t replace_specially_words(const char* filename, char old_content[MAX_SIZE], char replaced_content[MAX_SIZE])
{
	FILE* stream = NULL;

	errno_t if_open = fopen_s(&stream, filename, "r+");

	if (if_open)
	{
		perror("there is some mistakes while opening the target file");
		exit(EXIT_FAILURE);
	}

	char buffer[MAX_SIZE] = { 0 };

	int32_t is_found = 1;

	long pos = 0;

	long new_pos = 0;

	while (fgets(buffer, MAX_SIZE, stream) != NULL)
	{
		buffer[strcspn(buffer, "\n")] = '\0';

		is_found = strcmp(buffer, old_content);

		if (!is_found)
		{
			new_pos = ftell(stream) - strlen(buffer) - 2;
			break;
		}
	}

	if (!is_found)
	{
		rewind(stream);

		char temporary_index[MAX_SIZE] = { 0 };

		fread(temporary_index, sizeof(char), MAX_SIZE, stream);

		char* target_in_console = strstr(temporary_index, old_content);

		if (target_in_console != NULL)
		{
			ptrdiff_t pos_before_target = target_in_console - temporary_index;

			char before_content[MAX_SIZE] = { 0 };

			strncpy_s(before_content, MAX_SIZE, temporary_index, (rsize_t)pos_before_target);

			rewind(stream);

			size_t actual_len = strlen(before_content);

			fwrite(before_content, sizeof(char), actual_len, stream);

			pos = ftell(stream) + (long)strlen(buffer) + 2;

			char sub_content[MAX_SIZE] = { 0 };

			strcpy_s(sub_content, strlen(replaced_content) + 1, replaced_content);

			if (strlen(replaced_content) < strlen(old_content))
			{
				size_t diff = strlen(old_content) - strlen(replaced_content);

				fwrite(sub_content, sizeof(char), strlen(sub_content), stream);

				for (size_t index = 0; index < diff; index++)
				{
					fputc(' ', stream);
				}
			}
			else if (strlen(replaced_content) > strlen(old_content))
			{
				fseek(stream, 0, SEEK_END);

				long end_pos = ftell(stream);

				fseek(stream, pos, SEEK_SET);

				char end_content[MAX_SIZE] = { 0 };

				fread(end_content, sizeof(char), (size_t)(end_pos - pos), stream);

				fseek(stream, new_pos, SEEK_SET);

				fwrite(sub_content, sizeof(char), strlen(sub_content), stream);

				fputc('\n', stream);

				fputs(end_content, stream);
			}
			else
			{
				fwrite(sub_content, sizeof(char), strlen(sub_content), stream);
			}
		}
	}
	else
	{
		errno = EIO;
		perror("not find the sentence u point");
		exit(EXIT_FAILURE);
	}
}
