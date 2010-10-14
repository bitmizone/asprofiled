#pragma once
#include <vector>
#include <map>
#include "Param.h"
#include "astudillo/CGTFile.h"

// Holds attribute argument value and its tokenized form
class CAttributeArgument
{
public:
	CAttributeArgument(void);
	~CAttributeArgument(void);
	void AssignTokenValues(std::vector<CParam*>* params);
	WCHAR* argumentValue;
	std::vector<Token*> tokens;
	std::map<WCHAR*, CParam*>* tokensValues;
};
