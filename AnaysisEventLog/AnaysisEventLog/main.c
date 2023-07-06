#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "EventProcess.h"

long getFileSize(FILE *f);

int main(void) {
	FILE* f = NULL;
	long f_size = 0;
	char* buf = NULL;

	
	if (0 != fopen_s(&f, "Security.evtx", "rb")) {
		printf("can't find\n");
		return -1;
	}
	
	//printf("finded.! \n");

	if (f == NULL) {
		printf("f == NULL");
		return -1;
	}
	f_size = getFileSize(f);
	//printf("f_size : %d\n", f_size);

	if (f_size == 0) {
		printf("fileSize == 0");
		return -1;
	}

	buf = (char*)malloc(f_size);
	if (buf == NULL) {
		printf("buf == NULL");
		fclose(f);
		return -1;
	}
	memset(buf, '\0', f_size);
	
	fseek(f, 0, SEEK_SET);
	fread(buf, 1, f_size, f);
	fclose(f);

	processEventData(buf, f_size);

	free(buf);
	return 0;
}

long getFileSize(FILE* f) {
	long currentOffset = 0;
	long fileSize = 0;

	currentOffset = ftell(f);
	
	fseek(f, 0, SEEK_END);
	fileSize = ftell(f);
	fseek(f, currentOffset, SEEK_CUR);

	return fileSize;
}