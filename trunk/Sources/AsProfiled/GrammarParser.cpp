#include "StdAfx.h"
#include "GrammarParser.h"
#include "astudillo/CGTFile.h"

CGrammarParser::CGrammarParser(std::string cgtFilePath) : filePath(cgtFilePath) 
{ 
	this->cgtFile = new CGTFile();
	this->myReporter = new SimpleErrorRep();
}

CGrammarParser::~CGrammarParser(void)
{
	delete cgtFile;
	delete rdc;
	delete dfa;
	delete lalr;
	delete myErrors;
	delete myReporter; 
}

bool CGrammarParser::Initialize() {
	struct stat stFileInfo;
	int intStat;

  // Attempt to get the file attributes
	intStat = stat(this->filePath.c_str(),&stFileInfo);
	if (intStat != 0) {
		return false;
	}
	
	this->cgtFile = new CGTFile();
	bool loadOk = this->cgtFile->load(const_cast<char*>(this->filePath.c_str()));
	return loadOk;
}

std::vector<Token*> CGrammarParser::Scan(std::wstring argument) {
	dfa = cgtFile->getScanner();
	
	bool result = dfa->scan(const_cast<WCHAR*>(argument.c_str()));
	ASSERT(result);
	myErrors = dfa->getErrors();

	//  // If there are errors report them
	if (myErrors->errors.size() > 0) {
		for (unsigned int i=0; i < myErrors->errors.size(); i++) {
			std::cout << myReporter->composeParseErrorMsg (*myErrors->errors[i]) << endl;
		}
	}

	std::vector<Token*> tokens;
	
	for (vector<Token*>::iterator i = dfa->getTokens().begin(); i != dfa->getTokens().end(); ++i) {
		tokens.push_back((*i)->newInstance());
	}

	this->lalr = cgtFile->getParser();
	rdc = lalr->parse(tokens, true, true);
	// lalr->printReductionTree(rdc,0);
	for ( vector<Token*>::iterator i = tokens.begin(); i != tokens.end(); i++) {
		std::wstring st((*i)->symbol);
		wcout << st << endl;
	}
	return tokens;
}

						
