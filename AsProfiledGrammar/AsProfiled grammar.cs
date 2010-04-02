
using System;
using System.IO;
using System.Runtime.Serialization;
using com.calitha.goldparser.lalr;
using com.calitha.commons;

namespace com.calitha.goldparser
{

    [Serializable()]
    public class SymbolException : System.Exception
    {
        public SymbolException(string message) : base(message)
        {
        }

        public SymbolException(string message,
            Exception inner) : base(message, inner)
        {
        }

        protected SymbolException(SerializationInfo info,
            StreamingContext context) : base(info, context)
        {
        }

    }

    [Serializable()]
    public class RuleException : System.Exception
    {

        public RuleException(string message) : base(message)
        {
        }

        public RuleException(string message,
                             Exception inner) : base(message, inner)
        {
        }

        protected RuleException(SerializationInfo info,
                                StreamingContext context) : base(info, context)
        {
        }

    }

    enum SymbolConstants : int
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

    enum RuleConstants : int
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

    public class MyParser
    {
        private LALRParser parser;

        public MyParser(string filename)
        {
            FileStream stream = new FileStream(filename,
                                               FileMode.Open, 
                                               FileAccess.Read, 
                                               FileShare.Read);
            Init(stream);
            stream.Close();
        }

        public MyParser(string baseName, string resourceName)
        {
            byte[] buffer = ResourceUtil.GetByteArrayResource(
                System.Reflection.Assembly.GetExecutingAssembly(),
                baseName,
                resourceName);
            MemoryStream stream = new MemoryStream(buffer);
            Init(stream);
            stream.Close();
        }

        public MyParser(Stream stream)
        {
            Init(stream);
        }

        private void Init(Stream stream)
        {
            CGTReader reader = new CGTReader(stream);
            parser = reader.CreateNewParser();
            parser.TrimReductions = false;
            parser.StoreTokens = LALRParser.StoreTokensMode.NoUserObject;

            parser.OnTokenError += new LALRParser.TokenErrorHandler(TokenErrorEvent);
            parser.OnParseError += new LALRParser.ParseErrorHandler(ParseErrorEvent);
        }

        public void Parse(string source)
        {
            NonterminalToken token = parser.Parse(source);
            if (token != null)
            {
                Object obj = CreateObject(token);
                //todo: Use your object any way you like
            }
        }

        private Object CreateObject(Token token)
        {
            if (token is TerminalToken)
                return CreateObjectFromTerminal((TerminalToken)token);
            else
                return CreateObjectFromNonterminal((NonterminalToken)token);
        }

        private Object CreateObjectFromTerminal(TerminalToken token)
        {
            switch (token.Symbol.Id)
            {
                case (int)SymbolConstants.SYMBOL_EOF :
                //(EOF)
                //todo: Create a new object that corresponds to the symbol
                return null;

                case (int)SymbolConstants.SYMBOL_ERROR :
                //(Error)
                //todo: Create a new object that corresponds to the symbol
                return null;

                case (int)SymbolConstants.SYMBOL_WHITESPACE :
                //(Whitespace)
                //todo: Create a new object that corresponds to the symbol
                return null;

                case (int)SymbolConstants.SYMBOL_MINUS :
                //'-'
                //todo: Create a new object that corresponds to the symbol
                return null;

                case (int)SymbolConstants.SYMBOL_EXCLAMEQ :
                //'!='
                //todo: Create a new object that corresponds to the symbol
                return null;

                case (int)SymbolConstants.SYMBOL_AMP :
                //'&'
                //todo: Create a new object that corresponds to the symbol
                return null;

                case (int)SymbolConstants.SYMBOL_AMPAMP :
                //'&&'
                //todo: Create a new object that corresponds to the symbol
                return null;

                case (int)SymbolConstants.SYMBOL_LPARAN :
                //'('
                //todo: Create a new object that corresponds to the symbol
                return null;

                case (int)SymbolConstants.SYMBOL_RPARAN :
                //')'
                //todo: Create a new object that corresponds to the symbol
                return null;

                case (int)SymbolConstants.SYMBOL_TIMES :
                //'*'
                //todo: Create a new object that corresponds to the symbol
                return null;

                case (int)SymbolConstants.SYMBOL_DIV :
                //'/'
                //todo: Create a new object that corresponds to the symbol
                return null;

                case (int)SymbolConstants.SYMBOL_PIPE :
                //'|'
                //todo: Create a new object that corresponds to the symbol
                return null;

                case (int)SymbolConstants.SYMBOL_PIPEPIPE :
                //'||'
                //todo: Create a new object that corresponds to the symbol
                return null;

                case (int)SymbolConstants.SYMBOL_PLUS :
                //'+'
                //todo: Create a new object that corresponds to the symbol
                return null;

                case (int)SymbolConstants.SYMBOL_LT :
                //'<'
                //todo: Create a new object that corresponds to the symbol
                return null;

                case (int)SymbolConstants.SYMBOL_LTEQ :
                //'<='
                //todo: Create a new object that corresponds to the symbol
                return null;

                case (int)SymbolConstants.SYMBOL_EQEQ :
                //'=='
                //todo: Create a new object that corresponds to the symbol
                return null;

                case (int)SymbolConstants.SYMBOL_GT :
                //'>'
                //todo: Create a new object that corresponds to the symbol
                return null;

                case (int)SymbolConstants.SYMBOL_GTEQ :
                //'>='
                //todo: Create a new object that corresponds to the symbol
                return null;

                case (int)SymbolConstants.SYMBOL_BOOLEANLITERAL :
                //BooleanLiteral
                //todo: Create a new object that corresponds to the symbol
                return null;

                case (int)SymbolConstants.SYMBOL_DECIMALNUMBER :
                //DecimalNumber
                //todo: Create a new object that corresponds to the symbol
                return null;

                case (int)SymbolConstants.SYMBOL_IDENTIFIER :
                //Identifier
                //todo: Create a new object that corresponds to the symbol
                return null;

                case (int)SymbolConstants.SYMBOL_STRINGLITERAL :
                //StringLiteral
                //todo: Create a new object that corresponds to the symbol
                return null;

                case (int)SymbolConstants.SYMBOL_ADDEXP :
                //<Add Exp>
                //todo: Create a new object that corresponds to the symbol
                return null;

                case (int)SymbolConstants.SYMBOL_BITEXP :
                //<Bit Exp>
                //todo: Create a new object that corresponds to the symbol
                return null;

                case (int)SymbolConstants.SYMBOL_BOOLEANEXP :
                //<Boolean Exp>
                //todo: Create a new object that corresponds to the symbol
                return null;

                case (int)SymbolConstants.SYMBOL_CMPEXP :
                //<Cmp Exp>
                //todo: Create a new object that corresponds to the symbol
                return null;

                case (int)SymbolConstants.SYMBOL_MULTEXP :
                //<Mult Exp>
                //todo: Create a new object that corresponds to the symbol
                return null;

                case (int)SymbolConstants.SYMBOL_NEGATEEXP :
                //<Negate Exp>
                //todo: Create a new object that corresponds to the symbol
                return null;

                case (int)SymbolConstants.SYMBOL_PROGRAM :
                //<Program>
                //todo: Create a new object that corresponds to the symbol
                return null;

                case (int)SymbolConstants.SYMBOL_VALUE :
                //<Value>
                //todo: Create a new object that corresponds to the symbol
                return null;

            }
            throw new SymbolException("Unknown symbol");
        }

        public Object CreateObjectFromNonterminal(NonterminalToken token)
        {
            switch (token.Rule.Id)
            {
                case (int)RuleConstants.RULE_PROGRAM :
                //<Program> ::= <Boolean Exp>
                //todo: Create a new object using the stored tokens.
                return null;

                case (int)RuleConstants.RULE_BOOLEANEXP_PIPEPIPE :
                //<Boolean Exp> ::= <Boolean Exp> '||' <Cmp Exp>
                //todo: Create a new object using the stored tokens.
                return null;

                case (int)RuleConstants.RULE_BOOLEANEXP_AMPAMP :
                //<Boolean Exp> ::= <Boolean Exp> '&&' <Cmp Exp>
                //todo: Create a new object using the stored tokens.
                return null;

                case (int)RuleConstants.RULE_BOOLEANEXP :
                //<Boolean Exp> ::= <Cmp Exp>
                //todo: Create a new object using the stored tokens.
                return null;

                case (int)RuleConstants.RULE_CMPEXP_GT :
                //<Cmp Exp> ::= <Cmp Exp> '>' <Add Exp>
                //todo: Create a new object using the stored tokens.
                return null;

                case (int)RuleConstants.RULE_CMPEXP_LT :
                //<Cmp Exp> ::= <Cmp Exp> '<' <Add Exp>
                //todo: Create a new object using the stored tokens.
                return null;

                case (int)RuleConstants.RULE_CMPEXP_LTEQ :
                //<Cmp Exp> ::= <Cmp Exp> '<=' <Add Exp>
                //todo: Create a new object using the stored tokens.
                return null;

                case (int)RuleConstants.RULE_CMPEXP_GTEQ :
                //<Cmp Exp> ::= <Cmp Exp> '>=' <Add Exp>
                //todo: Create a new object using the stored tokens.
                return null;

                case (int)RuleConstants.RULE_CMPEXP_EQEQ :
                //<Cmp Exp> ::= <Cmp Exp> '==' <Add Exp>
                //todo: Create a new object using the stored tokens.
                return null;

                case (int)RuleConstants.RULE_CMPEXP_EXCLAMEQ :
                //<Cmp Exp> ::= <Cmp Exp> '!=' <Add Exp>
                //todo: Create a new object using the stored tokens.
                return null;

                case (int)RuleConstants.RULE_CMPEXP :
                //<Cmp Exp> ::= <Add Exp>
                //todo: Create a new object using the stored tokens.
                return null;

                case (int)RuleConstants.RULE_ADDEXP_PLUS :
                //<Add Exp> ::= <Add Exp> '+' <Mult Exp>
                //todo: Create a new object using the stored tokens.
                return null;

                case (int)RuleConstants.RULE_ADDEXP_MINUS :
                //<Add Exp> ::= <Add Exp> '-' <Mult Exp>
                //todo: Create a new object using the stored tokens.
                return null;

                case (int)RuleConstants.RULE_ADDEXP :
                //<Add Exp> ::= <Mult Exp>
                //todo: Create a new object using the stored tokens.
                return null;

                case (int)RuleConstants.RULE_MULTEXP_TIMES :
                //<Mult Exp> ::= <Mult Exp> '*' <Negate Exp>
                //todo: Create a new object using the stored tokens.
                return null;

                case (int)RuleConstants.RULE_MULTEXP_DIV :
                //<Mult Exp> ::= <Mult Exp> '/' <Negate Exp>
                //todo: Create a new object using the stored tokens.
                return null;

                case (int)RuleConstants.RULE_MULTEXP :
                //<Mult Exp> ::= <Bit Exp>
                //todo: Create a new object using the stored tokens.
                return null;

                case (int)RuleConstants.RULE_BITEXP_AMP :
                //<Bit Exp> ::= <Bit Exp> '&' <Negate Exp>
                //todo: Create a new object using the stored tokens.
                return null;

                case (int)RuleConstants.RULE_BITEXP_PIPE :
                //<Bit Exp> ::= <Bit Exp> '|' <Negate Exp>
                //todo: Create a new object using the stored tokens.
                return null;

                case (int)RuleConstants.RULE_BITEXP :
                //<Bit Exp> ::= <Negate Exp>
                //todo: Create a new object using the stored tokens.
                return null;

                case (int)RuleConstants.RULE_NEGATEEXP_MINUS :
                //<Negate Exp> ::= '-' <Value>
                //todo: Create a new object using the stored tokens.
                return null;

                case (int)RuleConstants.RULE_NEGATEEXP :
                //<Negate Exp> ::= <Value>
                //todo: Create a new object using the stored tokens.
                return null;

                case (int)RuleConstants.RULE_VALUE_IDENTIFIER :
                //<Value> ::= Identifier
                //todo: Create a new object using the stored tokens.
                return null;

                case (int)RuleConstants.RULE_VALUE_STRINGLITERAL :
                //<Value> ::= StringLiteral
                //todo: Create a new object using the stored tokens.
                return null;

                case (int)RuleConstants.RULE_VALUE_DECIMALNUMBER :
                //<Value> ::= DecimalNumber
                //todo: Create a new object using the stored tokens.
                return null;

                case (int)RuleConstants.RULE_VALUE_LPARAN_RPARAN :
                //<Value> ::= '(' <Cmp Exp> ')'
                //todo: Create a new object using the stored tokens.
                return null;

                case (int)RuleConstants.RULE_VALUE_BOOLEANLITERAL :
                //<Value> ::= BooleanLiteral
                //todo: Create a new object using the stored tokens.
                return null;

            }
            throw new RuleException("Unknown rule");
        }

        private void TokenErrorEvent(LALRParser parser, TokenErrorEventArgs args)
        {
            string message = "Token error with input: '"+args.Token.ToString()+"'";
            //todo: Report message to UI?
        }

        private void ParseErrorEvent(LALRParser parser, ParseErrorEventArgs args)
        {
            string message = "Parse error caused by token: '"+args.UnexpectedToken.ToString()+"'";
            //todo: Report message to UI?
        }

    }
}
