#include "StdAfx.h"
#include "AttributeArgument.h"

CAttributeArgument::CAttributeArgument(void)
{
	this->tokensValues = new std::map<WCHAR*, CParam*>();
}

CAttributeArgument::~CAttributeArgument(void)
{
	this->tokensValues->clear();
	delete this->tokensValues;
}

void CAttributeArgument::AssignTokenValues(std::vector<CParam*> *params) {
	for (ULONG i = 0; i < tokens.size(); ++i) {
		std::wstring tokenName(tokens.at(i)->symbol);
		if (tokenName.compare(L"Identifier") != 0) {
			if (this->tokensValues->find(tokens.at(i)->image) == tokensValues->end()) {
				std::wstring tokenValue(tokens.at(i)->image);
				for (ULONG j = 0; j < params->size(); ++j) {
					if (tokenValue.compare(params->at(j)->paramName) == 0) {
						this->tokensValues->insert(std::pair<WCHAR*, CParam*>(tokens.at(i)->image, params->at(j)));
					}
				}
			}
			// when above condition is not met we don't need to do anything else as
			// symbol and its associated param are already stored
		}
	}
}