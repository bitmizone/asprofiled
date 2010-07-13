#pragma once
#include "cor.h"
#include "common.h"

class CParamParser
{
public:
	CParamParser(IMetaDataImport& identifier);
	~CParamParser();
	PCCOR_SIGNATURE ParseSignature(PCCOR_SIGNATURE signature, LPWSTR signatureText);
private:
	IMetaDataImport& pMetaData;
};
