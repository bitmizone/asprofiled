#include "StdAfx.h"
#include "ValueReader.h"
#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
using namespace log4cxx;

LoggerPtr valueLogger(Logger::getLogger("valueLogger"));

CValueReader* CValueReader::GetInstance() {
	static CValueReader* instance; 
	if (instance == NULL) {
		instance = new CValueReader();
	}
	return instance;
}

CValueReader::CValueReader(void)
{
	if (valueLogger->isInfoEnabled() == false) {
		BasicConfigurator::configure();
		// Levels hierarchy: TRACE < DEBUG < INFO < WARN < ERROR < FATAL
		valueLogger->setLevel(Level::toLevel(log4cxx::Level::INFO_INT));
	}
}

CValueReader::~CValueReader(void)
{

}

// ----------------------------------------------------------------------------
std::wstring CValueReader::TraceULong(UINT_PTR startAddress)
{
	LOG4CXX_INFO(valueLogger, *(unsigned long long *)startAddress);
	return S_OK;
}

// ----------------------------------------------------------------------------
std::wstring CValueReader::TraceLong(UINT_PTR startAddress)
{
	LOG4CXX_INFO(valueLogger, *(long long *)startAddress);
	return S_OK;
}

// ----------------------------------------------------------------------------
std::wstring CValueReader::TraceUInt(UINT_PTR startAddress)
{
	LOG4CXX_INFO(valueLogger, *(unsigned int *)startAddress);
	return S_OK;
}

// ----------------------------------------------------------------------------
std::wstring CValueReader::TraceInt(UINT_PTR startAddress)
{
	std::wostringstream stream;
    stream << *(int *)startAddress;
	return stream.str();
}

// ----------------------------------------------------------------------------
std::wstring CValueReader::TraceUShort(UINT_PTR startAddress)
{
	LOG4CXX_INFO(valueLogger, *(unsigned short *)startAddress);
	return S_OK;
}

// ----------------------------------------------------------------------------
std::wstring CValueReader::TraceShort(UINT_PTR startAddress)
{
	LOG4CXX_INFO(valueLogger, *(short *)startAddress);
	return S_OK;
}

// ----------------------------------------------------------------------------
std::wstring CValueReader::TraceByte(UINT_PTR startAddress)
{
	LOG4CXX_INFO(valueLogger, *(unsigned char *)startAddress);
	return S_OK;
}

// ----------------------------------------------------------------------------
std::wstring CValueReader::TraceSByte(UINT_PTR startAddress)
{
	LOG4CXX_INFO(valueLogger, *(char *)startAddress);
	return S_OK;
}

// ----------------------------------------------------------------------------
std::wstring CValueReader::TraceChar(UINT_PTR startAddress)
{
	LOG4CXX_INFO(valueLogger, *(char*)startAddress);
	return S_OK;
}

// ----------------------------------------------------------------------------
std::wstring CValueReader::TraceBoolean(UINT_PTR startAddress)
{
	if((*(bool*)startAddress)) 
	{
		return L"true";
	}else{
		return L"false";
	}
}

// ----------------------------------------------------------------------------
std::wstring CValueReader::TraceFloat(UINT_PTR startAddress)
{
  LOG4CXX_INFO(valueLogger, *(float*)startAddress);
  return S_OK;
}

// ----------------------------------------------------------------------------
std::wstring CValueReader::TraceDouble(UINT_PTR startAddress)
{
  LOG4CXX_INFO(valueLogger, *(double*)startAddress);
  return S_OK;
}

// ----------------------------------------------------------------------------
std::wstring CValueReader::TraceString(
  UINT_PTR startAddress,
  ICorProfilerInfo2 *cpi)
{
	ObjectID* id = reinterpret_cast<ObjectID*>( startAddress);
	ObjectID oid = *id;
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
	std::wstring buff(buffer);
	std::wstring result = L"\"" + buff + L"\"";
	return result;
  }else{
	  LOG4CXX_INFO(valueLogger, "?");
  }
	return L"";
}

int CValueReader::ExceptionFilter(unsigned int code, struct _EXCEPTION_POINTERS *ep) {
   if (code == EXCEPTION_ACCESS_VIOLATION) {
      return EXCEPTION_EXECUTE_HANDLER;
   } else {
      return EXCEPTION_CONTINUE_SEARCH;
   };
}

// ----------------------------------------------------------------------------
