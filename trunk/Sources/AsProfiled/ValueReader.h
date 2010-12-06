#pragma once

#include "cor.h"
#include "corprof.h"
#include <iostream>
class CValueReader
{
public:
	static CValueReader* GetInstance();
	
	~CValueReader(void);
	std::wstring TraceULong(UINT_PTR startAddress);

	std::wstring TraceLong(UINT_PTR startAddress);

	std::wstring TraceUInt(UINT_PTR startAddress);

	std::wstring TraceInt(UINT_PTR startAddress);

	std::wstring TraceUShort(UINT_PTR startAddress);

	std::wstring TraceShort(UINT_PTR startAddress);

	std::wstring TraceByte(UINT_PTR startAddress);

	std::wstring TraceSByte(UINT_PTR startAddress);

	std::wstring TraceChar(UINT_PTR startAddress);

	std::wstring TraceBoolean(UINT_PTR startAddress);

	std::wstring TraceFloat(UINT_PTR startAddress);

	std::wstring TraceDouble(UINT_PTR startAddress);

	std::wstring TraceString(UINT_PTR startAddress, ICorProfilerInfo2 *cpi);

	int ExceptionFilter(unsigned int code, struct _EXCEPTION_POINTERS *ep);
private:
	CValueReader(void);
	

};
