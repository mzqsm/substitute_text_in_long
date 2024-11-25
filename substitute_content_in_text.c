#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <stdlib.h>

#define MAX_SIZE 200

errno_t replace_words_in_log(const char* filename, char* initial_content, char* replaced_content);

int main(void)
{
	int32_t if_mistake = 0;

	const char* filename = "C:\\Users\\H\\Desktop\\编程222用.log";//等修改完把路径再改回来

	char initial_content[MAX_SIZE] = { 0 };

	char replaced_content[MAX_SIZE] = { 0 };

	puts("plz input the sentence u want to be substituted!");

	scanf_s("%99[^\n]", initial_content, MAX_SIZE);

	puts("plz enter the sentence u want to replace");

	(void)getchar();

	scanf_s("%99[^\n]", replaced_content, MAX_SIZE);

	(void)getchar();

	errno_t res = replace_words_in_log(filename, initial_content, replaced_content);

	if (res)
	{
		char error_msg[200];
		strerror_s(error_msg, MAX_SIZE, res);
		fprintf(stderr, "An error occured : %s\n", error_msg);
	}
	else
	{
		puts("input finished!");
	}

	_fcloseall();

	perror("check if mistake :");

	return 0;
}

errno_t replace_words_in_log(const char* filename, char* initial_content, char* replaced_content)
{
	FILE* stream = NULL;

	if (filename == NULL || initial_content == NULL || replaced_content == NULL)
	{
		return EINVAL;
	}

	errno_t err = fopen_s(&stream, filename, "r+");

	if (err != 0 || stream == NULL)
	{
		char error_msg[100];

		strerror_s(error_msg, sizeof(error_msg), errno);

		fprintf(stderr, "Failed to open the file, because : %s\n", error_msg);

		exit(EXIT_FAILURE);
	}

	char buffer[MAX_SIZE] = { 0 };

	long pos = 0;

	int32_t is_found = 0;

	while (fgets(buffer, MAX_SIZE, stream) != NULL)
	{
		printf("texeeeeeee: %d\n",ftell(stream));
		buffer[strcspn(buffer, "\n")] = '\0';

		if (strcmp(buffer, initial_content) == 0)
		{
			pos = ftell(stream) - (long)strlen(buffer) - 2;//hello\n\0.

			printf("text1: %d\n", ftell(stream));

			printf("text2:%d\n", strlen(buffer));
			is_found = 1;

			break;
		}
	}

	if (is_found)
	{
		fseek(stream, pos, SEEK_SET);

		size_t replace_len = strlen(replaced_content);
		size_t search_len = strlen(initial_content);

		if (replace_len > MAX_SIZE - 1 || search_len > MAX_SIZE - 1)
		{
			fclose(stream);

			return ERANGE;
		}
		printf("final test: %d", ftell(stream));

		char input_index[MAX_SIZE] = { 0 };

		strcpy_s(input_index, MAX_SIZE, replaced_content);

		strcat_s(input_index, MAX_SIZE, "\r\n");

		int32_t index = fputs(input_index, stream);

		if (index == EOF)
		{
			fclose(stream);
			return errno;
		}
	}
	else
	{
		fclose(stream);
		return ENOENT;
	}

	fclose(stream);
	return 0;
}