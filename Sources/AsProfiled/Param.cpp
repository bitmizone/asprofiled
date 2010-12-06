#include "StdAfx.h"
#include "Param.h"
#include "ValueReader.h"

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
	this->isDataPersisted = false;
}

CParam::~CParam(void)
{
	delete [] this->paramName;
	delete [] this->paramTypeAsString;
}

CParam* CParam::GetCopy() {
	CParam* copy = new CParam();
	copy->value = this->value;
	copy->tokenType = this->tokenType;
	copy->result = this->result;
	wcsncpy(copy->paramTypeAsString, this->paramTypeAsString, NAME_BUFFER_SIZE);
	wcsncpy(copy->paramName, this->paramName, NAME_BUFFER_SIZE);
	copy->isDataPersisted = this->isDataPersisted;
	copy->evaluated = this->evaluated;
	copy->elementType = this->elementType;
	copy->dataLength = this->dataLength;
	copy->data = this->data;
	copy->cpi = this->cpi;
	copy->children = this->children;
	copy->beginningOfParamSignature = this->beginningOfParamSignature;
	copy->addressToParameterValue = this->addressToParameterValue;
	
	return copy;
}

NodeType CParam::GetType() {
	return ParamNode;
}

void CParam::PersistData() {
	this->persistedValue = this->ReadValue();
	isDataPersisted = true;
}

std::wstring CParam::GetValue() {
	if (isDataPersisted == true) {
		return this->persistedValue;
	}
	std::wstring result = this->ReadValue();
	return result;
}

std::wstring CParam::ReadValue() {
	switch (this->elementType) {
		case ELEMENT_TYPE_BOOLEAN:
			return CValueReader::GetInstance()->TraceBoolean(this->addressToParameterValue);
		case ELEMENT_TYPE_I:
			return CValueReader::GetInstance()->TraceInt(this->addressToParameterValue);
		case ELEMENT_TYPE_I1:
			return CValueReader::GetInstance()->TraceByte(this->addressToParameterValue);
		case ELEMENT_TYPE_I2:
			return CValueReader::GetInstance()->TraceShort(this->addressToParameterValue);
		case ELEMENT_TYPE_I4:
			return CValueReader::GetInstance()->TraceInt(this->addressToParameterValue);
		case ELEMENT_TYPE_I8:
			return CValueReader::GetInstance()->TraceLong(this->addressToParameterValue);
		case ELEMENT_TYPE_U:
			return CValueReader::GetInstance()->TraceUInt(this->addressToParameterValue);
		case ELEMENT_TYPE_U2:
			return CValueReader::GetInstance()->TraceUShort(this->addressToParameterValue);
		case ELEMENT_TYPE_U4:
			return CValueReader::GetInstance()->TraceUInt(this->addressToParameterValue);
		case ELEMENT_TYPE_U8:
			return CValueReader::GetInstance()->TraceULong(this->addressToParameterValue);
		case ELEMENT_TYPE_STRING:
			return CValueReader::GetInstance()->TraceString(this->addressToParameterValue, this->cpi);
		default:
			throw "Unknown type";
	}
}

