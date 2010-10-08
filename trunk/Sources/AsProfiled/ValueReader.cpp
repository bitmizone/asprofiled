#include "StdAfx.h"
#include "ValueReader.h"
#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
using namespace log4cxx;

LoggerPtr valueLogger(Logger::getLogger("attributeReader"));

CValueReader::CValueReader(void)
{
	valueLogger->setLevel(Level::toLevel(log4cxx::Level::INFO_INT));
}

CValueReader::~CValueReader(void)
{

}

// ----------------------------------------------------------------------------
HRESULT CValueReader::TraceULong(UINT_PTR startAddress)
{
	LOG4CXX_INFO(valueLogger, *(unsigned long long *)startAddress);
	return S_OK;
}

// ----------------------------------------------------------------------------
HRESULT CValueReader::TraceLong(UINT_PTR startAddress)
{
	LOG4CXX_INFO(valueLogger, *(long long *)startAddress);
	return S_OK;
}

// ----------------------------------------------------------------------------
HRESULT CValueReader::TraceUInt(UINT_PTR startAddress)
{
	LOG4CXX_INFO(valueLogger, *(unsigned int *)startAddress);
	return S_OK;
}

// ----------------------------------------------------------------------------
HRESULT CValueReader::TraceInt(UINT_PTR startAddress)
{
	LOG4CXX_INFO(valueLogger, *(int *)startAddress);
	return S_OK;
}

// ----------------------------------------------------------------------------
HRESULT CValueReader::TraceUShort(UINT_PTR startAddress)
{
	LOG4CXX_INFO(valueLogger, *(unsigned short *)startAddress);
	return S_OK;
}

// ----------------------------------------------------------------------------
HRESULT CValueReader::TraceShort(UINT_PTR startAddress)
{
	LOG4CXX_INFO(valueLogger, *(short *)startAddress);
	return S_OK;
}

// ----------------------------------------------------------------------------
HRESULT CValueReader::TraceByte(UINT_PTR startAddress)
{
	LOG4CXX_INFO(valueLogger, *(unsigned char *)startAddress);
	return S_OK;
}

// ----------------------------------------------------------------------------
HRESULT CValueReader::TraceSByte(UINT_PTR startAddress)
{
	LOG4CXX_INFO(valueLogger, *(char *)startAddress);
	return S_OK;
}

// ----------------------------------------------------------------------------
HRESULT CValueReader::TraceChar(UINT_PTR startAddress)
{
	LOG4CXX_INFO(valueLogger, *(char*)startAddress);
	return S_OK;
}

// ----------------------------------------------------------------------------
HRESULT CValueReader::TraceBoolean(UINT_PTR startAddress)
{
	if((*(bool*)startAddress)) 
	{
		LOG4CXX_INFO(valueLogger, "true");
	}else{
		LOG4CXX_INFO(valueLogger, "false");
	}
	return S_OK;
}

// ----------------------------------------------------------------------------
HRESULT CValueReader::TraceFloat(UINT_PTR startAddress)
{
  LOG4CXX_INFO(valueLogger, *(float*)startAddress);
  return S_OK;
}

// ----------------------------------------------------------------------------
HRESULT CValueReader::TraceDouble(UINT_PTR startAddress)
{
  LOG4CXX_INFO(valueLogger, *(double*)startAddress);
  return S_OK;
}

// ----------------------------------------------------------------------------
HRESULT CValueReader::TraceString(
  ObjectID oid,
  ICorProfilerInfo2 *cpi)
{
	if(oid == mdTokenNil || oid == 0xffffffff) 
	{
		LOG4CXX_INFO(valueLogger, "null");
		return S_OK;
	}

	//if(oid <= 0xFF) { // how to check whether ObjectID is valid?
	//	LOG4CXX_INFO(valueLogger, "?");
	//	return S_OK;
	//}

  // get string
  ULONG bufferOffset=0;
  ULONG stringLengthOffset=0;
  ULONG bufferLengthOffset=0;
  HRESULT hr = cpi->GetStringLayout(&bufferLengthOffset, &stringLengthOffset, &bufferOffset);
  if(SUCCEEDED(hr))
  {
    LPWSTR buffer = (LPWSTR) (oid + bufferOffset);
	DWORD bufferLength = *((DWORD *)(oid + bufferLengthOffset));
	DWORD stringLength = *((DWORD *)(oid + stringLengthOffset));
	LOG4CXX_INFO(valueLogger, buffer);
  }else{
	  LOG4CXX_INFO(valueLogger, "?");
  }
  return hr;
}

int CValueReader::ExceptionFilter(unsigned int code, struct _EXCEPTION_POINTERS *ep) {
   if (code == EXCEPTION_ACCESS_VIOLATION) {
      return EXCEPTION_EXECUTE_HANDLER;
   } else {
      return EXCEPTION_CONTINUE_SEARCH;
   };
}

// ----------------------------------------------------------------------------
