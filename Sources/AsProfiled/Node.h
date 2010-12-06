#pragma once

#include <vector>
#include "Operator.h"

enum NodeType { BaseNode, ParamNode };

class CNode
{
public:
	
	CNode(void);
	~CNode(void);
	virtual NodeType GetType();
	virtual std::wstring GetValue();
	bool IsTerminal();
	Operator::TokenTypeEnum tokenType;
	std::vector<CNode*>* children;
	bool result;
	std::wstring value;
	bool evaluated;
};
