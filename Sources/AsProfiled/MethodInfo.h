#pragma once

#include <vector>
#include "cor.h"
#include "corprof.h"
#include "common.h"
#include "ParamParser.h"
#include "Param.h"

enum StateOfReadingMethodBlob { BEGINNING, CALLING_CONVENTION_READ, ARGUMENTS_COUNT_READ, RETURN_VALUE_TYPE_READ, ARGUMENTS_TYPES_READ };

class CMethodInfo
{
public:
	
	CMethodInfo(FunctionID functionIdArg, CComQIPtr<ICorProfilerInfo2> ICorProfilerInfo2);
	~CMethodInfo(void);
	void Initialize();
	void InitializeInternals();

	WCHAR* GetMethodName();

	CorCallingConvention GetCallingConvention();
	ULONG GetArgumentsCount();
	// change to internal getters 
	PCCOR_SIGNATURE GetMethodSignatureBlob();
	IMetaDataImport* GetMetaDataImport();
	mdMethodDef GetMethodToken();
	mdTypeDef GetTypeToken();
	CParam* GetReturnValue();
	std::vector<CParam*>* GetArguments();
	StateOfReadingMethodBlob GetStateOfReadingMethodBlob();
	
private:
	// fields
	CComQIPtr<ICorProfilerInfo2> iCorProfilerInfo2;
	IMetaDataImport* pMetaDataImport;
	FunctionID functionId;
	

	mdMethodDef methodToken;
	mdTypeDef typeDefToken;

	PCCOR_SIGNATURE methodSignatureBlob;
	ULONG methodSignatureBlobSize;

	WCHAR* methodName;
	ULONG methodNameLength; // + 1 for '\0'

	StateOfReadingMethodBlob state;

	CorCallingConvention callingConvention;
	ULONG argumentsCount;

	CParam* returnValue;
	std::vector<CParam*>* arguments;

	CParamParser* paramParser;

};
