#include "StdAfx.h"
#include "GrammarParser.h"

CGrammarParser::CGrammarParser(std::string& cgtFilePath)
{
	struct stat stFileInfo;
	int intStat;

  // Attempt to get the file attributes
	intStat = stat(cgtFilePath.c_str(),&stFileInfo);
	if (intStat == 0) {
	
	}else{
		std::cout << "HAHAHAAGHA" << intStat << std::endl;
		FAILURE("Config file not found", 1);
	}
	
	FAILURE("Config file not found", 1);
	this->cgtFile = new CGTFile();
	bool loadOk = this->cgtFile->load(const_cast<char*>(cgtFilePath.c_str()));
	if (loadOk == false) {
		FAILURE("Config file not found", 1);
	}else{
		FAILURE("Config file not foundffffffffffff", 1);
	}
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
