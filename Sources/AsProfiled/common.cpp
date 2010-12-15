
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

std::vector<std::wstring> Split(const std::wstring& text, WCHAR delimiter) {
	std::vector<std::wstring> result;
	std::wstring::size_type start = 0;
	std::wstring::size_type end = text.find( delimiter, start );

	while( end != std::wstring::npos )
	{
		std::wstring token = text.substr( start, end - start );

		result.push_back( token );

		start = end + 1;
		end   = text.find( delimiter, start );
	}

	result.push_back( text.substr( start ) );

	return result;

}