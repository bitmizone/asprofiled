#include "stdafx.h" // wont compile without this
#include "profiler_helper.h"
#include "common.h"

CProfilerHelper::CProfilerHelper() { }

CProfilerHelper& CProfilerHelper::GetInstance() {
	static CProfilerHelper instance;
	return instance;
}

void CProfilerHelper::ParseCallingConvention(PCCOR_SIGNATURE& data) {
	ULONG oredCallingConvetion = CorSigUncompressCallingConv(data); // PAGE 154 from part II
	if ((oredCallingConvetion & IMAGE_CEE_CS_CALLCONV_DEFAULT_HASTHIS) == IMAGE_CEE_CS_CALLCONV_DEFAULT_HASTHIS) 
	{
		std::cout << "DEFAULT_HASTHIS calling convetion" << std::endl;
	}
	if ((oredCallingConvetion & IMAGE_CEE_CS_CALLCONV_C) == IMAGE_CEE_CS_CALLCONV_C) 
	{
		std::cout << "C calling convetion" << std::endl;
	}

	if ((oredCallingConvetion & IMAGE_CEE_CS_CALLCONV_DEFAULT) == IMAGE_CEE_CS_CALLCONV_DEFAULT) 
	{
		std::cout << "DEFAULT calling convetion" << std::endl;
	}

	if ((oredCallingConvetion & IMAGE_CEE_CS_CALLCONV_EXPLICITTHIS) == IMAGE_CEE_CS_CALLCONV_EXPLICITTHIS) 
	{
		std::cout << "EXPLICITTHIS calling convetion" << std::endl;
	}

	if ((oredCallingConvetion & IMAGE_CEE_CS_CALLCONV_FASTCALL) == IMAGE_CEE_CS_CALLCONV_FASTCALL) 
	{
		std::cout << "FASTCALL calling convetion" << std::endl;
	}

	if ((oredCallingConvetion & IMAGE_CEE_CS_CALLCONV_HASTHIS) == IMAGE_CEE_CS_CALLCONV_HASTHIS) 
	{
		std::cout << "HASTHIS calling convetion" << std::endl;
	}

	if ((oredCallingConvetion & IMAGE_CEE_CS_CALLCONV_STDCALL) == IMAGE_CEE_CS_CALLCONV_STDCALL) 
	{
		std::cout << "STDCALL calling convetion" << std::endl;
	}

	if ((oredCallingConvetion & IMAGE_CEE_CS_CALLCONV_THISCALL) == IMAGE_CEE_CS_CALLCONV_THISCALL) 
	{
		std::cout << "THISCALL calling convetion" << std::endl;
	}

	if ((oredCallingConvetion & IMAGE_CEE_CS_CALLCONV_VARARG) == IMAGE_CEE_CS_CALLCONV_VARARG) 
	{
		std::cout << "VARARG calling convetion" << std::endl;
	}

}

ULONG CProfilerHelper::GetArgumentsCount(PCCOR_SIGNATURE &data) {
	ULONG argsCount = 0;
	CorSigUncompressData(data, &argsCount);
	return argsCount;
}

// only fixed string args for now
std::vector<WCHAR*>* CProfilerHelper::ParseAttributeMetaData(const void* attributeBlob, ULONG blobSize, ULONG argumentsCount) {

	static UINT8 oneByteLengthUnicodeMarker = 0;
	static UINT8 twoBytesLengthUnicodeMarker = 6;
	static UINT8 threeBytesLengthUnicodeMarker = 14;
	static UINT8 fourBytesLengthUnicodeMarker = 30;
	UINT8* blob = (UINT8*) attributeBlob;
	ULONG prolog = *((INT16*)blob);
	// move forward
	// skip prolog
	blob += 2;
	ULONG stringLength = 0;
	std::vector<WCHAR*>* argumentsValues = new std::vector<WCHAR*>();

	for (ULONG i = 0; i < argumentsCount; ++i) {
		WCHAR* argument = NULL;
		if (*blob == 0xFF) { // string is null
			argument = new WCHAR[1];
			argument[0] = '\0';
			argumentsValues->push_back(argument);
			continue;
		}
		
		ULONG packedLen = 0; 
		blob += CorSigUncompressData(blob, &packedLen);
		argument = new WCHAR[packedLen + 1];
		ULONG consumedBytes = 0;
		UINT index=0;
		while (consumedBytes < packedLen) {
			UINT8 byte = *blob;
			if ((byte >> 7) == oneByteLengthUnicodeMarker) { 
				argument[index++] = *blob;
				blob++;
				consumedBytes++;
			}else if ((byte >> 5) == twoBytesLengthUnicodeMarker) {
				argument[index++] = *((UINT16*) blob);
				blob+=2;
				consumedBytes+=2;
			}else if ((byte >> 4) == threeBytesLengthUnicodeMarker) {
				blob+=3;
			}else{//four byte
				blob+=4;
			}
		}
		argument[index] = '\0';
		argumentsValues->push_back(argument);
	}
	return argumentsValues;
}