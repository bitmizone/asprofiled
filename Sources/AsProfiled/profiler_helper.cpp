#include "stdafx.h" // wont compile without this
#include "profiler_helper.h"


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