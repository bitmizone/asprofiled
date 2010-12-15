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
	//ULONG ReadData(ULONG_PTR startAddress);
	NodeType GetType();
	std::wstring GetValue();

	WCHAR* paramName;
	WCHAR* paramTypeAsString;
	CorElementType elementType;
	// Metadata signature
	PCCOR_SIGNATURE beginningOfParamSignature;
	// Parameter value
	UINT_PTR addressToParameterValue;
	ULONG dataLength;
	UINT data;
	ICorProfilerInfo2* cpi;
	void PersistData();
	CParam* GetCopy();
	std::wstring persistedValue;
	mdParamDef paramToken;
	mdTypeDef paramTypeToken;
private:
	bool isDataPersisted;
	std::wstring ReadValue();
	
};
