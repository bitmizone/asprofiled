#pragma once

using namespace std;
namespace Operator {

wstring LeftBracket = L"(";
wstring RightBracker = L")";

/// Compare operators
wstring LessOp = L"<";
wstring LessOrEqualOp = L"<=";
wstring EqualOp = L"==";
wstring NotEqualOp = L"!=";
wstring GreaterOp = L"<";
wstring GreaterOrEqualOp = L"<=";

/// Boolean operators
wstring OrOp = L"||";
wstring AndOp = L"&&";

// Additive operators
wstring AddOp = L"+";
wstring MinusOp = L"-";

// Multiplicative operators
wstring MultOp = L"*";
wstring DivideOp = L"/";

// Bit operators
wstring BitAndOp = L"&";
wstring BitOrOp = L"|";

enum CmpOperator { NotCmp, Less, LessOrEqual, Equal, NotEqual, Greater, GreaterOrEqual };
enum BooleanOperator {NotBool, BoolAnd, BoolOr};
enum AddOperator { NotAdd, Add, Minus };
enum MultOpeator { NotMult, Multiply, Divide };
enum BitOperator { NotBit, BitAnd, BitOr };
enum OperatorType {	NotOp, 
					CmpOperatorType, 
					BooleanOperatorType, 
                    AddOperatorType,
                    MultOpeatorType,
                    BitOperatorType
};

enum TokenTypeEnum { BadToken, Identifier, ReturnValue, Operator, BooleanLiteral, DecimalNumber, StringLiteral, InitialValue, EOS };



OperatorType GetOperatorType(WCHAR* operatorLiteral) {
	wstring wstringOperator(operatorLiteral);
	if (wstringOperator.compare(LessOp) == 0 || wstringOperator.compare(LessOrEqualOp) == 0
		|| wstringOperator.compare(EqualOp) == 0 || wstringOperator.compare(NotEqualOp) == 0
		|| wstringOperator.compare(GreaterOp) == 0 || wstringOperator.compare(GreaterOrEqualOp) == 0) {
			return CmpOperatorType;
	}else if (wstringOperator.compare(OrOp) == 0 || wstringOperator.compare(AndOp) == 0) {
		return BooleanOperatorType;
	}else if (wstringOperator.compare(AddOp) == 0 || wstringOperator.compare(MinusOp) == 0) {
		return AddOperatorType;
	}else if (wstringOperator.compare(MultOp) == 0 || wstringOperator.compare(DivideOp) == 0) {
		return MultOpeatorType;
	}else if (wstringOperator.compare(BitAndOp) == 0 || wstringOperator.compare(BitOrOp) == 0) {
		return BitOperatorType;
	}
	return NotOp;
}	

TokenTypeEnum GetTokenType(WCHAR* tokenLiteral) {
	wstring wstringToken(tokenLiteral);
	if (GetOperatorType(tokenLiteral) != NotOp) {
		return Operator;
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
	}
	return BadToken;
}

CmpOperator GetCmpOperator(WCHAR* operatorAsString) {
	wstring stringRepresentation(operatorAsString);
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

BooleanOperator GetBooleanOperator(WCHAR* operatorAsString) {
	wstring stringRepresentation(operatorAsString);
	if (stringRepresentation.compare(OrOp) == 0) {
		return BoolOr;
	}else if (stringRepresentation.compare(AndOp) == 0) {
		return BoolAnd;
	}
	return NotBool;
}

MultOpeator GetMultOperator(WCHAR* operatorAsString) {
	wstring stringRepresentation(operatorAsString);
	if (stringRepresentation.compare(MultOp) == 0) {
		return Multiply;
	}else if (stringRepresentation.compare(DivideOp) == 0) {
		return Divide;
	}
	return NotMult;
}

BitOperator GetBitOperator(WCHAR* operatorAsString) {
	wstring stringRepresentation(operatorAsString);
	if (stringRepresentation.compare(BitOrOp) == 0) {
		return BitOr;
	}else if (stringRepresentation.compare(BitAndOp) == 0) {
		return BitAnd;
	}
	return NotBit;
}

AddOperator GetAddOperator(WCHAR* operatorAsString) {
	wstring stringRepresentation(operatorAsString);
	if (stringRepresentation.compare(AddOp) == 0) {
		return Add;
	}else if (stringRepresentation.compare(MinusOp) == 0) {
		return Minus;
	}
	return NotAdd;
}
}