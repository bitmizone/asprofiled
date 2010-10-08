#pragma once

#include "cor.h"
#include "corprof.h"

class CValueReader
{
public:
	CValueReader(void);
	
	~CValueReader(void);

	

	HRESULT TraceULong(UINT_PTR startAddress);

	HRESULT TraceLong(UINT_PTR startAddress);

	HRESULT TraceUInt(UINT_PTR startAddress);

	HRESULT TraceInt(UINT_PTR startAddress);

	HRESULT TraceUShort(UINT_PTR startAddress);

	HRESULT TraceShort(UINT_PTR startAddress);

	HRESULT TraceByte(UINT_PTR startAddress);

	HRESULT TraceSByte(UINT_PTR startAddress);

	HRESULT TraceChar(UINT_PTR startAddress);

	HRESULT TraceBoolean(UINT_PTR startAddress);

	HRESULT TraceFloat(UINT_PTR startAddress);

	HRESULT TraceDouble(UINT_PTR startAddress);

	HRESULT TraceString(ObjectID oid, ICorProfilerInfo2 *cpi);

	int ExceptionFilter(unsigned int code, struct _EXCEPTION_POINTERS *ep);

	

};
