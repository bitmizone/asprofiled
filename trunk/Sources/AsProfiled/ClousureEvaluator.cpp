#include "StdAfx.h"
#include "ClousureEvaluator.h"
#include "Operator.h"
using namespace log4cxx;

LoggerPtr clousureEvaluatorLogger(Logger::getLogger("ce"));

CGrammarParser* CClousureEvaluator::parser = new CGrammarParser("c:\\grammar.cgt");

CClousureEvaluator::CClousureEvaluator(CMethodInfo* methodInfoArg, CAttributeInfo* attributeInfoArg, 
									   ICorProfilerInfo2* corProfilerInfo, COR_PRF_FUNCTION_ARGUMENT_INFO* argumentInfo)
{
	this->attributeInfo = attributeInfoArg;
	this->methodInfo = methodInfoArg;
	this->cpi = corProfilerInfo;
	this->tokenToParamMap = new std::map<wstring, CParam*>();
	// Configure Log4cxx
	if (clousureEvaluatorLogger->isInfoEnabled() == false) {
		BasicConfigurator::configure();
		// Levels hierarchy: TRACE < DEBUG < INFO < WARN < ERROR < FATAL
		clousureEvaluatorLogger->setLevel(Level::toLevel(log4cxx::Level::INFO_INT));
	}
	this->Initialize();
	this->reductionTree = NULL;
	this->CopyArgumentInfoStructure(argumentInfo);
}

void CClousureEvaluator::CopyArgumentInfoStructure(COR_PRF_FUNCTION_ARGUMENT_INFO* argInfo) {
	this->argumentInfo = new COR_PRF_FUNCTION_ARGUMENT_INFO(*argInfo);
}

CClousureEvaluator::~CClousureEvaluator(void)
{

}

void CClousureEvaluator::RewriteReductionTree(CNode* parentNode, Symbol* tree) {
	if (tree == NULL) {
		return;
	}

	CNode* childNode = NULL;
	parentNode->children = new std::vector<CNode*>();
	parentNode->tokenType = Operator::GetTokenType(tree->symbol.c_str());
	parentNode->value = tree->symbol;
	if (tree->type == NON_TERMINAL) {
		NonTerminal* nonTerminal = (NonTerminal*) tree;
		for (UINT i = 0 ; i < nonTerminal->children.size(); ++i) {
			childNode = new CNode();
			parentNode->children->push_back(childNode);
			RewriteReductionTree(childNode, nonTerminal->children[i]);
		}
	}else {
		Terminal* terminal = (Terminal*) tree;
		map<wstring, CParam*>::iterator iter = tokenToParamMap->find(terminal->image);
		if (iter == tokenToParamMap->end()) {
			childNode = new CNode();
			childNode->value = terminal->image;
		}else {
			childNode = iter->second;
		}
		childNode->value = terminal->image;
		parentNode->children->push_back(childNode);
		LOG4CXX_INFO(clousureEvaluatorLogger, terminal->image.c_str());

		// Identifier, DecimalValue
		// LOG4CXX_INFO(clousureEvaluatorLogger, terminal->symbol.c_str());
		// 2, 'literal'
		// LOG4CXX_INFO(clousureEvaluatorLogger, terminal->image.c_str());
	}
}

void CClousureEvaluator::PrintNodeTree(CNode* node, int depth) {
	for (int i = 0; i < depth; ++i) {
		std::wcout << L" ";
	}

	if (node == NULL ) {
		return;
	}

	std::wcout << node->value << ":" << (node->children == NULL ? 0 : node->children->size()) << ":" << (int)node->tokenType << std::endl;
	if (node->children == NULL) {
		return ;
	}
	for (UINT i = 0 ; i < node->children->size(); ++i) {
		PrintNodeTree(node->children->at(i), depth+1);
	}
}


void CClousureEvaluator::PrintReductionTree(Symbol* reductionTree) {
	if (reductionTree == NULL) {
		return;
	}
	LOG4CXX_INFO(clousureEvaluatorLogger, reductionTree->symbol);
	if (reductionTree->type == NON_TERMINAL) {
		NonTerminal* nonTerminal = (NonTerminal*) reductionTree;
		for (UINT i = 0 ; i < nonTerminal->children.size(); ++i) {
			PrintReductionTree(nonTerminal->children[i]);
		}
	}else {
		Terminal* terminal = (Terminal*) reductionTree;
		// LOG4CXX_INFO(clousureEvaluatorLogger, terminal->image.c_str());
		
	}
}

bool CClousureEvaluator::Equal(CNode* left, CNode* right) {
	while (left->children != NULL && left->children->size() == 1) {
		left = left->children->at(0);
	}
	while (right->children != NULL && right->children->size() == 1) {
		right = right->children->at(0);
	}

	if (left->children != NULL) {
		
	}
	if (right->children != NULL) {
	
	}

	std::wstring leftVal;
	std::wstring rightVal; 

	leftVal = left->GetValue();
	rightVal = right->GetValue();

	return leftVal.compare(rightVal) == 0;
}

bool CClousureEvaluator::And(CNode* leftNode, CNode* rightNode) {
	bool leftValue = this->EvaluateNode(leftNode);
	bool rightValue = this->EvaluateNode(rightNode);
	return leftValue && rightValue;
}

bool CClousureEvaluator::Eval(std::vector<CNode*>* arguments, std::wstring operatorValue) {
	Operator::OperatorType operatorType = Operator::GetOperatorType(operatorValue.c_str());
	bool result = false;
	switch (operatorType) {
		case Operator::AddOperatorType:
		{
			Operator::AddOperator additiveOp = Operator::GetAddOperator(operatorValue.c_str());
			switch (additiveOp) {
				case Operator::Add:
					break;
				case Operator::Minus:
					break;
			}
			break;
		}
		case Operator::MultOperatorType:
		{
			Operator::MultOperator multiplicativeOp = Operator::GetMultOperator(operatorValue.c_str());
			switch (multiplicativeOp) {
				case Operator::Multiply:
					break;
				case Operator::Divide:
					break;
				default:
					break;
			}
			break;
		}
		case Operator::CmpOperatorType:
		{
			Operator::CmpOperator cmpOperator = Operator::GetCmpOperator(operatorValue.c_str());
			switch (cmpOperator) {
				case Operator::Less:
					break;
				case Operator::LessOrEqual:
					break;
				case Operator::Equal:
					result = this->Equal(arguments->at(0), arguments->at(2));
					return result;
				case Operator::NotEqual:
					break;
				case Operator::Greater:
					break;
				case Operator::GreaterOrEqual:
					break;
			}
		}
		case Operator::BooleanOperatorType:
		{
			Operator::BooleanOperator booleanOperator = Operator::GetBooleanOperator(operatorValue.c_str());
			switch (booleanOperator) {
				case Operator::BoolAnd:
					result = this->And(arguments->at(0), arguments->at(2));
					return result;
					break;
				case Operator::BoolOr:
					break;
			}
		}
		case Operator::BitOperatorType: 
		{
			Operator::BitOperator bitOperator = Operator::GetBitOperator(operatorValue.c_str());
			switch (bitOperator) {
				case Operator::BitAnd:
					break;
				case Operator::BitOr:
					break;
			}
		}
		default:
			break;
	}
	return true;
}

bool CClousureEvaluator::EvaluateNode(CNode* node) {
	if (node == NULL) {
		ASSERT(false);
		return false;
	}
	if (node->evaluated == true) { // nothing to be done
		return node->result;
	}
	
	if (node->tokenType == Operator::StartSymbol) { // just go through the tree
		this->EvaluateNode(node->children->at(0));
		node->result = node->children->at(0)->result;
		return node->result;
	}

	if (node->tokenType == Operator::Expression) {
		// find operator
		UINT i;
		Operator::OperatorType operatorType = Operator::NotOp;
		std::wstring operatorValue;
		for (i = 0; i < node->children->size(); ++i) {
			CNode* child = node->children->at(i);
			if (child->tokenType == Operator::OperatorNT) {
				operatorValue = child->children->at(0)->value;
				break;
			}
		}	
		node->result = this->Eval(node->children, operatorValue);
	}

	return node->result;
}

bool CClousureEvaluator::EvalPreCondition() {
	std::vector<CAttributeArgument*>* arguments = attributeInfo->arguments;
	ASSERT(arguments->size() >= 2);
	CAttributeArgument* preCondition = arguments->at(0);
	LALR* lalr = parser->GetReductionTree(preCondition->tokens);

	CNode* tree = new CNode();
	Symbol* parsedTree = lalr->parse(preCondition->tokens, true, true);
	lalr->printReductionTree(parsedTree, 0);
	//this->PrintReductionTree(parsedTree);
	this->RewriteReductionTree(tree, parsedTree);
	bool result = this->EvaluateNode(tree);
	//this->PrintNodeTree(tree,0);
	if (result == true) {
		LOG4CXX_INFO(clousureEvaluatorLogger, "TRUE !!!");
	}else{
		LOG4CXX_INFO(clousureEvaluatorLogger, "FALSE !!!");
	}
	return result;
}

bool CClousureEvaluator::EvalPostCondition( COR_PRF_FUNCTION_ARGUMENT_RANGE *retvalRange) {
	CParam* returnValue = this->methodInfo->GetReturnValue();
	returnValue->addressToParameterValue = retvalRange->startAddress;
	
	ParsePostCondition(returnValue);
	std::vector<CAttributeArgument*>* arguments = attributeInfo->arguments;
	ASSERT(arguments->size() >= 2);
	CAttributeArgument* postCondtion = arguments->at(1);
	LALR* lalr = parser->GetReductionTree(postCondtion->tokens);
	CNode* tree = new CNode();
	Symbol* parsedTree = lalr->parse(postCondtion->tokens, true, true);
	lalr->printReductionTree(parsedTree, 0);
	this->RewriteReductionTree(tree, parsedTree);
	this->PrintNodeTree(tree,0);

	bool result = this->EvaluateNode(tree);
	if (result == true) {
		LOG4CXX_INFO(clousureEvaluatorLogger, "TRUE !!!");
	}else{
		LOG4CXX_INFO(clousureEvaluatorLogger, "FALSE !!!");
	}
	return result;
}

void CClousureEvaluator::Initialize()
{
	ParsePreCondition();
}

void CClousureEvaluator::BuildParamChain(CParam* param, std::vector<std::wstring> paramPath) 
{
	IMetaDataImport* metaData = this->methodInfo->GetMetaDataImport();
	ASSERT(false);
	mdParamDef* argumentTokens = this->methodInfo->argumentsTokens;
	
	// Start from second element, as first one is passed CParam
	CParam* currentParam = param;
	for (UINT i = 1; i < 2 /*paramPath.size()*/; ++i) {
		currentParam->children = new std::vector<CNode*>();
		mdFieldDef field = NULL;
		HRESULT hr = metaData->FindMember(param->paramTypeToken, paramPath[i].c_str(), NULL, 0, &field);
		if (!SUCCEEDED(hr)) 
			ASSERT(false);
		CParam* child = new CParam();
		currentParam = child;
	}
}

void CClousureEvaluator::ParsePreCondition() {

	std::vector<CAttributeArgument*>* arguments = attributeInfo->arguments;
	ASSERT(arguments->size() >= 2);
	
	CAttributeArgument* preCondition = arguments->at(0);
	for (ULONG j = 0; j < preCondition->tokens.size(); ++j) {
		std::vector<CParam*>* params =  this->methodInfo->GetArguments();
		wstring tokenValue(preCondition->tokens.at(j)->image);
		
		std::vector<std::wstring> parts = Split(tokenValue, '.');
		vector<CParam*>::iterator iter = params->begin();
		for (; iter != params->end(); ++iter) {
			CParam* param = (*iter);
			param->cpi = this->cpi;
			wstring paramName(param->paramName);
			if (tokenToParamMap->find(tokenValue) == tokenToParamMap->end()) {
				// HACK
				if (parts[0].compare(paramName) == 0) {
					tokenToParamMap->insert(pair<wstring, CParam*>(tokenValue, param));
					if (parts.size() > 1) { // we are dealing with compound type
						this->BuildParamChain(param, parts);
					}
				}
			}
		}
	}
}

void CClousureEvaluator::ParsePostCondition(CParam* returnValue) {
	
	std::vector<CAttributeArgument*>* arguments = attributeInfo->arguments;
	ASSERT(arguments->size() >= 2);
	CAttributeArgument* postCondition = arguments->at(1);
	for (ULONG j = 0; j < postCondition->tokens.size(); ++j) {
		std::vector<CParam*>* params =  this->methodInfo->GetArguments();
		vector<CParam*>::iterator iter = params->begin();
		for (; iter != params->end(); ++iter) {
			CParam* param = (*iter);
			// Rewrite
			param->cpi = this->cpi;
			wstring tokenValue(postCondition->tokens.at(j)->image);
			wstring paramName(param->paramName);
			Operator::TokenTypeEnum tokenType = Operator::GetTokenType(postCondition->tokens.at(j)->symbol);
			if (tokenType == Operator::InitialValue) {
				paramName = L"^" + paramName;
			}
			if (tokenToParamMap->find(tokenValue) == tokenToParamMap->end()) {
				
				if (tokenType == Operator::ReturnValue) {
						tokenToParamMap->insert(pair<wstring, CParam*>(tokenValue, returnValue));
				}
				// HACK
				if (tokenValue.compare(paramName) == 0) {
					if (tokenType == Operator::InitialValue) {
						param = param->GetCopy();
						param->PersistData();
					}
					
					tokenToParamMap->insert(pair<wstring, CParam*>(tokenValue, param));
				}
			}
		}
	}
}

CAttributeInfo* CClousureEvaluator::GetAttributeInfo() {
	return this->attributeInfo;
}

CMethodInfo* CClousureEvaluator::GetMethodInfo() {
	return this->methodInfo;
}