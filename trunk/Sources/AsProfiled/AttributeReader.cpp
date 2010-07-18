#include "StdAfx.h"
#include "AttributeReader.h"
#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>

using namespace log4cxx;

LoggerPtr attributeLogger(Logger::getLogger("attributeReader"));

CAttributeReader::CAttributeReader(void)
{
	// Configure Log4cxx
	BasicConfigurator::configure();
	// Levels hierarchy: TRACE < DEBUG < INFO < WARN < ERROR < FATAL
	attributeLogger->setLevel(Level::toLevel(log4cxx::Level::DEBUG_INT));
}

CAttributeReader::~CAttributeReader(void)
{
	Reset();
}

void CAttributeReader::Initialize(mdMethodDef methodTokenArg, IMetaDataImport* pMetaDataImportArg) {

	this->methodToken = methodTokenArg;
	this->pMetaDataImport = pMetaDataImportArg;
	mdCustomAttribute tokensOfCustomAttributes[10];
	HCORENUM phEnum = NULL;
	this->attributesCount = 0;
	
	pMetaDataImport->EnumCustomAttributes(&phEnum, this->methodToken, 0, 
												tokensOfCustomAttributes, 10, &(this->attributesCount));
	
	while (this->attributesCount > 0){
		pMetaDataImport->EnumCustomAttributes(&phEnum, this->methodToken, 0, 
												tokensOfCustomAttributes, 10, &(this->attributesCount));
	} // while (this->attributesCount > 0);

	

	pMetaDataImport->CountEnum(phEnum, &this->attributesCount);
	this->metadataCustomAttributes = new mdCustomAttribute[this->attributesCount];
	pMetaDataImport->EnumCustomAttributes(&phEnum, this->methodToken, 0, 
												this->metadataCustomAttributes, attributesCount, NULL);

	pMetaDataImport->CloseEnum(phEnum);
}

void CAttributeReader::PrintAttributesInfo() {

	LOG4CXX_DEBUG(attributeLogger, "------Attributes Info------");
	LOG4CXX_DEBUG(attributeLogger, "No of attributes: " << this->attributesCount);
	LOG4CXX_DEBUG(attributeLogger,"------End Of Attribute Info------");
}

void CAttributeReader::ReadAttributes() {
	HRESULT hr;
	for (ULONG i = 0 ; i < this->attributesCount ; ++i) {

		// get info about custom attribute
		mdCustomAttribute *currentAttribute = &this->metadataCustomAttributes[i];
		
		const void* attributeBlob = NULL;
		ULONG attributeBlobSize = 0;
		mdMethodDef attributeConstructor = mdTokenNil;
		pMetaDataImport->GetCustomAttributeProps(*currentAttribute, NULL, &attributeConstructor, &attributeBlob, &attributeBlobSize); 

		LOG4CXX_DEBUG(attributeLogger, "Attribute ctor token value: " <<  attributeConstructor);
		LOG4CXX_DEBUG(attributeLogger, "Custom attribute token value: " << *currentAttribute);

		// should be always equal 1
		INT16 prolog =  *((UINT16*) attributeBlob);
		LOG4CXX_DEBUG(attributeLogger, "prolog (always=1) = " << prolog);

		PCCOR_SIGNATURE methodMetadataBlob = 0;
		ULONG metaDataBlobSize = 0;
		hr = pMetaDataImport->GetMethodProps(attributeConstructor, 
											NULL, 
											NULL, 
											NULL, 
											NULL, 
											NULL, 
											&methodMetadataBlob, 
											&metaDataBlobSize, 
											NULL, 
											NULL);
		CProfilerHelper::GetInstance().ParseCallingConvention(methodMetadataBlob);
		ULONG argsCount = CProfilerHelper::GetInstance().GetArgumentsCount(methodMetadataBlob);	

		if (argsCount == 3) {
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
		
		//mdMethodDef typeToken = mdTokenNil;
		//hr = pMetaDataImport->GetCustomAttributeProps(metadataCustomAttr[i], NULL, &typeToken, NULL, NULL);
		//LPWSTR typeName = new WCHAR[NAME_BUFFER_SIZE];
		//typeName[0] = '\0';
		//hr = pMetaDataImport->GetMethodProps(typeToken, NULL, typeName, NAME_BUFFER_SIZE, NULL, NULL, NULL, NULL, NULL, NULL);
		//hr = pMetaDataImport->GetTypeDefProps(metadataCustomAttr[i], typeName, NAME_BUFFER_SIZE, NULL, NULL, NULL);
		//mdTypeDef typeOfAttribute = mdTokenNil;
		//pMetaDataImport->GetMemberRefProps(typeToken, &typeOfAttribute, typeName, NAME_BUFFER_SIZE, NULL, NULL, NULL);

		//cout << endl;
		//PrintCharArray(typeName);
		//cout << endl;

		//LPWSTR typeOwner = new WCHAR[NAME_BUFFER_SIZE];
		//pMetaDataImport->GetTypeRefProps(typeOfAttribute, NULL, typeOwner, NAME_BUFFER_SIZE, NULL);
		//PrintCharArray(typeOwner);
	}
}

void CAttributeReader::Reset() {
	if (this->metadataCustomAttributes != NULL) {
		//delete this->metadataCustomAttributes;
	}
}
