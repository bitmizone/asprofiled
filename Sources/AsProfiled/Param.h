#pragma once

#include "common.h"
#include "cor.h"

class CParam
{
public:
	CParam(void);
	~CParam(void);
	ULONG ReadData(ULONG_PTR startAddress);
	
	WCHAR* paramName;
	WCHAR* paramTypeAsString;
	CorElementType elementType;
	// Metadata signature
	PCCOR_SIGNATURE beginningOfParamSignature;
	// Parameter value
	UINT_PTR addressToParameterValue;
	ULONG dataLength;
	UINT data;
	
};
