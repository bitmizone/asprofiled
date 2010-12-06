#pragma once
#include <string>

namespace Operator {

	enum CmpOperator { NotCmp, Less, LessOrEqual, Equal, NotEqual, Greater, GreaterOrEqual };
	enum BooleanOperator {NotBool, BoolAnd, BoolOr};
	enum AddOperator { NotAdd, Add, Minus };
	enum MultOperator { NotMult, Multiply, Divide };
	enum BitOperator { NotBit, BitAnd, BitOr };
	enum OperatorType {	NotOp, 
						CmpOperatorType, 
						BooleanOperatorType, 
						AddOperatorType,
						MultOperatorType,
						BitOperatorType
	};

	enum TokenTypeEnum { OtherToken, StartSymbol, Identifier, ReturnValue, 
						Operator, BooleanLiteral, DecimalNumber, StringLiteral, 
						InitialValue, Bracket, Expression, OperatorNT, EOS };

	OperatorType GetOperatorType(const WCHAR* operatorLiteral);

	TokenTypeEnum GetTokenType(const WCHAR* tokenLiteral);

	CmpOperator GetCmpOperator(const WCHAR* operatorAsString);

	BooleanOperator GetBooleanOperator(const WCHAR* operatorAsString);

	MultOperator GetMultOperator(const WCHAR* operatorAsString);

	BitOperator GetBitOperator(const WCHAR* operatorAsString);

	AddOperator GetAddOperator(const WCHAR* operatorAsString);
}