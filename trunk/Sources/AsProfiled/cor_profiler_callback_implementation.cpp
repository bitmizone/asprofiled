#include "StdAfx.h"
#include "cor_profiler_callback_implementation.h"

CCorProfilerCallbackImplementation::CCorProfilerCallbackImplementation(void)
{
}

STDMETHODIMP CCorProfilerCallbackImplementation::Initialize(IUnknown *pICorProfilerInfoUnk)
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::Shutdown()
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::AppDomainCreationStarted(AppDomainID appDomainID)
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::AppDomainCreationFinished(AppDomainID appDomainID, HRESULT hrStatus)
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::AppDomainShutdownStarted(AppDomainID appDomainID)
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::AppDomainShutdownFinished(AppDomainID appDomainID, HRESULT hrStatus)
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::AssemblyLoadStarted(AssemblyID assemblyID)
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::AssemblyLoadFinished(AssemblyID assemblyID, HRESULT hrStatus)
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::AssemblyUnloadStarted(AssemblyID assemblyID)
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::AssemblyUnloadFinished(AssemblyID assemblyID, HRESULT hrStatus)
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::ModuleLoadStarted(ModuleID moduleID)
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::ModuleLoadFinished(ModuleID moduleID, HRESULT hrStatus)
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::ModuleUnloadStarted(ModuleID moduleID)
{
    return S_OK;
}
	  
STDMETHODIMP CCorProfilerCallbackImplementation::ModuleUnloadFinished(ModuleID moduleID, HRESULT hrStatus)
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::ModuleAttachedToAssembly(ModuleID moduleID, AssemblyID assemblyID)
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::ClassLoadStarted(ClassID classID)
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::ClassLoadFinished(ClassID classID, HRESULT hrStatus)
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::ClassUnloadStarted(ClassID classID)
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::ClassUnloadFinished(ClassID classID, HRESULT hrStatus)
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::FunctionUnloadStarted(FunctionID functionID)
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::JITCompilationStarted(FunctionID functionID, BOOL fIsSafeToBlock)
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::JITCompilationFinished(FunctionID functionID, HRESULT hrStatus, BOOL fIsSafeToBlock)
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::JITCachedFunctionSearchStarted(FunctionID functionID, BOOL *pbUseCachedFunction)
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::JITCachedFunctionSearchFinished(FunctionID functionID, COR_PRF_JIT_CACHE result)
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::JITFunctionPitched(FunctionID functionID)
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::JITInlining(FunctionID callerID, FunctionID calleeID, BOOL *pfShouldInline)
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::UnmanagedToManagedTransition(FunctionID functionID, COR_PRF_TRANSITION_REASON reason)
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::ManagedToUnmanagedTransition(FunctionID functionID, COR_PRF_TRANSITION_REASON reason)
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::ThreadCreated(ThreadID threadID)
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::ThreadDestroyed(ThreadID threadID)
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::ThreadAssignedToOSThread(ThreadID managedThreadID, DWORD osThreadID) 
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::RemotingClientInvocationStarted()
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::RemotingClientSendingMessage(GUID *pCookie, BOOL fIsAsync)
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::RemotingClientReceivingReply(GUID *pCookie, BOOL fIsAsync)
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::RemotingClientInvocationFinished()
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::RemotingServerReceivingMessage(GUID *pCookie, BOOL fIsAsync)
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::RemotingServerInvocationStarted()
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::RemotingServerInvocationReturned()
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::RemotingServerSendingReply(GUID *pCookie, BOOL fIsAsync)
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::RuntimeSuspendStarted(COR_PRF_SUSPEND_REASON suspendReason)
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::RuntimeSuspendFinished()
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::RuntimeSuspendAborted()
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::RuntimeResumeStarted()
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::RuntimeResumeFinished()
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::RuntimeThreadSuspended(ThreadID threadID)
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::RuntimeThreadResumed(ThreadID threadID)
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::MovedReferences(ULONG cmovedObjectIDRanges, ObjectID oldObjectIDRangeStart[], ObjectID newObjectIDRangeStart[], ULONG cObjectIDRangeLength[])
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::ObjectAllocated(ObjectID objectID, ClassID classID)
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::ObjectsAllocatedByClass(ULONG classCount, ClassID classIDs[], ULONG objects[])
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::ObjectReferences(ObjectID objectID, ClassID classID, ULONG objectRefs, ObjectID objectRefIDs[])
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::RootReferences(ULONG rootRefs, ObjectID rootRefIDs[])
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::ExceptionThrown(ObjectID thrownObjectID)
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::ExceptionUnwindFunctionEnter(FunctionID functionID)
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::ExceptionUnwindFunctionLeave()
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::ExceptionSearchFunctionEnter(FunctionID functionID)
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::ExceptionSearchFunctionLeave()
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::ExceptionSearchFilterEnter(FunctionID functionID)
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::ExceptionSearchFilterLeave()
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::ExceptionSearchCatcherFound(FunctionID functionID)
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::ExceptionCLRCatcherFound()
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::ExceptionCLRCatcherExecute()
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::ExceptionOSHandlerEnter(FunctionID functionID)
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::ExceptionOSHandlerLeave(FunctionID functionID)
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::ExceptionUnwindFinallyEnter(FunctionID functionID)
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::ExceptionUnwindFinallyLeave()
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::ExceptionCatcherEnter(FunctionID functionID,
    											 ObjectID objectID)
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::ExceptionCatcherLeave()
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::COMClassicVTableCreated(ClassID wrappedClassID, REFGUID implementedIID, void *pVTable, ULONG cSlots)
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::COMClassicVTableDestroyed(ClassID wrappedClassID, REFGUID implementedIID, void *pVTable)
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::ThreadNameChanged(ThreadID threadID, ULONG cchName, WCHAR name[])
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::GarbageCollectionStarted(int cGenerations, BOOL generationCollected[], COR_PRF_GC_REASON reason)
{
	return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::SurvivingReferences(ULONG cSurvivingObjectIDRanges, ObjectID objectIDRangeStart[], ULONG cObjectIDRangeLength[])
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::GarbageCollectionFinished()
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::FinalizeableObjectQueued(DWORD finalizerFlags, ObjectID objectID)
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::RootReferences2(ULONG cRootRefs, ObjectID rootRefIDs[], COR_PRF_GC_ROOT_KIND rootKinds[], COR_PRF_GC_ROOT_FLAGS rootFlags[], UINT_PTR rootIDs[])
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::HandleCreated(GCHandleID handleID, ObjectID initialObjectID)
{
    return S_OK;
}

STDMETHODIMP CCorProfilerCallbackImplementation::HandleDestroyed(GCHandleID handleID)
{
    return S_OK;
}

