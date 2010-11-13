#include "StdAfx.h"
#include "AttributeReader.h"
#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include "common.h"
#include "MethodInfo.h"
#include "TypeInfo.h"
#include "GrammarParser.h"
using namespace log4cxx;

LoggerPtr attributeLogger(Logger::getLogger("attributeReader"));

CAttributeReader::CAttributeReader(void)
{
	// Levels hierarchy: TRACE < DEBUG < INFO < WARN < ERROR < FATAL
	attributeLogger->setLevel(Level::toLevel(log4cxx::Level::INFO_INT));
	this->parser = new CGrammarParser("c:\\grammar.cgt");
	ASSERT(this->parser->Initialize());
}

CAttributeReader::~CAttributeReader(void)
{
	Reset();
	delete parser;
}

void CAttributeReader::Initialize(mdMethodDef methodTokenArg, IMetaDataImport* pMetaDataImportArg) {

	this->methodToken = methodTokenArg;
	this->pMetaDataImport = pMetaDataImportArg;
	mdCustomAttribute tokensOfCustomAttributes[1];
	HCORENUM phEnum = NULL;
	this->attributesCount = 0;
	
	// enum through attributes
	do {
		pMetaDataImport->EnumCustomAttributes(&phEnum, this->methodToken, 0, 
												tokensOfCustomAttributes, 1, &(this->attributesCount));
	} while (this->attributesCount > 0);

	pMetaDataImport->CountEnum(phEnum, &this->attributesCount);
	this->metadataCustomAttributes = new mdCustomAttribute[this->attributesCount];
	phEnum = NULL;
	pMetaDataImport->EnumCustomAttributes(&phEnum, this->methodToken, 0, 
												this->metadataCustomAttributes, attributesCount, NULL);
	pMetaDataImport->CloseEnum(phEnum);

	this->attributesInfo = new std::vector<CAttributeInfo*>();
	for (ULONG i = 0; i < this->attributesCount; ++i) 
	{
		CAttributeInfo* attributeInfo = new CAttributeInfo();
		attributeInfo->attributeToken = metadataCustomAttributes[i];
		pMetaDataImport->GetCustomAttributeProps(attributeInfo->attributeToken, NULL, 
												&attributeInfo->attributeConstructor, 
												&attributeInfo->attributeBlob, 
												&attributeInfo->attributeBlobSize);
		
		CMethodInfo attributeMethodInfo(this->pMetaDataImport, attributeInfo->attributeConstructor);	
		CTypeInfo typeInfo(pMetaDataImport, attributeMethodInfo.GetTypeToken());

		attributeInfo->argumentsCount = attributeMethodInfo.GetArgumentsCount();
		attributeInfo->typeName = typeInfo.GetName();

		std::vector<WCHAR*>* argumentsValues = CProfilerHelper::GetInstance().ParseAttributeMetaData(attributeInfo->attributeBlob, 
																									attributeInfo->attributeBlobSize, 
																									attributeInfo->argumentsCount);
		
		attributeInfo->arguments = new std::vector<CAttributeArgument*>();
		for (ULONG i = 0; i < argumentsValues->size(); ++i) 
		{
			CAttributeArgument* attributeArgument = new CAttributeArgument();
			attributeArgument->argumentValue = argumentsValues->at(i);
			std::wstring st(typeInfo.GetName());
			if (st.find(L"AsContractAttribute") != std::wstring::npos) {
				// ASSERT(false);
			}
			std::wstring argumentValue(argumentsValues->at(i));
			if (argumentValue.compare(L"") != 0) 
			{
				attributeArgument->tokens = parser->Scan(argumentValue);
			}

			attributeInfo->arguments->push_back(attributeArgument);
		}
		
		LOG4CXX_INFO(attributeLogger, attributeInfo->typeName);		
		LOG4CXX_INFO(attributeLogger, "Attribute arguments count: " << attributeInfo->argumentsCount);

		this->attributesInfo->push_back(attributeInfo);

	}
}

void CAttributeReader::PrintAttributesInfo() 
{
	LOG4CXX_INFO(attributeLogger, "------Attributes Info------");
	LOG4CXX_INFO(attributeLogger, "No of attributes: " << this->attributesCount);
	LOG4CXX_INFO(attributeLogger,"------End Of Attribute Info------");
}


ULONG CAttributeReader::GetAttributesCount() 
{
	return this->attributesCount;
}

CAttributeInfo* CAttributeReader::GetAttribute(std::wstring attributeName, ULONG numberOfAttributeParameter) {
	
	for (ULONG i = 0; i < this->attributesCount; i++) {
		std::wstring typeName(this->attributesInfo->at(i)->typeName);
		if (typeName.find(attributeName) != std::string::npos && this->attributesInfo->at(i)->argumentsCount == numberOfAttributeParameter) 
		{
			return attributesInfo->at(i);
		}
	}
	return NULL;
}

void CAttributeReader::Reset() {
	if (this->metadataCustomAttributes != NULL) {
		//delete this->metadataCustomAttributes;
	}
}

