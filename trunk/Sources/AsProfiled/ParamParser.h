#pragma once
#include "cor.h"
#include "common.h"
#include "Param.h"

class CParamParser
{
public:
	CParamParser(IMetaDataImport& pMetaDataIdentifier);
	~CParamParser();
	PCCOR_SIGNATURE ParseSignature(PCCOR_SIGNATURE signature, CParam& param);
	CorElementType GetType(PCCOR_SIGNATURE& sigBlob, bool &isByRef, mdTypeDef &typeDef, bool &isArray);
private:
	IMetaDataImport& pMetaData;
};
