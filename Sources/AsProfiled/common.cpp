
#include "stdafx.h"
#include "common.h"


void PrintCharArray(WCHAR* charArray) {
	int i = 0;
	while (((char) charArray[i]) != '\0') {
		printf("%c", charArray[i++]);
	}
}

// print binary representation of bytes
// used for checking correctness of retrieved CorTokenType
void ToBinary(void* p, int cBytes, int cBytesToSkip) {
	char* ptr = (char*) p;
	int i = 0;
	while (i < cBytes) {
		int bitPos;
		for (bitPos = 0; bitPos < 8; ++bitPos) {
			printf("%d", *(ptr + i) >> (7 - bitPos) & 0x1);
		}
		printf(" ");
		++i;
	}
	printf("\n");
}
