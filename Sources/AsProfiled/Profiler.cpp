// profiler.cpp : Implementation of CProfiler

#include "stdafx.h"

#include <assert.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>

#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>


#include "winnt.h"
#include "profiler.h"
#include "common.h"
#include "profiler_helper.h"
#include "astudillo/CGTFile.h"
#include "SimpleErrorRep.h"
#include "ParamParser.h"
#include "GrammarParser.h"

using namespace std;;
using namespace log4cxx;

CProfiler* _cProfilerGlobalHandler = NULL;

LoggerPtr loggerMyMain(Logger::getLogger("main"));


int callCounter = 0;

CProfiler::CProfiler() {
	// Configure Log4cxx
	BasicConfigurator::configure();
	// Levels hierarchy: TRACE < DEBUG < INFO < WARN < ERROR < FATAL
	loggerMyMain->setLevel(Level::toLevel(log4cxx::Level::INFO_INT));
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
	
	LOG4CXX_INFO(loggerMyMain, szMethodName)
	
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
	
	/////////////////// Parsing parameters, not needed. Done elsewhere
	/*for (ULONG i = 0; i < paramsCount; ++i) {
		if (i == 0) 
			cout << "(";
		metaDataImport->GetParamProps(params[i], &mdMethod, &paramPosition, paramName, NAME_BUFFER_SIZE, &paramNameLen, NULL, &corElementType, NULL, NULL);
		PCCOR_SIGNATURE sig;
		ULONG sizeOfSigInBytes;
		
		metaDataImport->GetSigFromToken(params[i], &sig, &sizeOfSigInBytes);
		
		PrintCharArray(paramName);
		if (i == paramsCount - 1) 
			cout << ")";
		else
			cout << ", ";
	}*/
	metaDataImport->CloseEnum(paramsEnum);	

	ModuleID moduleId = NULL;

	ULONG bufferLengthOffset, stringLengthOffset, bufferOffset;
	_ICorProfilerInfo2->GetStringLayout(&bufferLengthOffset, &stringLengthOffset, &bufferOffset);

	//HRESULT hh = _ICorProfilerInfo2->GetFunctionInfo2(functionID, func, &classId, &moduleId, &token, paramsCount, &pcTypeArgs, typeArgs);
	bool enableStringInfo = false;
	for (UINT i = 0 ; i < argumentInfo->numRanges ; i++) {
		COR_PRF_FUNCTION_ARGUMENT_RANGE range = argumentInfo->ranges[i];
		//UINT_PTR* ptr = &range.startAddress;
		// cout << endl  << *ptr;
		// ToBinary(ptr, range.length, 0);
		if (i == 0 || range.length == 0) {
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
			LOG4CXX_DEBUG(loggerMyMain, tempString);
			
			//UINT stringLen = *(tp + stringLengthOffset);
			LOG4CXX_DEBUG(loggerMyMain, "len = " << stringLength)
		}
		if (*id == 0x1000) {
			enableStringInfo = true;
		}

		LOG4CXX_DEBUG(loggerMyMain, "" << *id << "##### " << range.startAddress << " " << range.length);
	}
	cout << endl;
}

void _stdcall FunctionEnterGlobal(FunctionID functionID, UINT_PTR clientData, COR_PRF_FRAME_INFO func, COR_PRF_FUNCTION_ARGUMENT_INFO *argumentInfo) {

	//cout << "function enter global" << endl;
	_cProfilerGlobalHandler->FunctionEnter(functionID, clientData, func, argumentInfo);
	
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

void CProfiler::FunctionLeave(FunctionID functionID, UINT_PTR clientData, COR_PRF_FRAME_INFO func, COR_PRF_FUNCTION_ARGUMENT_RANGE *retvalRange) {
	
}


void _stdcall FunctionLeaveGlobal(FunctionID qfunctionID, UINT_PTR clientData, COR_PRF_FRAME_INFO func, COR_PRF_FUNCTION_ARGUMENT_INFO *argumentInfo) {
	
	
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


HRESULT CProfiler::GetFunctionData(FunctionID functionId) 
{

	return S_OK;
}

HRESULT CProfiler::GetFullMethodName(FunctionID functionId, LPWSTR wszMethod) {
	HRESULT hr = S_OK;
	IMetaDataImport* pMetaDataImport = 0;
	WCHAR szFunction[NAME_BUFFER_SIZE];
	WCHAR szClass[NAME_BUFFER_SIZE];
	mdMethodDef methodToken = mdTypeDefNil;
	mdTypeDef typeDefToken = mdTypeDefNil;
	mdCustomAttribute metadataCustomAttr[10];
	ULONG cchMethod;
	ULONG cchClass;
	ULONG count;
	HCORENUM phEnum = NULL;
	PCCOR_SIGNATURE sigBlob = NULL;
	ULONG sigBlobBytesCount = NULL;

	hr = _ICorProfilerInfo2->GetTokenAndMetaDataFromFunction(functionId, IID_IMetaDataImport, (LPUNKNOWN *) &pMetaDataImport, &methodToken);

	LOG4CXX_DEBUG(loggerMyMain, "methodToken dump:" << ToBinary((void*) &methodToken, 4, 3));
	

	if (SUCCEEDED(hr)) {
		
		hr = pMetaDataImport->GetMethodProps(methodToken, &typeDefToken, szFunction, 
											NAME_BUFFER_SIZE, &cchMethod, 
											NULL, &sigBlob, &sigBlobBytesCount, NULL, NULL);
	
		hr = pMetaDataImport->EnumCustomAttributes(&phEnum, methodToken, 0, metadataCustomAttr, 10, &count);

		while (count > 0) 
		{
			pMetaDataImport->EnumCustomAttributes(&phEnum, methodToken, 0, metadataCustomAttr, 10, &count);
		}

		ULONG lSum = 0;
		pMetaDataImport->CountEnum(phEnum, &lSum);

		LOG4CXX_DEBUG(loggerMyMain, "------Attributes Info------");
		LOG4CXX_DEBUG(loggerMyMain, "No of attributes: " << lSum);

		for (ULONG i = 0 ; i < lSum ; ++i) {

			// get info about custom attribute
			mdCustomAttribute *currentAttribute = &metadataCustomAttr[i];
			
			const void* attributeBlob = NULL;
			ULONG attributeBlobSize = 0;
			mdMethodDef attributeConstructor = mdTokenNil;
			pMetaDataImport->GetCustomAttributeProps(*currentAttribute, &methodToken, &attributeConstructor, &attributeBlob, &attributeBlobSize); 

			LOG4CXX_DEBUG(loggerMyMain, "Attribute ctor token value: "<< hex <<  attributeConstructor);
			LOG4CXX_DEBUG(loggerMyMain, "Custom attribute token value: " << hex << *currentAttribute);

			// should be always equal 1
			INT16 prolog =  *((UINT16*) attributeBlob);
			LOG4CXX_DEBUG(loggerMyMain, "prolog (always=1) = " << prolog);

			PCCOR_SIGNATURE methodMetadataBlob = 0;
			ULONG metaDataBlobSize = 0;
			hr = pMetaDataImport->GetMethodProps(attributeConstructor, 
												NULL, 
												NULL, 
												NULL, 
												NULL, 
												NULL, 
												&methodMetadataBlob, 
												&metaDataBlobSize, 
												NULL, 
												NULL);

			
			CProfilerHelper::GetInstance().ParseCallingConvention(methodMetadataBlob);
			ULONG argsCount = CProfilerHelper::GetInstance().GetArgumentsCount(methodMetadataBlob);	
			
			if (argsCount == 3) {
							

				//////////////////////////////////////////
				///////// GRAMMAR PARSER SECTION//////////
				//std::string filePath = "baba";
				//std::string& filePathRef = filePath; 
				//CGrammarParser* grammarParser = new CGrammarParser(filePathRef);
				//delete grammarParser;
				//CProfilerHelper::GetInstance().ParseAttributeMetaData(attributeBlob, attributeBlobSize);
				//WCHAR* argument = L"bieda > 3";

				//CGTFile    cgtFile;
				//Symbol     *rdc;
				//DFA        *dfa;
				//LALR       *lalr;
				//ErrorTable       *myErrors;
				//  SimpleErrorRep   myReporter; 
				//bool ok = cgtFile.load("c:\\grammar.cgt");
				//std::cout << ok << std::endl;
				//dfa = cgtFile.getScanner();
				//
				//std::cout << "OK2" << std::endl;
				//ok = dfa->scan(argument);
				//myErrors = dfa->getErrors();
  
				//  // If there are errors report them
				//  if (myErrors->errors.size() > 0) {
				//	for (unsigned int i=0; i < myErrors->errors.size(); i++) {
				//		std::cout << myReporter.composeParseErrorMsg (*myErrors->errors[i]) << endl;
				//	}
				//  }

				//std::cout << ok << std::endl;
				////delete [] argument;
				//std::cout << "OK4" << std::endl;
				//vector<Token*> tokens = dfa->getTokens();
				//std::cout << "OK5" << std::endl;
				//lalr = cgtFile.getParser();
				//std::cout << "OK6" << std::endl;
				//rdc = lalr->parse(tokens, true, true);
				//std::cout << "OK7" << std::endl;
				//lalr->printReductionTree(rdc,0);
				//std::cout << "OK8" << std::endl;
				//for ( vector<Token*>::iterator i = tokens.begin(); i != tokens.end(); i++) {
				//	cout << (*i)->kind << endl;
				//}

			}
			
			//mdMethodDef typeToken = mdTokenNil;
			//hr = pMetaDataImport->GetCustomAttributeProps(metadataCustomAttr[i], NULL, &typeToken, NULL, NULL);
			//LPWSTR typeName = new WCHAR[NAME_BUFFER_SIZE];
			//typeName[0] = '\0';
			//hr = pMetaDataImport->GetMethodProps(typeToken, NULL, typeName, NAME_BUFFER_SIZE, NULL, NULL, NULL, NULL, NULL, NULL);
			//hr = pMetaDataImport->GetTypeDefProps(metadataCustomAttr[i], typeName, NAME_BUFFER_SIZE, NULL, NULL, NULL);
			//mdTypeDef typeOfAttribute = mdTokenNil;
			//pMetaDataImport->GetMemberRefProps(typeToken, &typeOfAttribute, typeName, NAME_BUFFER_SIZE, NULL, NULL, NULL);

			//cout << endl;
			//PrintCharArray(typeName);
			//cout << endl;*/

			//LPWSTR typeOwner = new WCHAR[NAME_BUFFER_SIZE];
			//pMetaDataImport->GetTypeRefProps(typeOfAttribute, NULL, typeOwner, NAME_BUFFER_SIZE, NULL);
			//PrintCharArray(typeOwner);
			cout << endl;

		}
		LOG4CXX_DEBUG(loggerMyMain,"------End Of Attribute Info------");
		pMetaDataImport->CloseEnum(phEnum);
		
		LOG4CXX_DEBUG(loggerMyMain, "------Method Info------");
		ULONG callConv = 0;
		ULONG paramsCount = 0;
		// call convention 
		sigBlob += CorSigUncompressData(sigBlob, &callConv);
		if ( callConv != IMAGE_CEE_CS_CALLCONV_FIELD) {
			sigBlob += CorSigUncompressData(sigBlob,&paramsCount);
			LOG4CXX_DEBUG(loggerMyMain, "# of arguments: " << paramsCount);
		}
		LPWSTR returnType = new WCHAR[NAME_BUFFER_SIZE];
		returnType[0] = '\0';
		// get function's return type
		CParamParser* paramParser = new CParamParser(*pMetaDataImport);
		sigBlob = paramParser->ParseSignature(sigBlob, returnType);

		LOG4CXX_DEBUG(loggerMyMain, returnType);
		
		// get function's arguments type
		for ( ULONG i = 0; (sigBlob != NULL) && (i < paramsCount); ++i )
		{
			LPWSTR parameters = new WCHAR[NAME_BUFFER_SIZE];
			parameters[0] = '\0';
			sigBlob = paramParser->ParseSignature(sigBlob, parameters);

			if ( sigBlob != NULL )
			{
				LOG4CXX_DEBUG(loggerMyMain,parameters);
			}
		}	


		LOG4CXX_DEBUG(loggerMyMain, ToBinary((void*) &typeDefToken, 4, 3));

		if (SUCCEEDED(hr)) 
		{
			hr = pMetaDataImport->GetTypeDefProps(typeDefToken, szClass, NAME_BUFFER_SIZE, &cchClass, NULL, NULL);
			if (SUCCEEDED(hr)) {
				
				_snwprintf_s(wszMethod,NAME_BUFFER_SIZE, NAME_BUFFER_SIZE ,L"%s.%s",szClass,szFunction);
				//LOG4CXX_INFO(loggerMyMain, wszMethod);
			}
			
			if (SUCCEEDED(hr)) {
				 // printf("%p ", pchData);
			}

			ULONG callingConvetion = 0;
			sigBlob += CorSigUncompressData(sigBlob, &callingConvetion);
			// getMethodProps error
		}
		LOG4CXX_DEBUG(loggerMyMain,"------End Of Method Info------");
		// get token and metadata from function error
			
	}
	else 
	{
		// log error
	}

	pMetaDataImport->Release();
	return hr;
}




STDMETHODIMP CProfiler::Shutdown() {
	LOG4CXX_DEBUG(loggerMyMain, "Program has ended");
	LOG4CXX_DEBUG(loggerMyMain, "Function call counter is " << callCounter);
	return S_OK;
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
	LOG4CXX_DEBUG(loggerMyMain, "Program has started");
    return S_OK;
} 

