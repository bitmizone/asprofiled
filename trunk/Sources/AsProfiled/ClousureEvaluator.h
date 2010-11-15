#pragma once

#include "cor.h"
#include "MethodInfo.h"
#include "AttributeInfo.h"
#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
using namespace log4cxx;


class CClousureEvaluator
{
public:
	CClousureEvaluator(CMethodInfo* methodInfo, CAttributeInfo* attributeInfo);
	~CClousureEvaluator(void);
	bool EvalPreCondition();
	bool EvalPostCondition();
private:
	CAttributeInfo* attributeInfo;
	
};
