#pragma once

#include "common.h"
#include "cor.h"

class CParam
{
public:
	CParam(void);
	~CParam(void);
	WCHAR* paramName;
	WCHAR* paramTypeAsString;
	CorElementType elementType;
	PCCOR_SIGNATURE beginningOfParamSignature;

	
};
