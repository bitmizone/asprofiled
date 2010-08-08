#include "StdAfx.h"
#include "MethodInfo.h"

CMethodInfo::CMethodInfo(FunctionID functionIdArg, 
						 CComQIPtr<ICorProfilerInfo2> ICorProfilerInfo2,
						 COR_PRF_FUNCTION_ARGUMENT_INFO* functionArguments) : 
							functionId(functionIdArg), 
							iCorProfilerInfo2(ICorProfilerInfo2),
							argumentsInfo(functionArguments)
{
	Initialize();
}

CMethodInfo::~CMethodInfo(void)
{
	if (methodName != NULL) {
		delete[] this->methodName;
	}
	if (this->paramParser != NULL) {
		delete this->paramParser;
	}

	if (this->arguments != NULL) {
	}
	if (pMetaDataImport != NULL) {
		pMetaDataImport->Release();
	}
}

void CMethodInfo::Initialize() 
{
	this->methodToken = mdTypeDefNil;
	this->typeDefToken = mdTypeDefNil;	
	this->pMetaDataImport = NULL;
	this->methodSignatureBlob = NULL;
	this->methodName = NULL;
	this->methodNameLength = 0;
	this->state = BEGINNING;
	this->callingConvention = IMAGE_CEE_CS_CALLCONV_MAX; // invalid calling convention
	this->paramParser = NULL;
	this->argumentsCount = 0;
	this->returnValue = NULL;
	this->arguments = NULL;

	HRESULT hr = this->iCorProfilerInfo2->GetTokenAndMetaDataFromFunction(functionId, IID_IMetaDataImport, 
																			(LPUNKNOWN *) &pMetaDataImport, 
																			&methodToken);
	this->paramParser = new CParamParser(*pMetaDataImport);
}

void CMethodInfo::InitializeInternals() 
{
	this->methodName = new WCHAR[NAME_BUFFER_SIZE];
	this->pMetaDataImport->GetMethodProps(methodToken, &this->typeDefToken, this->methodName, 
											NAME_BUFFER_SIZE, &this->methodNameLength, NULL, 
											&this->methodSignatureBlob, &this->methodSignatureBlobSize,
											NULL, NULL);
}

WCHAR* CMethodInfo::GetMethodName() {
	if (this->methodName == NULL) {
		this->InitializeInternals();
	}
	return this->methodName;
}

CorCallingConvention CMethodInfo::GetCallingConvention() {
	if (this->methodSignatureBlob == NULL) {
		this->InitializeInternals();
	}
	if (this->state == BEGINNING) {
		this->methodSignatureBlob += CorSigUncompressData(methodSignatureBlob, (ULONG*) &callingConvention);
		this->state = CALLING_CONVENTION_READ;
	}else {
		this->InitializeInternals();
		return this->GetCallingConvention();
	}

	return callingConvention;
}

ULONG CMethodInfo::GetArgumentsCount() {
	if (this->state == CALLING_CONVENTION_READ) {
		if (this->callingConvention != IMAGE_CEE_CS_CALLCONV_FIELD) {
			this->methodSignatureBlob += CorSigUncompressData(this->methodSignatureBlob, &this->argumentsCount);
		}
		this->state = ARGUMENTS_COUNT_READ;	
	}else if (this->callingConvention == IMAGE_CEE_CS_CALLCONV_MAX) { // assumption: IMAGE_CEE_CS_CALLCONV_MAX is default
		this->GetCallingConvention();
		return this->GetArgumentsCount();
	}
	return this->argumentsCount;
}

CParam* CMethodInfo::GetReturnValue() {
	
	if (this->returnValue != NULL) {
		return this->returnValue;
	}
	if (this->state == ARGUMENTS_COUNT_READ) {
		this->returnValue = new CParam();
		this->methodSignatureBlob = paramParser->ParseSignature(this->methodSignatureBlob, *returnValue);
		this->state = RETURN_VALUE_TYPE_READ;
	}else{
		this->GetArgumentsCount();
		return this->GetReturnValue();
	}
	return this->returnValue;
}


//read agrument value



std::vector<CParam*>* CMethodInfo::GetArguments() {
	if (this->arguments != NULL) {
		return this->arguments;
	}
	if (this->state == RETURN_VALUE_TYPE_READ) {
		this->arguments = new std::vector<CParam*>();
		for ( ULONG i = 0; (this->methodSignatureBlob != NULL) && (i < this->argumentsCount); ++i )
		{
			CParam* argument = new CParam();
			this->methodSignatureBlob = paramParser->ParseSignature(this->methodSignatureBlob, *argument);
			if ( this->methodSignatureBlob != NULL )
			{
				this->arguments->push_back(argument);
			}
		}	
	}else{
		this->GetReturnValue();
		return this->GetArguments();
	}
	HCORENUM parametersEnum = NULL;
	ULONG tempArgumentsCount;
	ULONG parameterNameLength;
	mdParamDef argumentsTokens[MaxParametersCount];
	this->pMetaDataImport->EnumParams(&parametersEnum, this->methodToken, argumentsTokens, MaxParametersCount, &tempArgumentsCount);
	ASSERT(tempArgumentsCount == this->argumentsCount);
	for (ULONG i = 0; i < tempArgumentsCount; ++i) {
		CParam* argument = this->arguments->at(i);
		this->pMetaDataImport->GetParamProps(argumentsTokens[i], NULL, NULL, argument->paramName, NAME_BUFFER_SIZE, &parameterNameLength, NULL, NULL, NULL, NULL);
    }
	return this->arguments;
}


PCCOR_SIGNATURE CMethodInfo::GetMethodSignatureBlob() {
	if (methodSignatureBlob == NULL) {
		this->InitializeInternals();
	}
	return methodSignatureBlob;
}

mdTypeDef CMethodInfo::GetTypeToken() {
	if (this->typeDefToken == mdTypeDefNil) {
		this->InitializeInternals();
	}
	return this->typeDefToken;
}


// Gets metaData object for importing and manipulating existing metadata
IMetaDataImport* CMethodInfo::GetMetaDataImport() 
{
	return this->pMetaDataImport;
}

mdMethodDef CMethodInfo::GetMethodToken() 
{
	return this->methodToken;
}

StateOfReadingMethodBlob CMethodInfo::GetStateOfReadingMethodBlob() {
	return this->state;
}
