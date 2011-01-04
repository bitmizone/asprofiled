#pragma once

#include "common.h"
#include "cor.h"
#include "Node.h"
#include "ValueReader.h"
class CParam : public CNode
{
public:
	CParam(void);
	~CParam(void);
	WCHAR* paramName;
	WCHAR* paramTypeAsString;
	
	// Metadata signature
	PCCOR_SIGNATURE beginningOfParamSignature;
	
	//UINT data;
	ICorProfilerInfo2* cpi;
	void PersistData();
	CParam* GetCopy();
	std::wstring persistedValue;
	mdParamDef paramToken;
	mdTypeDef paramTypeToken;
	void SetAddressToParameterValue(UINT_PTR address);
	UINT_PTR GetAddressToParameterValue();
	void SetParameterType(CorElementType elementType);
	CorElementType GetParameterType();
	NodeType GetType();
	std::wstring GetValue();
private:
	// Type of parameter
	CorElementType elementType;
	// Address to param's value
	UINT_PTR addressToParameterValue;
	// Used in preserving data during method call
	bool isDataPersisted;
	// Virtual gettter, read parameter value
	std::wstring ReadValue();
	
};
