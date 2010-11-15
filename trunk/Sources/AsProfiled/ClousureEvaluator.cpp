#include "StdAfx.h"
#include "ClousureEvaluator.h"
#include "cor.h"
#include "MethodInfo.h"
#include "AttributeArgument.h"
#include "Operator.h"

using namespace log4cxx;
using namespace std;


LoggerPtr clousureEvaluatorLogger(Logger::getLogger("ce"));

CClousureEvaluator::CClousureEvaluator(CMethodInfo* methodInfo, CAttributeInfo* attributeInfoArg)
{
	this->attributeInfo = attributeInfoArg;
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

bool CClousureEvaluator::EvalPreCondition() {
	std::vector<CAttributeArgument*>* arguments = attributeInfo->arguments;
		for (ULONG i = 0 ; i < arguments->size(); ++i) {
			CAttributeArgument* arg = arguments->at(i);
			LOG4CXX_INFO(clousureEvaluatorLogger, arg->argumentValue);
			for (ULONG j = 0; j < arg->tokens.size(); ++j) {
				LOG4CXX_INFO(clousureEvaluatorLogger, arg->tokens.at(j)->symbol);
				Operator::TokenTypeEnum type = Operator::GetTokenType(arg->tokens.at(j)->symbol);
				LOG4CXX_INFO(clousureEvaluatorLogger, "Token type is " << (int) type);
			}
		}
	return true;
}

bool CClousureEvaluator::EvalPostCondition() {
	return true;
}