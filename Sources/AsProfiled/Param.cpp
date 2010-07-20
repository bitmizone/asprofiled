#include "StdAfx.h"
#include "Param.h"

CParam::CParam(void)
{
	this->paramName = new WCHAR[NAME_BUFFER_SIZE];
	this->paramType = new WCHAR[NAME_BUFFER_SIZE];
	this->paramName[0] = '\0';
	this->paramType[0] = '\0';

}

CParam::~CParam(void)
{
}
