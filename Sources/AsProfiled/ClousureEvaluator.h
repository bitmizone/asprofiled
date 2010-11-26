#pragma once

#include "cor.h"
#include "MethodInfo.h"
#include "AttributeInfo.h"
#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <map>
#include "GrammarParser.h"
using namespace log4cxx;

class CClousureEvaluator
{
public:
	CClousureEvaluator(CMethodInfo* methodInfo, CAttributeInfo* attributeInfo, ICorProfilerInfo2 *corProfilerInfo);
	~CClousureEvaluator(void);
	CAttributeInfo* GetAttributeInfo();
	CMethodInfo* GetMethodInfo();
	bool EvalPreCondition(COR_PRF_FUNCTION_ARGUMENT_INFO* argumentInfo);
	bool EvalPostCondition();
	
private:
	void PrintReductionTree(Symbol* tree);
	void TraceValues();
	void Initialize();
	void ParsePreCondition();
	void ParsePostCondition();
	bool initialized;
	CAttributeInfo* attributeInfo;
	CMethodInfo* methodInfo;
	ICorProfilerInfo2* cpi;
	std::map<std::wstring, CParam*>* tokenToParamMap;
	static CGrammarParser* parser;
};
