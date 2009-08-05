#include "stdafx.h"
#include <cstdio>

void PrintCharArray(WCHAR* charArray) {
	int i = 0;
	
	while ((char) charArray[i] != '\0') {
		printf("%c", charArray[i++]);
	}
	printf("\n");
}