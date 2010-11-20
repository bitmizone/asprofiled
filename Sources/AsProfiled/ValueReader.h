#pragma once

#include "cor.h"
#include "corprof.h"
#include <iostream>
class CValueReader
{
public:
	static CValueReader* GetInstance();
	
	~CValueReader(void);
	HRESULT TraceULong(UINT_PTR startAddress);

	HRESULT TraceLong(UINT_PTR startAddress);

	HRESULT TraceUInt(UINT_PTR startAddress);

	std::wstring TraceInt(UINT_PTR startAddress);

	HRESULT TraceUShort(UINT_PTR startAddress);

	HRESULT TraceShort(UINT_PTR startAddress);

	HRESULT TraceByte(UINT_PTR startAddress);

	HRESULT TraceSByte(UINT_PTR startAddress);

	HRESULT TraceChar(UINT_PTR startAddress);

	HRESULT TraceBoolean(UINT_PTR startAddress);

	HRESULT TraceFloat(UINT_PTR startAddress);

	HRESULT TraceDouble(UINT_PTR startAddress);

	HRESULT TraceString(UINT_PTR startAddress, ICorProfilerInfo2 *cpi);

	int ExceptionFilter(unsigned int code, struct _EXCEPTION_POINTERS *ep);
private:
	CValueReader(void);
	

};
