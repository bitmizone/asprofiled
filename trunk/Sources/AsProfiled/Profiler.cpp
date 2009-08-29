// profiler.cpp : Implementation of CProfiler
#include "winnt.h"
#include "stdafx.h"
#include "profiler.h"
#include "common.h"
#include <assert.h>
#include <iostream>
#include <fstream>

using namespace std;;


CProfiler* _cProfilerGlobalHandler = NULL;

int callCounter = 0;

CProfiler::CProfiler() {
	
}



// static function mapper 
UINT_PTR CProfiler::FunctionMapper(FunctionID functionId, BOOL *pbHookFunction) {
	
	if (_cProfilerGlobalHandler != NULL) {
		_cProfilerGlobalHandler->MapFunction(functionId);
	}
	return (UINT_PTR) functionId;
}

void CProfiler::MapFunction(FunctionID functionId) {
	WCHAR szMethodName[NAME_BUFFER_SIZE];

	cout << "map function" << endl;
	HRESULT hr = GetFullMethodName(functionId, szMethodName);
	hr = GetFunctionData(functionId);
	PrintCharArray(szMethodName);
}

void CProfiler::FunctionEnter(FunctionID functionID, UINT_PTR clientData, COR_PRF_FRAME_INFO func, COR_PRF_FUNCTION_ARGUMENT_INFO *argumentInfo) {
	//cout << "function enter" << endl;
	cout << (int)  argumentInfo->numRanges << endl;
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
	ULONG pchData;
	ULONG count;
	const void* ppData;
	HCORENUM phEnum = NULL;
	

	hr = _ICorProfilerInfo2->GetTokenAndMetaDataFromFunction(functionId, IID_IMetaDataImport, (LPUNKNOWN *) &pMetaDataImport, &methodToken);
#ifdef DEBUG_ALL
	ToBinary((void*) &methodToken, 4, 3);
#endif
	if (SUCCEEDED(hr)) {

		hr = pMetaDataImport->GetMethodProps(methodToken, &typeDefToken, szFunction, 
											NAME_BUFFER_SIZE, &cchMethod, 
											NULL, NULL, NULL, NULL, NULL);
		// DEBUG PURPOSES
#ifdef DEBUG_ALL
		ToBinary((void*) &typeDefToken, 4, 3);
#endif
		if (SUCCEEDED(hr)) 
		{
			hr = pMetaDataImport->GetTypeDefProps(typeDefToken, szClass, NAME_BUFFER_SIZE, &cchClass, NULL, NULL);
			if (SUCCEEDED(hr)) {
				_snwprintf_s(wszMethod,NAME_BUFFER_SIZE, NAME_BUFFER_SIZE ,L"%s.%s",szClass,szFunction);
			}
			
			hr = pMetaDataImport->EnumCustomAttributes(&phEnum, typeDefToken, 0, metadataCustomAttr, 10, &count);
			int sum = count;
			while (count > 0) 
			{
				pMetaDataImport->EnumCustomAttributes(&phEnum, methodToken, 0, metadataCustomAttr, 10, &count);
				sum += count;
			}
			ULONG lSum = 0;
			pMetaDataImport->CountEnum(phEnum, &lSum);
			pMetaDataImport->CloseEnum(phEnum);
			
			if (SUCCEEDED(hr)) {
				 // printf("%p ", pchData);
			}
			// getMethodProps error
		}
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
	cout << "Program has ended" << endl;
	cout << "Function call counter is " << callCounter << endl;
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
	//	COR_PRF_ENABLE_FUNCTION_ARGS	= 0x2000000,
	//	COR_PRF_ENABLE_FUNCTION_RETVAL	= 0x4000000,
	//  COR_PRF_ENABLE_FRAME_INFO	= 0x8000000,
	//  COR_PRF_ENABLE_STACK_SNAPSHOT	= 0x10000000,
	//  COR_PRF_USE_PROFILE_IMAGES	= 0x20000000,
	// End New in VS2005
	//COR_PRF_ALL	= 0x3fffffff,
	//COR_PRF_MONITOR_IMMUTABLE	= COR_PRF_MONITOR_CODE_TRANSITIONS | COR_PRF_MONITOR_REMOTING | COR_PRF_MONITOR_REMOTING_COOKIE | COR_PRF_MONITOR_REMOTING_ASYNC | COR_PRF_MONITOR_GC | COR_PRF_ENABLE_REJIT | COR_PRF_ENABLE_INPROC_DEBUGGING | COR_PRF_ENABLE_JIT_MAPS | COR_PRF_DISABLE_OPTIMIZATIONS | COR_PRF_DISABLE_INLINING | COR_PRF_ENABLE_OBJECT_ALLOCATED | COR_PRF_ENABLE_FUNCTION_ARGS | COR_PRF_ENABLE_FUNCTION_RETVAL | COR_PRF_ENABLE_FRAME_INFO | COR_PRF_ENABLE_STACK_SNAPSHOT | COR_PRF_USE_PROFILE_IMAGES

	// set the event mask 
	DWORD eventMask = (DWORD)(COR_PRF_MONITOR_ENTERLEAVE);
	return _ICorProfilerInfo2->SetEventMask(COR_PRF_MONITOR_ENTERLEAVE | 
											COR_PRF_ENABLE_FUNCTION_RETVAL |  
											COR_PRF_ENABLE_FUNCTION_ARGS);
}



STDMETHODIMP CProfiler::Initialize(IUnknown *pICorProfilerInfoUnk)
{
	_cProfilerGlobalHandler = this;
	// query for ICorProfiler2 object
	HRESULT hr =  pICorProfilerInfoUnk->QueryInterface(IID_ICorProfilerInfo2, (LPVOID*) &_ICorProfilerInfo2);
	// register for recieving specified events
	hr = SetEventMask();
	// register handlers for function enter/leave events
	
	_ICorProfilerInfo2->SetEnterLeaveFunctionHooks2( &FunctionEnterHandler, &FunctionLeaveHandler, NULL);

	_ICorProfilerInfo2->SetFunctionIDMapper(FunctionMapper);
	cout << "Program has started" << endl;
    return S_OK;
} 