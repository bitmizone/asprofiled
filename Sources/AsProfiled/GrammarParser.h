#pragma once

#ifndef GRAMMAR_PARSER_H
#define GRAMMER_PARSER_H

#include <string>
#include "astudillo/CGTFile.h"
#include "SimpleErrorRep.h"
#include "common.h"
#include <sys/stat.h> 
class CGrammarParser
{
public:
	CGrammarParser(std::string cgtFilePath);
	~CGrammarParser(void);
	bool Initialize(void);
	std::vector<Token*> Scan(std::wstring argument);
	LALR* GetReductionTree(std::vector<Token*> tokens);
private:
	std::string filePath;
	CGTFile *cgtFile;
	Symbol *rdc;
	DFA	*dfa;
	LALR *lalr;
	ErrorTable *myErrors;
	SimpleErrorRep *myReporter; 
};

#endif GRAMMAR_PARSER_H