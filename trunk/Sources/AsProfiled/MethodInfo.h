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
	
	CMethodInfo(FunctionID functionIdArg, CComQIPtr<ICorProfilerInfo2> ICorProfilerInfo2, COR_PRF_FUNCTION_ARGUMENT_INFO* functionArguments);
	~CMethodInfo(void);
	void Initialize();
	void InitializeInternals();

	WCHAR* GetMethodName();

	CorCallingConvention GetCallingConvention();
	ULONG GetArgumentsCount();
	mdTypeDef GetTypeToken();
	// change to internal getters 
	PCCOR_SIGNATURE GetMethodSignatureBlob();
	IMetaDataImport* GetMetaDataImport();
	mdMethodDef GetMethodToken();
	CParam* GetReturnValue();

	std::vector<CParam*>* GetArguments();
	StateOfReadingMethodBlob GetStateOfReadingMethodBlob();

	
private:
	// fields
	CComQIPtr<ICorProfilerInfo2> iCorProfilerInfo2;
	// Object for operating and extracting metadata
	IMetaDataImport* pMetaDataImport;
	// Function global identifier
	FunctionID functionId;
	
	// Method token
	mdMethodDef methodToken;
	// Token for type which owns this method
	mdTypeDef typeDefToken;
	// Binary metadata blob
	PCCOR_SIGNATURE methodSignatureBlob;
	// Size of method's signature blob
	ULONG methodSignatureBlobSize;

	WCHAR* methodName;
	ULONG methodNameLength; // + 1 for '\0'
	// Internal state idicating state of reading metadata blob
	StateOfReadingMethodBlob state;
	// Method's calling convention
	CorCallingConvention callingConvention;
	ULONG argumentsCount;

	CParam* returnValue;
	std::vector<CParam*>* arguments;

	CParamParser* paramParser;
	// Represents a function's arguments
	COR_PRF_FUNCTION_ARGUMENT_INFO* argumentsInfo;

};
