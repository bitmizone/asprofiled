
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
char* ToBinary(void* p, int cBytes, int cBytesToSkip) {
	char* ptr = (char*) p;
	char* binaryRepresentation = new char[cBytes * 8];
	int i = 0;
	while (i < cBytes) {
		int bitPos;
		for (bitPos = 0; bitPos < 8; ++bitPos) {
			binaryRepresentation[i * 8 + bitPos] = *(ptr + i) >> (7 - bitPos) & 0x1;
		}
		++i;
	}
	return binaryRepresentation;
}
