#include "StdAfx.h"
#include "ClousureEvaluator.h"
#include "cor.h"
#include "MethodInfo.h"
#include "AttributeArgument.h"
#include "Operator.h"
#include "ValueReader.h"
#include <vector>
using namespace log4cxx;
using namespace std;


LoggerPtr clousureEvaluatorLogger(Logger::getLogger("ce"));

CGrammarParser* CClousureEvaluator::parser = new CGrammarParser("c:\\grammar.cgt");

CClousureEvaluator::CClousureEvaluator(CMethodInfo* methodInfoArg, CAttributeInfo* attributeInfoArg, ICorProfilerInfo2* corProfilerInfo)
{
	this->attributeInfo = attributeInfoArg;
	this->methodInfo = methodInfoArg;
	this->cpi = corProfilerInfo;
	this->initialized = false;
	this->tokenToParamMap = new std::map<wstring, CParam*>();
	// Configure Log4cxx
	if (clousureEvaluatorLogger->isInfoEnabled() == false) {
		BasicConfigurator::configure();
		// Levels hierarchy: TRACE < DEBUG < INFO < WARN < ERROR < FATAL
		clousureEvaluatorLogger->setLevel(Level::toLevel(log4cxx::Level::INFO_INT));
	}
}

CClousureEvaluator::~CClousureEvaluator(void)
{
	
}

void CClousureEvaluator::PrintReductionTree(Symbol* reductionTree) {
	if (reductionTree == NULL) {
		return;
	}
	if (reductionTree->type == NON_TERMINAL) {
		NonTerminal* nonTerminal = (NonTerminal*) reductionTree;
		for (UINT i = 0 ; i < nonTerminal->children.size(); ++i) {
			PrintReductionTree(nonTerminal->children[i]);
		}
	}else {
		Terminal* terminal = (Terminal*) reductionTree;
		LOG4CXX_INFO(clousureEvaluatorLogger, terminal->image.c_str());
		LOG4CXX_INFO(clousureEvaluatorLogger, terminal->symbol.c_str());
	}
}

bool CClousureEvaluator::EvalPreCondition(COR_PRF_FUNCTION_ARGUMENT_INFO* argumentInfo) {
	if (this->initialized == false) {
		Initialize();
	}
	std::vector<CAttributeArgument*>* arguments = attributeInfo->arguments;
	ASSERT(arguments->size() >= 2);
	CAttributeArgument* preCondition = arguments->at(0);
	LALR* lalr = parser->GetReductionTree(preCondition->tokens);
	//this->PrintReductionTree(lalr->parse(preCondition->tokens,true, true));
	lalr->printReductionTree(lalr->parse(preCondition->tokens, true, true), 0);
	std::vector<Token*>::iterator iter = preCondition->tokens.begin();
	for ( ; iter != preCondition->tokens.end(); ++iter) {
		Token* token = (*iter);
		Operator::TokenTypeEnum tokenType = Operator::GetTokenType(token->symbol);
		wstring tokenValue(token->image);
		
		CParam* param = NULL;
		map<wstring, CParam*>::iterator iter = tokenToParamMap->find(tokenValue);
		switch (tokenType) {
			case Operator::Identifier :
				
				if (iter != tokenToParamMap->end()) {
					param = (*iter).second;
				}
				break;
			default:
				break;
		}
	}
	
	return true;
}

bool CClousureEvaluator::EvalPostCondition() {
	if (this->initialized == false) {
		Initialize();
	}
	
	return true;
}

void CClousureEvaluator::TraceValues() {
	std::vector<CParam*>* params =  this->methodInfo->GetArguments();
	vector<CParam*>::iterator iter = params->begin();
	for (; iter != params->end(); ++iter) {
		LOG4CXX_INFO(clousureEvaluatorLogger, (*iter)->paramName);
		CParam* param = *iter;
		switch ((*iter)->elementType) {
			case ELEMENT_TYPE_BOOLEAN :
				CValueReader::GetInstance()->TraceBoolean((*iter)->addressToParameterValue);		
				break;
			case ELEMENT_TYPE_I4 :
				CValueReader::GetInstance()->TraceInt(param->addressToParameterValue);
				break;
			case ELEMENT_TYPE_CHAR :
				CValueReader::GetInstance()->TraceChar(param->addressToParameterValue);
				break;
			case ELEMENT_TYPE_I1:
				CValueReader::GetInstance()->TraceByte(param->addressToParameterValue);
				break;
			case ELEMENT_TYPE_I2:
				CValueReader::GetInstance()->TraceShort(param->addressToParameterValue);
				break;
			case ELEMENT_TYPE_STRING:
				CValueReader::GetInstance()->TraceString(param->addressToParameterValue, cpi);
				break;


			default:
				
				break;
		}
	}
}

void CClousureEvaluator::Initialize()
{
	ParsePreCondition();
	//std::vector<CParam*>* params =  this->methodInfo->GetArguments();
	//vector<CParam*>::iterator iter = params->begin();
	//for (; iter != params->end(); ++iter) {
		//CParam* param = (*iter);
		//for (ULONG i = 0 ; i < arguments->size(); ++i) {
		//	CParam* arg = arguments->at(i);
		//	for (ULONG j = 0; j < arg->tokens.size(); ++j) {
		//		// LOG4CXX_INFO(clousureEvaluatorLogger, arg->tokens.at(j)->symbol);
		//		// Operator::TokenTypeEnum type = Operator::GetTokenType(arg->tokens.at(j)->symbol);
		//		// LOG4CXX_INFO(clousureEvaluatorLogger, "Token type is " << (int) type);
		//	}
		//}
	//}
	this->initialized = true;
}

void CClousureEvaluator::ParsePreCondition() {
	std::vector<CAttributeArgument*>* arguments = attributeInfo->arguments;
	ASSERT(arguments->size() >= 2);
	CAttributeArgument* preCondition = arguments->at(0);
	for (ULONG j = 0; j < preCondition->tokens.size(); ++j) {
		std::vector<CParam*>* params =  this->methodInfo->GetArguments();
		vector<CParam*>::iterator iter = params->begin();
		for (; iter != params->end(); ++iter) {
			CParam* param = (*iter);
			wstring tokenValue(preCondition->tokens.at(j)->image);
			wstring paramName(param->paramName);
			if (tokenToParamMap->find(tokenValue) == tokenToParamMap->end()) {
				if (tokenValue.find(paramName) != wstring.npos) {
					tokenToParamMap->insert(pair<wstring, CParam*>(tokenValue, param));
				}
			}
		}
	}
}

void CClousureEvaluator::ParsePostCondition() {
	std::vector<CAttributeArgument*>* arguments = attributeInfo->arguments;
	ASSERT(arguments->size() >= 2);
	CAttributeArgument* postCondition = arguments->at(1);
	for (ULONG j = 0; j < postCondition->tokens.size(); ++j) {
		
	}
}

CAttributeInfo* CClousureEvaluator::GetAttributeInfo() {
	return this->attributeInfo;
}

CMethodInfo* CClousureEvaluator::GetMethodInfo() {
	return this->methodInfo;
}