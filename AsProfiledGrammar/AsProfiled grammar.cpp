enum SymbolConstants
{
   SYMBOL_EOF            =  0, // (EOF)
   SYMBOL_ERROR          =  1, // (Error)
   SYMBOL_WHITESPACE     =  2, // (Whitespace)
   SYMBOL_MINUS          =  3, // '-'
   SYMBOL_EXCLAMEQ       =  4, // '!='
   SYMBOL_AMP            =  5, // '&'
   SYMBOL_AMPAMP         =  6, // '&&'
   SYMBOL_LPARAN         =  7, // '('
   SYMBOL_RPARAN         =  8, // ')'
   SYMBOL_TIMES          =  9, // '*'
   SYMBOL_DIV            = 10, // '/'
   SYMBOL_PIPE           = 11, // '|'
   SYMBOL_PIPEPIPE       = 12, // '||'
   SYMBOL_PLUS           = 13, // '+'
   SYMBOL_LT             = 14, // '<'
   SYMBOL_LTEQ           = 15, // '<='
   SYMBOL_EQEQ           = 16, // '=='
   SYMBOL_GT             = 17, // '>'
   SYMBOL_GTEQ           = 18, // '>='
   SYMBOL_BOOLEANLITERAL = 19, // BooleanLiteral
   SYMBOL_DECIMALNUMBER  = 20, // DecimalNumber
   SYMBOL_IDENTIFIER     = 21, // Identifier
   SYMBOL_STRINGLITERAL  = 22, // StringLiteral
   SYMBOL_ADDEXP         = 23, // <Add Exp>
   SYMBOL_BITEXP         = 24, // <Bit Exp>
   SYMBOL_BOOLEANEXP     = 25, // <Boolean Exp>
   SYMBOL_CMPEXP         = 26, // <Cmp Exp>
   SYMBOL_MULTEXP        = 27, // <Mult Exp>
   SYMBOL_NEGATEEXP      = 28, // <Negate Exp>
   SYMBOL_PROGRAM        = 29, // <Program>
   SYMBOL_VALUE          = 30  // <Value>
};

enum RuleConstants
{
   RULE_PROGRAM              =  0, // <Program> ::= <Boolean Exp>
   RULE_BOOLEANEXP_PIPEPIPE  =  1, // <Boolean Exp> ::= <Boolean Exp> '||' <Cmp Exp>
   RULE_BOOLEANEXP_AMPAMP    =  2, // <Boolean Exp> ::= <Boolean Exp> '&&' <Cmp Exp>
   RULE_BOOLEANEXP           =  3, // <Boolean Exp> ::= <Cmp Exp>
   RULE_CMPEXP_GT            =  4, // <Cmp Exp> ::= <Cmp Exp> '>' <Add Exp>
   RULE_CMPEXP_LT            =  5, // <Cmp Exp> ::= <Cmp Exp> '<' <Add Exp>
   RULE_CMPEXP_LTEQ          =  6, // <Cmp Exp> ::= <Cmp Exp> '<=' <Add Exp>
   RULE_CMPEXP_GTEQ          =  7, // <Cmp Exp> ::= <Cmp Exp> '>=' <Add Exp>
   RULE_CMPEXP_EQEQ          =  8, // <Cmp Exp> ::= <Cmp Exp> '==' <Add Exp>
   RULE_CMPEXP_EXCLAMEQ      =  9, // <Cmp Exp> ::= <Cmp Exp> '!=' <Add Exp>
   RULE_CMPEXP               = 10, // <Cmp Exp> ::= <Add Exp>
   RULE_ADDEXP_PLUS          = 11, // <Add Exp> ::= <Add Exp> '+' <Mult Exp>
   RULE_ADDEXP_MINUS         = 12, // <Add Exp> ::= <Add Exp> '-' <Mult Exp>
   RULE_ADDEXP               = 13, // <Add Exp> ::= <Mult Exp>
   RULE_MULTEXP_TIMES        = 14, // <Mult Exp> ::= <Mult Exp> '*' <Negate Exp>
   RULE_MULTEXP_DIV          = 15, // <Mult Exp> ::= <Mult Exp> '/' <Negate Exp>
   RULE_MULTEXP              = 16, // <Mult Exp> ::= <Bit Exp>
   RULE_BITEXP_AMP           = 17, // <Bit Exp> ::= <Bit Exp> '&' <Negate Exp>
   RULE_BITEXP_PIPE          = 18, // <Bit Exp> ::= <Bit Exp> '|' <Negate Exp>
   RULE_BITEXP               = 19, // <Bit Exp> ::= <Negate Exp>
   RULE_NEGATEEXP_MINUS      = 20, // <Negate Exp> ::= '-' <Value>
   RULE_NEGATEEXP            = 21, // <Negate Exp> ::= <Value>
   RULE_VALUE_IDENTIFIER     = 22, // <Value> ::= Identifier
   RULE_VALUE_STRINGLITERAL  = 23, // <Value> ::= StringLiteral
   RULE_VALUE_DECIMALNUMBER  = 24, // <Value> ::= DecimalNumber
   RULE_VALUE_LPARAN_RPARAN  = 25, // <Value> ::= '(' <Cmp Exp> ')'
   RULE_VALUE_BOOLEANLITERAL = 26  // <Value> ::= BooleanLiteral
};
