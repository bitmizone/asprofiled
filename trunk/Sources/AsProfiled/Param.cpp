#include "StdAfx.h"
#include "Param.h"

CParam::CParam(void)
{

	this->paramName = new WCHAR[NAME_BUFFER_SIZE];
	this->paramTypeAsString = new WCHAR[NAME_BUFFER_SIZE];
	this->paramName[0] = '\0';
	this->paramTypeAsString[0] = '\0';
	this->elementType = ELEMENT_TYPE_END;
	beginningOfParamSignature = NULL;

}

CParam::~CParam(void)
{
	delete [] this->paramName;
	delete [] this->paramTypeAsString;
}
