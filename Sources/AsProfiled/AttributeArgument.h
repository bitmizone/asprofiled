#pragma once
#include <vector>
#include "astudillo/CGTFile.h"
class CAttributeArgument
{
public:
	CAttributeArgument(void);
	~CAttributeArgument(void);
	WCHAR* argumentValue;
	std::vector<Token*> tokens;
};
