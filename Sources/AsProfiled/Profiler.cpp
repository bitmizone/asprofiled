// profiler.cpp : Implementation of CProfiler

#include "stdafx.h"
#include "profiler.h"
#include "MethodInfo.h"

using namespace std;;
using namespace log4cxx;

CProfiler* _cProfilerGlobalHandler = NULL;
LoggerPtr myMainLogger(Logger::getLogger("main"));

CProfiler::CProfiler() 
{
	// Configure Log4cxx
	BasicConfigurator::configure();
	// Levels hierarchy: TRACE < DEBUG < INFO < WARN < ERROR < FATAL
	myMainLogger->setLevel(Level::toLevel(log4cxx::Level::INFO_INT));
}


// Static function mapper 
// We might need to map functions in order to track them
UINT_PTR CProfiler::FunctionMapper(FunctionID functionId, BOOL *pbHookFunction) {
	
	if (_cProfilerGlobalHandler != NULL) {
		_cProfilerGlobalHandler->MapFunction(functionId);
	}
	return (UINT_PTR) functionId;
}

// Do whatever we want with function ID
void CProfiler::MapFunction(FunctionID functionId) {
		//cout << "map function" << endl;

}

// Function called by .NET runtime when function is invoked
void CProfiler::FunctionEnter(FunctionID functionID, UINT_PTR clientData, COR_PRF_FRAME_INFO func, COR_PRF_FUNCTION_ARGUMENT_INFO *argumentInfo) {
	
	// buffer for function name
	WCHAR szMethodName[NAME_BUFFER_SIZE];
	// read function name
	HRESULT hr = GetFullMethodName(functionID, szMethodName);
	
	LOG4CXX_INFO(myMainLogger, szMethodName)
	
	// Declaration of pointer to IMetaDataImport interface, 
	// which provides methods for importing and manipulating existing metadata
	IMetaDataImport* metaDataImport = NULL;
	// A pointer that references the metadata for called function 
	mdToken token = mdTokenNil;
	mdMethodDef mdMethod = mdTypeDefNil;
	// Enumerator for function's parameters
	HCORENUM paramsEnum = NULL;
	// Arguments count
	ULONG paramsCount = 0;
	// Token to param's metadata
	mdParamDef params[1024];
	// Param postion in function signature
	ULONG paramPosition = 0;
	// Parameter name lenght
	ULONG paramNameLen = 0;
	// Parameter's name
	LPWSTR paramName = 0;
	// Parameter's type
	DWORD corElementType = 0;
	
	// Retrieve function's metadata 
	 _ICorProfilerInfo2->GetTokenAndMetaDataFromFunction(functionID, IID_IMetaDataImport, (IUnknown**) &metaDataImport, &token);
	
	// Get enumerator to interate over parameters
	metaDataImport->EnumParams(&paramsEnum, token, params, 1024, &paramsCount);
	ULONG32 pcTypeArgs = 0;		
	metaDataImport->CloseEnum(paramsEnum);	

	ULONG bufferLengthOffset, stringLengthOffset, bufferOffset;
	_ICorProfilerInfo2->GetStringLayout(&bufferLengthOffset, &stringLengthOffset, &bufferOffset);

	// HRESULT hh = _ICorProfilerInfo2->GetFunctionInfo2(functionID, func, &classId, &moduleId, &token, paramsCount, &pcTypeArgs, typeArgs);
	bool enableStringInfo = false;
	for (UINT i = 0 ; i < argumentInfo->numRanges ; i++) {
		COR_PRF_FUNCTION_ARGUMENT_RANGE range = argumentInfo->ranges[i];

		if (i == 0 || range.length == 0) 
		{
			continue;
		}
		ObjectID* id = reinterpret_cast<ObjectID*>( range.startAddress);
		ULONG size = 0;
		if (enableStringInfo) {
			/*DWORD stringLen = 0;
			UINT_PTR* tp = reinterpret_cast<UINT_PTR*>(id);*/
			ObjectID stringOID;
			DWORD stringLength;
			WCHAR tempString[NAME_BUFFER_SIZE];  
			memcpy(&stringOID, ((const void *)(range.startAddress)), range.length);
			memcpy(&stringLength, ((const void *)(stringOID + stringLengthOffset)), sizeof(DWORD));
			memcpy(tempString, ((const void *)(stringOID + bufferOffset)), stringLength * sizeof(DWORD));
			//tempString[stringLength * sizeof(DWORD)] = '\0'; 
			LOG4CXX_DEBUG(myMainLogger, tempString);
			
			//UINT stringLen = *(tp + stringLengthOffset);
			LOG4CXX_DEBUG(myMainLogger, "len = " << stringLength)
		}
		if (*id == 0x1000) {
			enableStringInfo = true;
		}
	}
}

void CProfiler::FunctionLeave(FunctionID functionID, UINT_PTR clientData, COR_PRF_FRAME_INFO func, COR_PRF_FUNCTION_ARGUMENT_RANGE *retvalRange) {
	
}


HRESULT CProfiler::GetFunctionData(FunctionID functionId) 
{
	return S_OK;
}

HRESULT CProfiler::GetFullMethodName(FunctionID functionId, LPWSTR wszMethod) {
	HRESULT hr = S_OK;
	IMetaDataImport* pMetaDataImport = 0;
	WCHAR* szFunction;
	WCHAR szClass[NAME_BUFFER_SIZE];
	mdMethodDef methodToken = mdTypeDefNil;
	mdTypeDef typeDefToken = mdTypeDefNil;	
	ULONG cchClass;
	
	PCCOR_SIGNATURE sigBlob = NULL;
	CMethodInfo methodInfo(functionId, _ICorProfilerInfo2);
	methodInfo.Initialize();

	pMetaDataImport = methodInfo.GetMetaDataImport();
	methodToken = methodInfo.GetMethodToken();
	
	if (SUCCEEDED(hr)) {
		
		sigBlob = methodInfo.GetMethodSignatureBlob();
		szFunction = methodInfo.GetMethodName();
		typeDefToken = methodInfo.GetTypeToken();
		this->attributeReader->Initialize(methodToken, pMetaDataImport);
		this->attributeReader->PrintAttributesInfo();

		ULONG callConv = methodInfo.GetCallingConvention();
		ULONG paramsCount = 0;

		// Get calling convention 
		sigBlob = methodInfo.GetMethodSignatureBlob();
		paramsCount = methodInfo.GetArgumentsCount();
		sigBlob = methodInfo.GetMethodSignatureBlob();
		LOG4CXX_DEBUG(myMainLogger, "# of arguments: " << methodInfo.GetArgumentsCount());
		
		// Get function's return type
		LPWSTR returnType = new WCHAR[NAME_BUFFER_SIZE];
		returnType[0] = '\0';
		CParamParser* paramParser = new CParamParser(*pMetaDataImport);
		CParam* param = methodInfo.GetReturnValue();
		sigBlob = methodInfo.GetMethodSignatureBlob();
		LOG4CXX_DEBUG(myMainLogger, param->paramType);
		

		// Get method's arguments
		std::vector<CParam*>* arguments = methodInfo.GetArguments();
		sigBlob = methodInfo.GetMethodSignatureBlob();
		for (vector<CParam*>::iterator iter = arguments->begin(); iter != arguments->end(); iter++) {
			LOG4CXX_INFO(myMainLogger, (*iter)->paramType);
		}

				
		LOG4CXX_DEBUG(myMainLogger, ToBinary((void*) &typeDefToken, 4, 3));

		if (SUCCEEDED(hr)) 
		{
			hr = pMetaDataImport->GetTypeDefProps(typeDefToken, szClass, NAME_BUFFER_SIZE, &cchClass, NULL, NULL);
			if (SUCCEEDED(hr)) {
				_snwprintf_s(wszMethod,NAME_BUFFER_SIZE, NAME_BUFFER_SIZE ,L"%s.%s",szClass,szFunction);
			}

			ULONG callingConvetion = 0;
			sigBlob += CorSigUncompressData(sigBlob, &callingConvetion);
			// getMethodProps error
		}
			
	}
	else 
	{
		// log error
	}

	pMetaDataImport->Release();
	return hr;
}


void _stdcall FunctionEnterGlobal(FunctionID functionID, UINT_PTR clientData, COR_PRF_FRAME_INFO func, COR_PRF_FUNCTION_ARGUMENT_INFO *argumentInfo) {
	_cProfilerGlobalHandler->FunctionEnter(functionID, clientData, func, argumentInfo);
	
}

void _stdcall FunctionLeaveGlobal(FunctionID qfunctionID, UINT_PTR clientData, COR_PRF_FRAME_INFO func, COR_PRF_FUNCTION_ARGUMENT_INFO *argumentInfo) {
	
	
}

// restore call stack
void _declspec(naked) FunctionEnterHandler(FunctionID functionID, UINT_PTR clientData, COR_PRF_FRAME_INFO func, COR_PRF_FUNCTION_ARGUMENT_INFO *argumentInfo) 
{
	__asm 
	{
		push	ebp // create stack frame
		mov		ebp, esp // move current stack pointer to ebp
		pushad
		mov		eax, [ebp+0x14]
		push	eax
		mov		eax, [ebp+0x10]
		push	eax
		mov		eax, [ebp+0x0C]
		push	eax
		mov		eax, [ebp+0x08]
		push	eax
		call	FunctionEnterGlobal
		popad
		pop		ebp
		ret		16
	}
}


void _declspec(naked) FunctionLeaveHandler(FunctionID functionID, UINT_PTR clientData, COR_PRF_FRAME_INFO func, COR_PRF_FUNCTION_ARGUMENT_RANGE *retvalRange)
{
	__asm 
	{
		push	ebp
		mov		ebp,esp
		pushad
		mov		eax, [ebp+0x14]
		push	eax
		mov		eax, [ebp+0x10]
		push	eax
		mov		eax, [ebp+0x0C]
		push	eax
		mov		eax, [ebp+0x08]
		push	eax
		call	FunctionLeaveGlobal
		popad
		pop		ebp
		ret		16
	}
}

HRESULT CProfiler::SetEventMask()
{
	//COR_PRF_MONITOR_NONE	= 0,
	//COR_PRF_MONITOR_FUNCTION_UNLOADS	= 0x1,
	//COR_PRF_MONITOR_CLASS_LOADS	= 0x2,
	//COR_PRF_MONITOR_MODULE_LOADS	= 0x4,
	//COR_PRF_MONITOR_ASSEMBLY_LOADS	= 0x8,
	//COR_PRF_MONITOR_APPDOMAIN_LOADS	= 0x10,
	//COR_PRF_MONITOR_JIT_COMPILATION	= 0x20,
	//COR_PRF_MONITOR_EXCEPTIONS	= 0x40,
	//COR_PRF_MONITOR_GC	= 0x80,
	//COR_PRF_MONITOR_OBJECT_ALLOCATED	= 0x100,
	//COR_PRF_MONITOR_THREADS	= 0x200,
	//COR_PRF_MONITOR_REMOTING	= 0x400,
	//COR_PRF_MONITOR_CODE_TRANSITIONS	= 0x800,
	//COR_PRF_MONITOR_ENTERLEAVE	= 0x1000,
	//COR_PRF_MONITOR_CCW	= 0x2000,
	//COR_PRF_MONITOR_REMOTING_COOKIE	= 0x4000 | COR_PRF_MONITOR_REMOTING,
	//COR_PRF_MONITOR_REMOTING_ASYNC	= 0x8000 | COR_PRF_MONITOR_REMOTING,
	//COR_PRF_MONITOR_SUSPENDS	= 0x10000,
	//COR_PRF_MONITOR_CACHE_SEARCHES	= 0x20000,
	//COR_PRF_MONITOR_CLR_EXCEPTIONS	= 0x1000000,
	//COR_PRF_MONITOR_ALL	= 0x107ffff,
	//COR_PRF_ENABLE_REJIT	= 0x40000,
	//COR_PRF_ENABLE_INPROC_DEBUGGING	= 0x80000,
	//COR_PRF_ENABLE_JIT_MAPS	= 0x100000,
	//COR_PRF_DISABLE_INLINING	= 0x200000,
	//COR_PRF_DISABLE_OPTIMIZATIONS	= 0x400000,
	//COR_PRF_ENABLE_OBJECT_ALLOCATED	= 0x800000,
	
	// New in VS2005
	//COR_PRF_ENABLE_FUNCTION_ARGS	= 0x2000000,
	//COR_PRF_ENABLE_FUNCTION_RETVAL	= 0x4000000,
	//COR_PRF_ENABLE_FRAME_INFO	= 0x8000000,
	//COR_PRF_ENABLE_STACK_SNAPSHOT	= 0x10000000,
	//COR_PRF_USE_PROFILE_IMAGES	= 0x20000000,
	// End New in VS2005
	
	//COR_PRF_ALL	= 0x3fffffff,
	//COR_PRF_MONITOR_IMMUTABLE	= COR_PRF_MONITOR_CODE_TRANSITIONS | COR_PRF_MONITOR_REMOTING | COR_PRF_MONITOR_REMOTING_COOKIE | COR_PRF_MONITOR_REMOTING_ASYNC | COR_PRF_MONITOR_GC | COR_PRF_ENABLE_REJIT | COR_PRF_ENABLE_INPROC_DEBUGGING | COR_PRF_ENABLE_JIT_MAPS | COR_PRF_DISABLE_OPTIMIZATIONS | COR_PRF_DISABLE_INLINING | COR_PRF_ENABLE_OBJECT_ALLOCATED | COR_PRF_ENABLE_FUNCTION_ARGS | COR_PRF_ENABLE_FUNCTION_RETVAL | COR_PRF_ENABLE_FRAME_INFO | COR_PRF_ENABLE_STACK_SNAPSHOT | COR_PRF_USE_PROFILE_IMAGES

	// set the event mask 
	DWORD eventMask = (DWORD)(COR_PRF_MONITOR_ENTERLEAVE);
	return _ICorProfilerInfo2->SetEventMask(COR_PRF_MONITOR_ENTERLEAVE | 
											COR_PRF_ENABLE_FUNCTION_RETVAL |  
											COR_PRF_ENABLE_FUNCTION_ARGS |
											COR_PRF_ENABLE_FRAME_INFO);
}

STDMETHODIMP CProfiler::Initialize(IUnknown *pICorProfilerInfoUnk)
{
	_cProfilerGlobalHandler = this;
	// Query for ICorProfiler2 object
	HRESULT hr =  pICorProfilerInfoUnk->QueryInterface(IID_ICorProfilerInfo2, (LPVOID*) &_ICorProfilerInfo2);
	// Register for recieving specified events
	hr = SetEventMask();

	// Register handlers for function enter/leave events
	_ICorProfilerInfo2->SetEnterLeaveFunctionHooks2( &FunctionEnterHandler, &FunctionLeaveHandler, NULL);
	
	// Setting function mapper
	_ICorProfilerInfo2->SetFunctionIDMapper(FunctionMapper);
	LOG4CXX_DEBUG(myMainLogger, "Program has started");
	
	LOG4CXX_DEBUG(myMainLogger, "Initializing grammar parser...");
	string filePath = "C:\\grammar";
	grammarParser = new CGrammarParser(filePath);
	bool success = grammarParser->Initialize();
	if (success == false) {
		// LOG4CXX_FATAL(myMainLogger, "File "  << filePath.c_str() << " not found");
		//exit(1);
	}

	LOG4CXX_TRACE(myMainLogger, "Initializing attribute reader");
	attributeReader = new CAttributeReader();

    return S_OK;
} 

STDMETHODIMP CProfiler::Shutdown() {
	delete grammarParser;
	delete attributeReader;
	LOG4CXX_DEBUG(myMainLogger, "Program has ended");
	return S_OK;
}
