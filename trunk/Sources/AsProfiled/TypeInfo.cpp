#include "StdAfx.h"
#include "TypeInfo.h"
#include <string>
CTypeInfo::CTypeInfo(IMetaDataImport* pMetaDataImport, mdTypeDef typeToken) : 
	metaDataImport(pMetaDataImport), typeDefToken(typeToken)
{
	this->name = NULL;
}

CTypeInfo::~CTypeInfo(void)
{
}

WCHAR* CTypeInfo::GetName(void) {
	ULONG c = 0;
	if (this->name == NULL) {
		name = new WCHAR[NAME_BUFFER_SIZE];
		this->metaDataImport->GetTypeDefProps(typeDefToken, this->name, NAME_BUFFER_SIZE, &c, NULL, NULL);
	}
	return this->name;
}
