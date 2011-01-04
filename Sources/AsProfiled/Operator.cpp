#include "StdAfx.h"
#include "Operator.h"

namespace Operator {
	
	std::wstring LeftBracket = L"(";
	std::wstring RightBracket = L")";

	///// Compare operators
	std::wstring LessOp = L"<";
	std::wstring LessOrEqualOp = L"<=";
	std::wstring EqualOp = L"==";
	std::wstring NotEqualOp = L"!=";
	std::wstring GreaterOp = L">";
	std::wstring GreaterOrEqualOp = L"<=";

	///// Boolean operators
	std::wstring OrOp = L"||";
	std::wstring AndOp = L"&&";

	//// Additive operators
	std::wstring AddOp = L"+";
	std::wstring MinusOp = L"-";

	//// Multiplicative operators
	std::wstring MultOp = L"*";
	std::wstring DivideOp = L"/";

	//// Bit operators
	std::wstring BitAndOp = L"&";
	std::wstring BitOrOp = L"|";

	/// Non terminals
	std::wstring AddNT = L"";
	std::wstring MultNT = L"";
	std::wstring BitNT = L"";
	std::wstring BoolNT = L"";
	std::wstring ProgramNT = L"";


	OperatorType Operator::GetOperatorType(const WCHAR* operatorLiteral) {
		std::wstring wstringOperator(operatorLiteral);
		if (wstringOperator.compare(LessOp) == 0 || wstringOperator.compare(LessOrEqualOp) == 0
			|| wstringOperator.compare(EqualOp) == 0 || wstringOperator.compare(NotEqualOp) == 0
			|| wstringOperator.compare(GreaterOp) == 0 || wstringOperator.compare(GreaterOrEqualOp) == 0) {
				return CmpOperatorType;
		}else if (wstringOperator.compare(OrOp) == 0 || wstringOperator.compare(AndOp) == 0) {
			return BooleanOperatorType;
		}else if (wstringOperator.compare(AddOp) == 0 || wstringOperator.compare(MinusOp) == 0) {
			return AddOperatorType;
		}else if (wstringOperator.compare(MultOp) == 0 || wstringOperator.compare(DivideOp) == 0) {
			return MultOperatorType;
		}else if (wstringOperator.compare(BitAndOp) == 0 || wstringOperator.compare(BitOrOp) == 0) {
			return BitOperatorType;
		}
		return NotOp;
	}	

	TokenTypeEnum Operator::GetTokenType(const WCHAR* tokenLiteral) {
		std::wstring wstringToken(tokenLiteral);
		if (GetOperatorType(tokenLiteral) != NotOp) {
			return Operator;
		}else if (wstringToken.find(L"Operator") != std::wstring::npos) {
			return OperatorNT;
		}else if (wstringToken.compare(LeftBracket) == 0 ||
					wstringToken.compare(RightBracket) == 0) {
			return Bracket;
		}else if (wstringToken.compare(L"Identifier") == 0) {
			return Identifier;
		}else if (wstringToken.compare(L"ReturnValue") == 0) {
			return ReturnValue;
		}else if (wstringToken.compare(L"BooleanLiteral") == 0) {
			return BooleanLiteral;
		}else if (wstringToken.compare(L"DecimalNumber") == 0) {
			return DecimalNumber;
		}else if (wstringToken.compare(L"StringLiteral") == 0) {
			return StringLiteral;
		}else if (wstringToken.compare(L"InitialValue") == 0) {
			return InitialValue;
		}else if (wstringToken.compare(L"EOF") == 0) {
			return EOS;
		}else if (wstringToken.compare(L"Program") == 0) {
			return StartSymbol;
		}
		return Expression;
	}

	CmpOperator Operator::GetCmpOperator(const WCHAR* operatorAsString) {
		std::wstring stringRepresentation(operatorAsString);
		if (stringRepresentation.compare(L"<") == 0) {
			return Less;
		}else if(stringRepresentation.compare( L"<=") == 0) {
			return LessOrEqual;
		}else if(stringRepresentation.compare( L"==") == 0) {
			return Equal;
		}else if(stringRepresentation.compare( L"!=") == 0) {
			return NotEqual;
		}else if(stringRepresentation.compare( L">") == 0) {
			return Greater;
		}else if(stringRepresentation.compare( L">=") == 0) {
			return GreaterOrEqual;
		}
		return NotCmp;
	}

	BooleanOperator Operator::GetBooleanOperator(const WCHAR* operatorAsString) {
		std::wstring stringRepresentation(operatorAsString);
		if (stringRepresentation.compare(OrOp) == 0) {
			return BoolOr;
		}else if (stringRepresentation.compare(AndOp) == 0) {
			return BoolAnd;
		}
		return NotBool;
	}

	MultOperator Operator::GetMultOperator(const WCHAR* operatorAsString) {
		std::wstring stringRepresentation(operatorAsString);
		if (stringRepresentation.compare(MultOp) == 0) {
			return Multiply;
		}else if (stringRepresentation.compare(DivideOp) == 0) {
			return Divide;
		}
		return NotMult;
	}

	BitOperator Operator::GetBitOperator(const WCHAR* operatorAsString) {
		std::wstring stringRepresentation(operatorAsString);
		if (stringRepresentation.compare(BitOrOp) == 0) {
			return BitOr;
		}else if (stringRepresentation.compare(BitAndOp) == 0) {
			return BitAnd;
		}
		return NotBit;
	}

	AddOperator Operator::GetAddOperator(const WCHAR* operatorAsString) {
		std::wstring stringRepresentation(operatorAsString);
		if (stringRepresentation.compare(AddOp) == 0) {
			return Add;
		}else if (stringRepresentation.compare(MinusOp) == 0) {
			return Minus;
		}
		return NotAdd;
	}
}