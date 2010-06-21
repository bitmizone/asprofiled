#pragma once

#include "cor.h"
#include "corprof.h"
#include <iostream>

class CProfilerHelper {
	public:
		static CProfilerHelper& GetInstance();
		void ParseCallingConvention(PCCOR_SIGNATURE& data);
		ULONG GetArgumentsCount(PCCOR_SIGNATURE& data);
		WCHAR* ParseAttributeMetaData(const void* attributeBlob, ULONG blobSize);
	private:
		CProfilerHelper();
};