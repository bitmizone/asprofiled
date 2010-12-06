#include "StdAfx.h"
#include "Node.h"

CNode::CNode(void)
{
	this->children = NULL;
	// this->children = new std::vector<CNode*>();
	this->tokenType = Operator::OtherToken;
	this->result = false;
	this->evaluated = false;
}

CNode::~CNode(void)
{
	if (this->children != NULL) {
		delete this->children;
	}
	
}

NodeType CNode::GetType() {
	return BaseNode;
}

std::wstring CNode::GetValue() {
	return value;
}

bool CNode::IsTerminal() {
	return this->children == NULL || this->children->size() == 0;
}