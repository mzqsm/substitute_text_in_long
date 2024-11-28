#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <errno.h>
#include <stddef.h>
#include <windows.h>

#define BUFFER_SIZE 256

errno_t update_log_record_s(const char* filename, char* search_str, char* replace_str);

int main(void)
{
	const char* log_file = "C:\\Users\\H\\Desktop\\�����.txt";
	const char* search_str = "thx";
	const char* replace_str = "welcome";
	errno_t result = update_log_record_s(log_file, search_str,
		replace_str);
	if (result != 0) {
		char error_msg[256];
		strerror_s(error_msg, sizeof(error_msg), result);
		fprintf(stderr, "An error occurred: %s\n", error_msg);
	}
	else {
		printf("Record updated successfully.\n");
	}
	_fcloseall();

	return 0;
}




errno_t update_log_record_s(const char* filename, char* search_str, char* replace_str)
{
	if (filename == NULL ||search_str == NULL || replace_str == NULL)
	{
		return EINVAL; // ������Ч��������
	}
	FILE* file_ptr = NULL;
	errno_t err = fopen_s(&file_ptr, filename, "r+");

	if (err != 0 || file_ptr == NULL) {
		char error_msg[256];
		strerror_s(error_msg, sizeof(error_msg), errno);
		fprintf(stderr, "faile open : %s\n", error_msg);
		exit(EXIT_FAILURE);
	}
	char buffer[BUFFER_SIZE];
	long position = 0;
	int found = 0;
	while (fgets(buffer, BUFFER_SIZE, file_ptr) != NULL)
	{
		if (strstr(buffer, search_str) != NULL) {
			found = 1;
				position = ftell(file_ptr) - (long)strlen(buffer) - 1;
			break; // �ҵ���һ��ƥ����֮������ֹͣ
		}
}
	if (found) {
		fseek(file_ptr, position, SEEK_SET);
			size_t replace_len = strlen(replace_str);
		size_t search_len = strlen(search_str);
		if (replace_len > BUFFER_SIZE - 1 || search_len >
			BUFFER_SIZE - 1) {
			fclose(file_ptr);
			return ERANGE; // ���ش����룬��ʾ�ַ������ȳ�����Χ
		}
		// д���¼�¼֮ǰ���������λ�õ�������
		memset(buffer,' ',strlen(buffer) - 1); // �ÿո���䣬������
		
			buffer[strlen(buffer) - 1] = '\n'; // �������з�
		fseek(file_ptr,position, SEEK_SET);
		//���»ص�����еĿ�ʼ
		fputs(buffer, file_ptr);
		; // ���ԭ���е�����
		fseek(file_ptr, position, SEEK_SET);//���»ص�����еĿ�ʼ
		int result = fputs(replace_str, file_ptr); // д���滻���ַ�
			if (result == EOF) {
				fclose(file_ptr);
				return errno;
			}
	}
	else {
		fclose(file_ptr);
		return ENOENT; // ����δ�ҵ���ƥ����
	}
		fclose(file_ptr);
		return 0;
	}
