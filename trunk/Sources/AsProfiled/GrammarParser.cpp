#include "StdAfx.h"
#include "GrammarParser.h"

CGrammarParser::CGrammarParser(std::string cgtFilePath) : filePath(cgtFilePath) 
{ 
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
