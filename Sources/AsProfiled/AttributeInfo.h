#pragma once
#include <vector>
#include "cor.h"
#include "AttributeArgument.h"

class CAttributeInfo
{
public:
	CAttributeInfo(void);
	~CAttributeInfo(void);

	mdCustomAttribute attributeToken;
	const void* attributeBlob;
	ULONG attributeBlobSize;
	mdMethodDef attributeConstructor;
	ULONG argumentsCount;
	WCHAR* typeName;
	std::vector<CAttributeArgument*>* arguments;
	
};
