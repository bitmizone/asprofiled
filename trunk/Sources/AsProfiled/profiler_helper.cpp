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
	std::cout << "ParamCount " << argsCount << std::endl;
	return argsCount;
}

// only fixed string args for now
void CProfilerHelper::ParseAttributeMetaData(const void* attributeBlob, ULONG blobSize) {
	static UINT8 oneByteLengthUnicodeMarker = 0;
	static UINT8 twoBytesLengthUnicodeMarker = 6;
	static UINT8 threeBytesLengthUnicodeMarker = 14;
	static UINT8 fourBytesLengthUnicodeMarker = 30;
	UINT8* blob = (UINT8*) attributeBlob;
	ToBinary((void*)blob, blobSize, 0);
	ULONG prolog = *((INT16*)blob);
	std::cout << " PROLOG = "<< prolog << std::endl;
	// move forward
	// skip prolog
	blob += 2;
	ULONG stringLength = 0;
	if (*blob == 0xFF) { // string is null
		std::cout << "string is null" << std::endl;
		blob++;
	}
	
	ULONG packedLen = 0; 
	CorSigUncompressData(blob, &packedLen);
	std::cout<< "PackedLen is " << packedLen << std::endl;
	ULONG consumedBytes = 0 ;
	WCHAR arg[1024];
	UINT index=0;
	while (consumedBytes <= packedLen) {
		UINT8 byte = *blob;
		if ((byte >> 7) == oneByteLengthUnicodeMarker) { 
			arg[index++] = *blob;
			blob++;
			consumedBytes++;
		}else if ((byte >> 5) == twoBytesLengthUnicodeMarker) {
			arg[index++] = *((UINT16*) blob);
			blob+=2;
			consumedBytes+=2;
		}else if ((byte >> 4) == threeBytesLengthUnicodeMarker) {
			blob+=3;
		}else{//four byte
			blob+=4;
		}
		
	}
	arg[index] = '\0';
	PrintCharArray(arg);

}