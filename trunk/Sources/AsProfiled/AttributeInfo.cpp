#include "StdAfx.h"
#include "AttributeInfo.h"

CAttributeInfo::CAttributeInfo(void)
{
	attributeToken = mdTokenNil;
	attributeBlob = NULL;
	attributeBlobSize = 0;
	attributeConstructor = mdTokenNil;
	argumentsCount = 0;
	typeName = NULL;
}

CAttributeInfo::~CAttributeInfo(void)
{
}
