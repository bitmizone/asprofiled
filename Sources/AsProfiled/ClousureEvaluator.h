#pragma once

#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <map>
#include <vector>
#include <memory>
#include "cor.h"
#include "MethodInfo.h"
#include "AttributeInfo.h"
#include "GrammarParser.h"
#include "ValueReader.h"
#include "Node.h"
using namespace log4cxx;

class CClousureEvaluator
{
public:
	CClousureEvaluator(CMethodInfo* methodInfo, CAttributeInfo* attributeInfo, ICorProfilerInfo2 *corProfilerInfo, COR_PRF_FUNCTION_ARGUMENT_INFO* argumentInfo);
	~CClousureEvaluator(void);
	CAttributeInfo* GetAttributeInfo();
	CMethodInfo* GetMethodInfo();
	bool EvalPreCondition();
	bool EvalPostCondition( COR_PRF_FUNCTION_ARGUMENT_RANGE *retvalRange);
	
private:
	void CopyArgumentInfoStructure(COR_PRF_FUNCTION_ARGUMENT_INFO* argInfo);
	void PrintNodeTree(CNode* node, int depth);
	void RewriteReductionTree(CNode* evaluationTree, Symbol* tree);
	void PrintReductionTree(Symbol* tree);
	void Initialize();
	void ParsePreCondition();
	void ParsePostCondition(CParam* returnValue);
	bool EvaluateNode(CNode* node);
	bool Eval(std::vector<CNode*>* arguments, std::wstring operatorValue);
	void BuildParamChain(CParam* param, std::vector<std::wstring> paramPath);
	COR_PRF_FUNCTION_ARGUMENT_INFO* argumentInfo;
	COR_PRF_FUNCTION_ARGUMENT_INFO* initialArgumentInfo;
	CAttributeInfo* attributeInfo;
	CMethodInfo* methodInfo;
	ICorProfilerInfo2* cpi;
	std::map<std::wstring, CParam*>* tokenToParamMap;
	static CGrammarParser* parser;
	CNode* reductionTree;

	/// boolean operations
	bool And(CNode* leftNode, CNode* rightNode);
	/// cmp operations
	bool Equal(CNode* leftNode, CNode* rightNode);
};
