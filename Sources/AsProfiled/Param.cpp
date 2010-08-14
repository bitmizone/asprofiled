#include "StdAfx.h"
#include "Param.h"

CParam::CParam(void)
{
	this->paramName = new WCHAR[NAME_BUFFER_SIZE];
	this->paramTypeAsString = new WCHAR[NAME_BUFFER_SIZE];
	this->paramName[0] = '\0';
	this->paramTypeAsString[0] = '\0';
	this->elementType = ELEMENT_TYPE_END;
	this->beginningOfParamSignature = NULL;
	this->addressToParameterValue = NULL;
	this->dataLength = 0;
	this->data = 0;
}

CParam::~CParam(void)
{
	delete [] this->paramName;
	delete [] this->paramTypeAsString;
}

ULONG CParam::ReadData(ULONG_PTR startAddress) {
	this->addressToParameterValue = startAddress;

	UINT8* pValue = reinterpret_cast<UINT8*>(startAddress);
	switch (this->elementType) {
		case ELEMENT_TYPE_BOOLEAN:
			/*std::cout << std::hex << *pValue << std::endl;
			std::cout << *(bool*)startAddress << std::endl;*/
			data = *pValue;
			this->dataLength = 1;
			break;
		default:
			break;
	}

	return this->dataLength;
}
