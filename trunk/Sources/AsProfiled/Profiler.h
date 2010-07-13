// Profiler.h : Declaration of the CProfiler

#pragma once
#include "resource.h"       // main symbols

#include "AsProfiled_i.h"
#include "cor_profiler_callback_implementation.h"
#include "corprof.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

// CProfiler



class ATL_NO_VTABLE CProfiler :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CProfiler, &CLSID_Profiler>,
	//public IDispatchImpl<IProfiler, &IID_IProfiler, &LIBID_AsProfiledLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public CCorProfilerCallbackImplementation
{
public:
	CProfiler();

DECLARE_REGISTRY_RESOURCEID(IDR_PROFILER)


BEGIN_COM_MAP(CProfiler)
//	COM_INTERFACE_ENTRY(IProfiler)
	COM_INTERFACE_ENTRY(ICorProfilerCallback2)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct() {
		return S_OK;
	}

	void FinalRelease() {
	
	}

	// STARTUP/SHUTDOWN EVENTS
    STDMETHOD(Initialize)(IUnknown *pICorProfilerInfoUnk);
    STDMETHOD(Shutdown)();
	

	void FunctionEnter(FunctionID functionID, UINT_PTR clientData, COR_PRF_FRAME_INFO func, COR_PRF_FUNCTION_ARGUMENT_INFO *argumentInfo);
	void FunctionLeave(FunctionID functionID, UINT_PTR clientData, COR_PRF_FRAME_INFO func, COR_PRF_FUNCTION_ARGUMENT_RANGE *retvalRange);

	void MapFunction(FunctionID);
	static UINT_PTR _stdcall FunctionMapper(FunctionID functionId, BOOL *pbHookFunction);

private :
	CComQIPtr<ICorProfilerInfo2> _ICorProfilerInfo2;
	HRESULT SetEventMask();
	HRESULT GetFullMethodName(FunctionID functionId, LPWSTR wszMethod);
	HRESULT GetFunctionData(FunctionID functionId);
	void ParseParam(PCCOR_SIGNATURE signature);
	/*PCCOR_SIGNATURE ParseSignature( IMetaDataImport *metaDataImport, PCCOR_SIGNATURE signature, LPWSTR signatureText);*/
};

OBJECT_ENTRY_AUTO(__uuidof(Profiler), CProfiler)
