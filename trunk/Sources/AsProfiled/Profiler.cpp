// profiler.cpp : Implementation of CProfiler
#include "winnt.h"
#include "stdafx.h"
#include "profiler.h"
#include "common.h"
#include "profiler_helper.h"
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
	

	//cout << "map function" << endl;

}

void CProfiler::FunctionEnter(FunctionID functionID, UINT_PTR clientData, COR_PRF_FRAME_INFO func, COR_PRF_FUNCTION_ARGUMENT_INFO *argumentInfo) {
	WCHAR szMethodName[NAME_BUFFER_SIZE];
	HRESULT hr = GetFullMethodName(functionID, szMethodName);
	PrintCharArray(szMethodName);
	
	IMetaDataImport* metaDataImport = NULL;
	mdToken token = mdTokenNil;
	HCORENUM paramsEnum = NULL;
	ULONG paramsCount = 0;
	mdParamDef params[1024];
	ClassID classId = NULL;
	mdMethodDef mdMethod = mdTypeDefNil;
	ULONG paramPosition = 0;
	WCHAR paramName[NAME_BUFFER_SIZE];
	
	ULONG paramNameLen = 0;
	DWORD corElementType = 0;

	_ICorProfilerInfo2->GetTokenAndMetaDataFromFunction(functionID, IID_IMetaDataImport, (IUnknown**) &metaDataImport, &token);



	metaDataImport->EnumParams(&paramsEnum, token, params, 1024, &paramsCount);

	for (ULONG i = 0; i < paramsCount; ++i) {
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
	ULONG pchData;
	ULONG count;
	const void* ppData;
	HCORENUM phEnum = NULL;
	PCCOR_SIGNATURE sigBlob = NULL;
	ULONG sigBlobBytesCount = NULL;

	hr = _ICorProfilerInfo2->GetTokenAndMetaDataFromFunction(functionId, IID_IMetaDataImport, (LPUNKNOWN *) &pMetaDataImport, &methodToken);
#ifdef DEBUG_ALL
	ToBinary((void*) &methodToken, 4, 3);
#endif
	if (SUCCEEDED(hr)) {

		hr = pMetaDataImport->GetMethodProps(methodToken, &typeDefToken, szFunction, 
											NAME_BUFFER_SIZE, &cchMethod, 
											NULL, &sigBlob, &sigBlobBytesCount, NULL, NULL);
	
		hr = pMetaDataImport->EnumCustomAttributes(&phEnum, methodToken, 0, metadataCustomAttr, 10, &count);
			int sum = count;
			while (count > 0) 
			{
				pMetaDataImport->EnumCustomAttributes(&phEnum, methodToken, 0, metadataCustomAttr, 10, &count);
				sum += count;
			}
			for (int i = 0 ; i < sum > 0 ; ++i) {
				UINT16 prolog =  *((UINT16*) metadataCustomAttr);
				//should be always equal 1
				cout << "prolog = " << prolog;
			}
			cout << "No of attributes: " << sum << endl;
			ULONG lSum = 0;
			pMetaDataImport->CountEnum(phEnum, &lSum);
			pMetaDataImport->CloseEnum(phEnum);

		ULONG callConv = 0;
		ULONG paramsCount = 0;
		// call convention 
		sigBlob += CorSigUncompressData(sigBlob, &callConv);
		if ( callConv != IMAGE_CEE_CS_CALLCONV_FIELD) {
			sigBlob += CorSigUncompressData(sigBlob,&paramsCount);
			cout << paramsCount << endl;
		}
		LPWSTR returnType = new WCHAR[NAME_BUFFER_SIZE];
		returnType[0] = '\0';
		sigBlob = ParseSignature(pMetaDataImport, sigBlob, returnType);
		PrintCharArray(returnType);
		cout << " ";

		for ( ULONG i = 0; (sigBlob != NULL) && (i < paramsCount); ++i )
		{
			LPWSTR parameters = new WCHAR[NAME_BUFFER_SIZE];
			parameters[0] = '\0';
			sigBlob = ParseSignature(pMetaDataImport, sigBlob, parameters);

			if ( sigBlob != NULL )
			{
				PrintCharArray(parameters);
				cout << " ";
			}
		}	
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
			
			if (SUCCEEDED(hr)) {
				 // printf("%p ", pchData);
			}

			ULONG callingConvetion = 0;
			sigBlob += CorSigUncompressData(sigBlob, &callingConvetion);
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

PCCOR_SIGNATURE CProfiler::ParseSignature( IMetaDataImport *metaDataImport, PCCOR_SIGNATURE signature, LPWSTR signatureText)
{	
	COR_SIGNATURE corSignature = *signature++;

	switch (corSignature) 
	{	
	case ELEMENT_TYPE_VOID:
		wcscat(signatureText, L"void");
		break;							
	case ELEMENT_TYPE_BOOLEAN:	
		wcscat(signatureText, L"bool");	
		break;			
	case ELEMENT_TYPE_CHAR:
		wcscat(signatureText, L"wchar");	
		break;							
	case ELEMENT_TYPE_I1:
		wcscat(signatureText, L"int8");	
		break;		 		
	case ELEMENT_TYPE_U1:
		wcscat(signatureText, L"unsigned int8");	
		break;	 		
	case ELEMENT_TYPE_I2:
		wcscat(signatureText, L"int16");	
		break;		
	case ELEMENT_TYPE_U2:
		wcscat(signatureText, L"unsigned int16");	
		break;					
	case ELEMENT_TYPE_I4:
		wcscat(signatureText, L"int32");	
		break;        
	case ELEMENT_TYPE_U4:
		wcscat(signatureText, L"unsigned int32");	
		break;				
	case ELEMENT_TYPE_I8:
		wcscat(signatureText, L"int64");	
		break;				
	case ELEMENT_TYPE_U8:
		wcscat(signatureText, L"unsigned int64");	
		break;		
	case ELEMENT_TYPE_R4:
		wcscat(signatureText, L"float32");	
		break;					
	case ELEMENT_TYPE_R8:
		wcscat(signatureText, L"float64");	
		break;			 		
	case ELEMENT_TYPE_STRING:
		wcscat(signatureText, L"String");	
		break;	
	case ELEMENT_TYPE_VAR:
		wcscat(signatureText, L"class variable(unsigned int8)");	
		break;		
	case ELEMENT_TYPE_MVAR:
		wcscat(signatureText, L"method variable(unsigned int8)");	
		break;					         
	case ELEMENT_TYPE_TYPEDBYREF:
		wcscat(signatureText, L"refany");	
		break;		 		
	case ELEMENT_TYPE_I:
		wcscat(signatureText, L"int");	
		break;			
	case ELEMENT_TYPE_U:
		wcscat(signatureText, L"unsigned int");	
		break;			  		
	case ELEMENT_TYPE_OBJECT:
		wcscat(signatureText, L"Object");	
		break;		       
	case ELEMENT_TYPE_SZARRAY:	 
		signature = ParseSignature(metaDataImport, signature, signatureText); 
		wcscat(signatureText, L"[]");
		break;				
	case ELEMENT_TYPE_PINNED:
		signature = ParseSignature(metaDataImport, signature, signatureText); 
		wcscat(signatureText, L"pinned");	
		break;	        
	case ELEMENT_TYPE_PTR:   
		signature = ParseSignature(metaDataImport, signature, signatureText); 
		wcscat(signatureText, L"*");	
		break;           
	case ELEMENT_TYPE_BYREF:   
		signature = ParseSignature(metaDataImport, signature, signatureText); 
		wcscat(signatureText, L"&");	
		break;
	case ELEMENT_TYPE_VALUETYPE:   
	case ELEMENT_TYPE_CLASS:	
	case ELEMENT_TYPE_CMOD_REQD:
	case ELEMENT_TYPE_CMOD_OPT:
		{
			mdToken	token;	
			signature += CorSigUncompressToken( signature, &token ); 

			WCHAR className[ NAME_BUFFER_SIZE ];
			if ( TypeFromToken( token ) == mdtTypeRef )
			{
				metaDataImport->GetTypeRefProps(token, NULL, className, NAME_BUFFER_SIZE, NULL);
			}
			else
			{
				metaDataImport->GetTypeDefProps(token, className, NAME_BUFFER_SIZE, NULL, NULL, NULL );
			}

			wcscat(signatureText, className );
		}
		break;		
	case ELEMENT_TYPE_GENERICINST:
		{
			signature = ParseSignature(metaDataImport, signature, signatureText); 

			wcscat(signatureText, L"<");	
			ULONG arguments = CorSigUncompressData(signature);
			for (ULONG i = 0; i < arguments; ++i)
			{
				if(i != 0)
				{
					wcscat(signatureText, L", ");	
				}

				signature = ParseSignature(metaDataImport, signature, signatureText);
			}
			wcscat(signatureText, L">");	
		}
		break;		        
	case ELEMENT_TYPE_ARRAY:	
		{
			signature = ParseSignature(metaDataImport, signature, signatureText);              
			ULONG rank = CorSigUncompressData(signature);													
			if ( rank == 0 ) 
			{
				wcscat(signatureText, L"[?]");
			}
			else 
			{		
				ULONG arraysize = (sizeof(ULONG) * 2 * rank);
				ULONG *lower = (ULONG *)_alloca(arraysize);
				memset(lower, 0, arraysize); 
				ULONG *sizes = &lower[rank];

				ULONG numsizes = CorSigUncompressData(signature);	
				for (ULONG i = 0; i < numsizes && i < rank; i++)
				{
					sizes[i] = CorSigUncompressData(signature);	
				}

				ULONG numlower = CorSigUncompressData(signature);	
				for (ULONG i = 0; i < numlower && i < rank; i++)	
				{
					lower[i] = CorSigUncompressData( signature ); 
				}

				wcscat(signatureText, L"[");	
				for (ULONG i = 0; i < rank; ++i)	
				{					
					if (i > 0) 
					{
						wcscat(signatureText, L",");
					}

					if (lower[i] == 0)
					{
						if(sizes[i] != 0)
						{
							WCHAR *size = new WCHAR[NAME_BUFFER_SIZE];
							size[0] = '\0';
							wsprintf(size, L"%d", sizes[i]);											
							wcscat(signatureText, size);
						}
					}	
					else	
					{
						WCHAR *low = new WCHAR[NAME_BUFFER_SIZE];
						low[0] = '\0';
						wsprintf(low, L"%d", lower[i]);
						wcscat(signatureText, low);
						wcscat(signatureText, L"...");	

						if (sizes[i] != 0)	
						{
							WCHAR *size = new WCHAR[NAME_BUFFER_SIZE];
							size[0] = '\0';
							wsprintf(size, L"%d", (lower[i] + sizes[i] + 1));
							wcscat(signatureText, size);
						}
					}
				}
				wcscat(signatureText, L"]");
			}
		} 
		break;   		    
	default:	
	case ELEMENT_TYPE_END:	
	case ELEMENT_TYPE_SENTINEL:	
		WCHAR *elementType = new WCHAR[NAME_BUFFER_SIZE];
		elementType[0] = '\0';
		wsprintf(elementType, L"<UNKNOWN:0x%X>", corSignature);
		wcscat(signatureText, elementType);
		break;				                      				            
	}

	return signature;
}