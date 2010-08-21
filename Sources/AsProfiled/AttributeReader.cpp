#include "StdAfx.h"
#include "AttributeReader.h"
#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include "common.h"
#include "MethodInfo.h"
#include "TypeInfo.h"

using namespace log4cxx;

LoggerPtr attributeLogger(Logger::getLogger("attributeReader"));

CAttributeReader::CAttributeReader(void)
{
	// Levels hierarchy: TRACE < DEBUG < INFO < WARN < ERROR < FATAL
	attributeLogger->setLevel(Level::toLevel(log4cxx::Level::INFO_INT));
}

CAttributeReader::~CAttributeReader(void)
{
	Reset();
}

void CAttributeReader::Initialize(mdMethodDef methodTokenArg, IMetaDataImport* pMetaDataImportArg) {

	this->methodToken = methodTokenArg;
	this->pMetaDataImport = pMetaDataImportArg;
	mdCustomAttribute tokensOfCustomAttributes[1];
	HCORENUM phEnum = NULL;
	this->attributesCount = 0;
		
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
}

void CAttributeReader::PrintAttributesInfo() {

	LOG4CXX_INFO(attributeLogger, "------Attributes Info------");
	LOG4CXX_INFO(attributeLogger, "No of attributes: " << this->attributesCount);
	LOG4CXX_INFO(attributeLogger,"------End Of Attribute Info------");
}

void CAttributeReader::ReadAttributes() {
	HRESULT hr;
	
	for (ULONG i = 0 ; i < this->attributesCount ; ++i) {

		// get info about custom attribute
		mdCustomAttribute *currentAttribute = &this->metadataCustomAttributes[i];
		// LOG4CXX_INFO(attributeLogger, "address" << currentAttribute);
		const void* attributeBlob = NULL;
		ULONG attributeBlobSize = 0;
		mdMethodDef attributeConstructor = mdTokenNil;
		pMetaDataImport->GetCustomAttributeProps(*currentAttribute, NULL, &attributeConstructor, &attributeBlob, &attributeBlobSize); 
		ASSERT(attributeConstructor != mdTokenNil);
		//LOG4CXX_INFO(attributeLogger, "Blob size " << attributeBlobSize);

		// should be always equal 1
		INT16 prolog =  *((UINT16*) attributeBlob);
		LOG4CXX_INFO(attributeLogger, "prolog (always=1) = " << prolog);
		
		PCCOR_SIGNATURE methodMetadataBlob = 0;
		ULONG metaDataBlobSize = 0;
		mdTypeDef typeDefToken = mdTokenNil;
		
		hr = pMetaDataImport->GetMethodProps(attributeConstructor, 
											&typeDefToken, 
											NULL, 
											NULL, 
											NULL, 
											NULL, 
											&methodMetadataBlob, 
											&metaDataBlobSize, 
											NULL, 
											NULL);
		
		ASSERT(hr == S_OK);
		CMethodInfo attributeMethodInfo(this->pMetaDataImport, attributeConstructor);
		CTypeInfo* typeInfo = new CTypeInfo(pMetaDataImport, typeDefToken);

		std::wstring attributeName(typeInfo->GetName());
		LOG4CXX_INFO(attributeLogger, attributeName.c_str());
		LOG4CXX_INFO(attributeLogger, "Attribute arguments count: " << attributeMethodInfo.GetArgumentsCount());
		ULONG argsCount = attributeMethodInfo.GetArgumentsCount();	
		if (attributeName.find(L"AsContractAttribute") != std::string::npos) {
			WCHAR* argument = CProfilerHelper::GetInstance().ParseAttributeMetaData(attributeBlob, attributeBlobSize);
			std::cout << "FOUND" << std::endl;
			PrintCharArray(argument);
			std::cout << std::endl;
			//LOG4CXX_INFO(attributeLogger, argument);
						//////////////////////////////////////////
						///////// GRAMMAR PARSER SECTION//////////
						
						//delete grammarParser;
						//CProfilerHelper::GetInstance().ParseAttributeMetaData(attributeBlob, attributeBlobSize);
						//WCHAR* argument = L"bieda > 3";

						//CGTFile    cgtFile;
						//Symbol     *rdc;
						//DFA        *dfa;
						//LALR       *lalr;
						//ErrorTable       *myErrors;
						//  SimpleErrorRep   myReporter; 
						//bool ok = cgtFile.load("c:\\grammar.cgt");
						//std::cout << ok << std::endl;
						//dfa = cgtFile.getScanner();
						//
						//std::cout << "OK2" << std::endl;
						//ok = dfa->scan(argument);
						//myErrors = dfa->getErrors();
		  
						//  // If there are errors report them
						//  if (myErrors->errors.size() > 0) {
						//	for (unsigned int i=0; i < myErrors->errors.size(); i++) {
						//		std::cout << myReporter.composeParseErrorMsg (*myErrors->errors[i]) << endl;
						//	}
						//  }

						//std::cout << ok << std::endl;
						////delete [] argument;
						//std::cout << "OK4" << std::endl;
						//vector<Token*> tokens = dfa->getTokens();
						//std::cout << "OK5" << std::endl;
						//lalr = cgtFile.getParser();
						//std::cout << "OK6" << std::endl;
						//rdc = lalr->parse(tokens, true, true);
						//std::cout << "OK7" << std::endl;
						//lalr->printReductionTree(rdc,0);
						//std::cout << "OK8" << std::endl;
						//for ( vector<Token*>::iterator i = tokens.begin(); i != tokens.end(); i++) {
						//	cout << (*i)->kind << endl;
						//}

		}
				
		delete typeInfo;
	}
}

void CAttributeReader::Reset() {
	if (this->metadataCustomAttributes != NULL) {
		//delete this->metadataCustomAttributes;
	}
}
