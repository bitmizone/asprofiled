#pragma once

#include "cor.h"
#include "corprof.h"
#include <iostream>
#include <vector>
class CProfilerHelper {
	public:
		static CProfilerHelper& GetInstance();
		void ParseCallingConvention(PCCOR_SIGNATURE& data);
		ULONG GetArgumentsCount(PCCOR_SIGNATURE& data);
		std::vector<WCHAR*>* ParseAttributeMetaData(const void* attributeBlob, ULONG blobSize, ULONG argumentsCount);
	private:
		CProfilerHelper();
};