#pragma once

#include "cor.h"
#include "profiler_helper.h"
#include "GrammarParser.h"
#include "AttributeInfo.h"

class CAttributeReader
{
public:
	CAttributeReader(void);
	~CAttributeReader(void);
	void Initialize(mdMethodDef methodToken, IMetaDataImport* pMetaDataImport);
	void PrintAttributesInfo();
	ULONG GetAttributesCount();
	CAttributeInfo* GetAttribute(std::wstring attributeName, ULONG numberOfAttributeParameter);
private:
	void Reset(void);
	// Token of method which attributes are meant to be proccessed
	mdMethodDef methodToken;
	// array of tokens to metadata of attributes
	mdCustomAttribute* metadataCustomAttributes;
	// count of method's attributes
	ULONG attributesCount;
	// Handle to MetaDataImport interface
	IMetaDataImport* pMetaDataImport;
	// Grammar parser
	CGrammarParser* parser;
	
	std::vector<CAttributeInfo*>* attributesInfo;
};
