/*

This file was generated by the "C - Kessels engine template.c" template.

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "engine.h"                 /* The Kessels engine. */
#include "grammar.h"                /* Generated by GOLD. */



#define TRIMREDUCTIONS 1            /* 0=off, 1=on */
int Debug = 1;                      /* 0=off, 1=on */




/* Load input file from disk into memory. */
char *LoadInputFile(char *FileName) {
  FILE *Fin;
  char *InputBuf;
  struct stat statbuf;
  size_t BytesRead;

  /* Sanity check. */
  if ((FileName == NULL) || (*FileName == '\0')) return(NULL);

  /* Open the file. */
  Fin = fopen(FileName,"rb");
  if (Fin == NULL) {
    fprintf(stdout,"Could not open input file: %s\n",FileName);
    return(NULL);
    }

  /* Get the size of the file. */
  if (fstat(fileno(Fin),&statbuf) != 0) {
    fprintf(stdout,"Could not stat() the input file: %s\n",FileName);
    fclose(Fin);
    return(NULL);
    }

  /* Allocate memory for the input. */
  InputBuf = (char *)malloc(statbuf.st_size + 1);
  if (InputBuf == NULL) {
    fprintf(stdout,"Not enough memory to load the file: %s\n",FileName);
    fclose(Fin);
    return(NULL);
    }

  /* Load the file into memory. */
  BytesRead = fread(InputBuf,1,statbuf.st_size,Fin);
  InputBuf[statbuf.st_size] = '\0';

  /* Close the file. */
  fclose(Fin);

  /* Exit if there was an error while reading the file. */
  if (BytesRead != statbuf.st_size) {
    fprintf(stdout,"Error while reading input file: %s\n",FileName);
    free(InputBuf);
    return(NULL);
    }

  return(InputBuf);
  }




/* Make a readable copy of a string. All characters outside 32...128 are
   displayed as a HEX number in square brackets, for example "[0A]". */
void ReadableString(char *Input, char *Output, long int Width) {
  char s1[BUFSIZ];

  if (Output == NULL) return;
  while ((Input != NULL) && (*Input != '\0')) {
    if ((*Input >= 32) && ((unsigned char)*Input <= 127)) {
        if (Width > 1) *Output++ = *Input;
      } else {
        if (Width > 4) {
          sprintf(s1,"%02X",*Input);
          *Output++ = '[';
          *Output++ = s1[0];
          *Output++ = s1[1];
          *Output++ = ']';
          }
        }
    Input++;
    }
  *Output = '\0';
  }




void ShowErrorMessage(struct ParseDataStruct *Data, char *Message) {
  struct TokenStruct *Token;
  char s1[BUFSIZ];

  Token = Data->TokenStack;
  if (Token != NULL) Token = Token->NextToken;
  if (Token == NULL) {
    fprintf(stdout,"%s.\n",Message);
    return;
    }
  if (Token->Data == NULL) {
    fprintf(stdout,"%s at line %d column %d.\n",Message,Token->Line,
      Token->Column);
    return;
    }
  ReadableString(Token->Data,s1,BUFSIZ);
  fprintf(stdout,"%s at line %d column %d: %s\n",Message,Token->Line,
    Token->Column,s1);
  }




void ShowIndent(int Indent) {
  int i;
  for (i = 0; i < Indent; i++) fprintf(stdout,"  ");
  }




/* Forward declaration of Interpret(); */
char *Interpret(struct TokenStruct *Token, int Debug, int Indent);




/* <Program> ::= <Boolean Exp> */
char *Rule_Program(struct TokenStruct *Token, int Debug, int Indent) {
  /* Arguments: 1 */
  struct TokenStruct *TokenArg;
  char *Value;

  /* This is just an example of how to fetch the arguments. Usually
     you won't need them all. Remember to free() all the results! */
  TokenArg = Token->Reduction->TokenStack;
  while (TokenArg != NULL) {
    Value = Interpret(TokenArg,Debug,Indent);
    if (Value != NULL) free(Value);
    TokenArg = TokenArg->NextToken;
    }

  /* Return a result. In this example it's always NULL, but it could
     be a real result needed by the rule that is calling. */
  return(NULL);
  };



/* <Boolean Exp> ::= <Boolean Exp> '||' <Cmp Exp> */
char *Rule_BooleanExp_PipePipe(struct TokenStruct *Token, int Debug, int Indent) {
  /* Arguments: 3 */
  struct TokenStruct *TokenArg;
  char *Value;

  /* This is just an example of how to fetch the arguments. Usually
     you won't need them all. Remember to free() all the results! */
  TokenArg = Token->Reduction->TokenStack;
  while (TokenArg != NULL) {
    Value = Interpret(TokenArg,Debug,Indent);
    if (Value != NULL) free(Value);
    TokenArg = TokenArg->NextToken;
    }

  /* Return a result. In this example it's always NULL, but it could
     be a real result needed by the rule that is calling. */
  return(NULL);
  };



/* <Boolean Exp> ::= <Boolean Exp> '&&' <Cmp Exp> */
char *Rule_BooleanExp_AmpAmp(struct TokenStruct *Token, int Debug, int Indent) {
  /* Arguments: 3 */
  struct TokenStruct *TokenArg;
  char *Value;

  /* This is just an example of how to fetch the arguments. Usually
     you won't need them all. Remember to free() all the results! */
  TokenArg = Token->Reduction->TokenStack;
  while (TokenArg != NULL) {
    Value = Interpret(TokenArg,Debug,Indent);
    if (Value != NULL) free(Value);
    TokenArg = TokenArg->NextToken;
    }

  /* Return a result. In this example it's always NULL, but it could
     be a real result needed by the rule that is calling. */
  return(NULL);
  };



/* <Boolean Exp> ::= <Cmp Exp> */
char *Rule_BooleanExp(struct TokenStruct *Token, int Debug, int Indent) {
  /* Arguments: 1 */
  struct TokenStruct *TokenArg;
  char *Value;

  /* This is just an example of how to fetch the arguments. Usually
     you won't need them all. Remember to free() all the results! */
  TokenArg = Token->Reduction->TokenStack;
  while (TokenArg != NULL) {
    Value = Interpret(TokenArg,Debug,Indent);
    if (Value != NULL) free(Value);
    TokenArg = TokenArg->NextToken;
    }

  /* Return a result. In this example it's always NULL, but it could
     be a real result needed by the rule that is calling. */
  return(NULL);
  };



/* <Cmp Exp> ::= <Cmp Exp> '>' <Add Exp> */
char *Rule_CmpExp_Gt(struct TokenStruct *Token, int Debug, int Indent) {
  /* Arguments: 3 */
  struct TokenStruct *TokenArg;
  char *Value;

  /* This is just an example of how to fetch the arguments. Usually
     you won't need them all. Remember to free() all the results! */
  TokenArg = Token->Reduction->TokenStack;
  while (TokenArg != NULL) {
    Value = Interpret(TokenArg,Debug,Indent);
    if (Value != NULL) free(Value);
    TokenArg = TokenArg->NextToken;
    }

  /* Return a result. In this example it's always NULL, but it could
     be a real result needed by the rule that is calling. */
  return(NULL);
  };



/* <Cmp Exp> ::= <Cmp Exp> '<' <Add Exp> */
char *Rule_CmpExp_Lt(struct TokenStruct *Token, int Debug, int Indent) {
  /* Arguments: 3 */
  struct TokenStruct *TokenArg;
  char *Value;

  /* This is just an example of how to fetch the arguments. Usually
     you won't need them all. Remember to free() all the results! */
  TokenArg = Token->Reduction->TokenStack;
  while (TokenArg != NULL) {
    Value = Interpret(TokenArg,Debug,Indent);
    if (Value != NULL) free(Value);
    TokenArg = TokenArg->NextToken;
    }

  /* Return a result. In this example it's always NULL, but it could
     be a real result needed by the rule that is calling. */
  return(NULL);
  };



/* <Cmp Exp> ::= <Cmp Exp> '<=' <Add Exp> */
char *Rule_CmpExp_LtEq(struct TokenStruct *Token, int Debug, int Indent) {
  /* Arguments: 3 */
  struct TokenStruct *TokenArg;
  char *Value;

  /* This is just an example of how to fetch the arguments. Usually
     you won't need them all. Remember to free() all the results! */
  TokenArg = Token->Reduction->TokenStack;
  while (TokenArg != NULL) {
    Value = Interpret(TokenArg,Debug,Indent);
    if (Value != NULL) free(Value);
    TokenArg = TokenArg->NextToken;
    }

  /* Return a result. In this example it's always NULL, but it could
     be a real result needed by the rule that is calling. */
  return(NULL);
  };



/* <Cmp Exp> ::= <Cmp Exp> '>=' <Add Exp> */
char *Rule_CmpExp_GtEq(struct TokenStruct *Token, int Debug, int Indent) {
  /* Arguments: 3 */
  struct TokenStruct *TokenArg;
  char *Value;

  /* This is just an example of how to fetch the arguments. Usually
     you won't need them all. Remember to free() all the results! */
  TokenArg = Token->Reduction->TokenStack;
  while (TokenArg != NULL) {
    Value = Interpret(TokenArg,Debug,Indent);
    if (Value != NULL) free(Value);
    TokenArg = TokenArg->NextToken;
    }

  /* Return a result. In this example it's always NULL, but it could
     be a real result needed by the rule that is calling. */
  return(NULL);
  };



/* <Cmp Exp> ::= <Cmp Exp> '==' <Add Exp> */
char *Rule_CmpExp_EqEq(struct TokenStruct *Token, int Debug, int Indent) {
  /* Arguments: 3 */
  struct TokenStruct *TokenArg;
  char *Value;

  /* This is just an example of how to fetch the arguments. Usually
     you won't need them all. Remember to free() all the results! */
  TokenArg = Token->Reduction->TokenStack;
  while (TokenArg != NULL) {
    Value = Interpret(TokenArg,Debug,Indent);
    if (Value != NULL) free(Value);
    TokenArg = TokenArg->NextToken;
    }

  /* Return a result. In this example it's always NULL, but it could
     be a real result needed by the rule that is calling. */
  return(NULL);
  };



/* <Cmp Exp> ::= <Cmp Exp> '!=' <Add Exp> */
char *Rule_CmpExp_ExclamEq(struct TokenStruct *Token, int Debug, int Indent) {
  /* Arguments: 3 */
  struct TokenStruct *TokenArg;
  char *Value;

  /* This is just an example of how to fetch the arguments. Usually
     you won't need them all. Remember to free() all the results! */
  TokenArg = Token->Reduction->TokenStack;
  while (TokenArg != NULL) {
    Value = Interpret(TokenArg,Debug,Indent);
    if (Value != NULL) free(Value);
    TokenArg = TokenArg->NextToken;
    }

  /* Return a result. In this example it's always NULL, but it could
     be a real result needed by the rule that is calling. */
  return(NULL);
  };



/* <Cmp Exp> ::= <Add Exp> */
char *Rule_CmpExp(struct TokenStruct *Token, int Debug, int Indent) {
  /* Arguments: 1 */
  struct TokenStruct *TokenArg;
  char *Value;

  /* This is just an example of how to fetch the arguments. Usually
     you won't need them all. Remember to free() all the results! */
  TokenArg = Token->Reduction->TokenStack;
  while (TokenArg != NULL) {
    Value = Interpret(TokenArg,Debug,Indent);
    if (Value != NULL) free(Value);
    TokenArg = TokenArg->NextToken;
    }

  /* Return a result. In this example it's always NULL, but it could
     be a real result needed by the rule that is calling. */
  return(NULL);
  };



/* <Add Exp> ::= <Add Exp> '+' <Mult Exp> */
char *Rule_AddExp_Plus(struct TokenStruct *Token, int Debug, int Indent) {
  /* Arguments: 3 */
  struct TokenStruct *TokenArg;
  char *Value;

  /* This is just an example of how to fetch the arguments. Usually
     you won't need them all. Remember to free() all the results! */
  TokenArg = Token->Reduction->TokenStack;
  while (TokenArg != NULL) {
    Value = Interpret(TokenArg,Debug,Indent);
    if (Value != NULL) free(Value);
    TokenArg = TokenArg->NextToken;
    }

  /* Return a result. In this example it's always NULL, but it could
     be a real result needed by the rule that is calling. */
  return(NULL);
  };



/* <Add Exp> ::= <Add Exp> '-' <Mult Exp> */
char *Rule_AddExp_Minus(struct TokenStruct *Token, int Debug, int Indent) {
  /* Arguments: 3 */
  struct TokenStruct *TokenArg;
  char *Value;

  /* This is just an example of how to fetch the arguments. Usually
     you won't need them all. Remember to free() all the results! */
  TokenArg = Token->Reduction->TokenStack;
  while (TokenArg != NULL) {
    Value = Interpret(TokenArg,Debug,Indent);
    if (Value != NULL) free(Value);
    TokenArg = TokenArg->NextToken;
    }

  /* Return a result. In this example it's always NULL, but it could
     be a real result needed by the rule that is calling. */
  return(NULL);
  };



/* <Add Exp> ::= <Mult Exp> */
char *Rule_AddExp(struct TokenStruct *Token, int Debug, int Indent) {
  /* Arguments: 1 */
  struct TokenStruct *TokenArg;
  char *Value;

  /* This is just an example of how to fetch the arguments. Usually
     you won't need them all. Remember to free() all the results! */
  TokenArg = Token->Reduction->TokenStack;
  while (TokenArg != NULL) {
    Value = Interpret(TokenArg,Debug,Indent);
    if (Value != NULL) free(Value);
    TokenArg = TokenArg->NextToken;
    }

  /* Return a result. In this example it's always NULL, but it could
     be a real result needed by the rule that is calling. */
  return(NULL);
  };



/* <Mult Exp> ::= <Mult Exp> '*' <Negate Exp> */
char *Rule_MultExp_Times(struct TokenStruct *Token, int Debug, int Indent) {
  /* Arguments: 3 */
  struct TokenStruct *TokenArg;
  char *Value;

  /* This is just an example of how to fetch the arguments. Usually
     you won't need them all. Remember to free() all the results! */
  TokenArg = Token->Reduction->TokenStack;
  while (TokenArg != NULL) {
    Value = Interpret(TokenArg,Debug,Indent);
    if (Value != NULL) free(Value);
    TokenArg = TokenArg->NextToken;
    }

  /* Return a result. In this example it's always NULL, but it could
     be a real result needed by the rule that is calling. */
  return(NULL);
  };



/* <Mult Exp> ::= <Mult Exp> '/' <Negate Exp> */
char *Rule_MultExp_Div(struct TokenStruct *Token, int Debug, int Indent) {
  /* Arguments: 3 */
  struct TokenStruct *TokenArg;
  char *Value;

  /* This is just an example of how to fetch the arguments. Usually
     you won't need them all. Remember to free() all the results! */
  TokenArg = Token->Reduction->TokenStack;
  while (TokenArg != NULL) {
    Value = Interpret(TokenArg,Debug,Indent);
    if (Value != NULL) free(Value);
    TokenArg = TokenArg->NextToken;
    }

  /* Return a result. In this example it's always NULL, but it could
     be a real result needed by the rule that is calling. */
  return(NULL);
  };



/* <Mult Exp> ::= <Bit Exp> */
char *Rule_MultExp(struct TokenStruct *Token, int Debug, int Indent) {
  /* Arguments: 1 */
  struct TokenStruct *TokenArg;
  char *Value;

  /* This is just an example of how to fetch the arguments. Usually
     you won't need them all. Remember to free() all the results! */
  TokenArg = Token->Reduction->TokenStack;
  while (TokenArg != NULL) {
    Value = Interpret(TokenArg,Debug,Indent);
    if (Value != NULL) free(Value);
    TokenArg = TokenArg->NextToken;
    }

  /* Return a result. In this example it's always NULL, but it could
     be a real result needed by the rule that is calling. */
  return(NULL);
  };



/* <Bit Exp> ::= <Bit Exp> '&' <Negate Exp> */
char *Rule_BitExp_Amp(struct TokenStruct *Token, int Debug, int Indent) {
  /* Arguments: 3 */
  struct TokenStruct *TokenArg;
  char *Value;

  /* This is just an example of how to fetch the arguments. Usually
     you won't need them all. Remember to free() all the results! */
  TokenArg = Token->Reduction->TokenStack;
  while (TokenArg != NULL) {
    Value = Interpret(TokenArg,Debug,Indent);
    if (Value != NULL) free(Value);
    TokenArg = TokenArg->NextToken;
    }

  /* Return a result. In this example it's always NULL, but it could
     be a real result needed by the rule that is calling. */
  return(NULL);
  };



/* <Bit Exp> ::= <Bit Exp> '|' <Negate Exp> */
char *Rule_BitExp_Pipe(struct TokenStruct *Token, int Debug, int Indent) {
  /* Arguments: 3 */
  struct TokenStruct *TokenArg;
  char *Value;

  /* This is just an example of how to fetch the arguments. Usually
     you won't need them all. Remember to free() all the results! */
  TokenArg = Token->Reduction->TokenStack;
  while (TokenArg != NULL) {
    Value = Interpret(TokenArg,Debug,Indent);
    if (Value != NULL) free(Value);
    TokenArg = TokenArg->NextToken;
    }

  /* Return a result. In this example it's always NULL, but it could
     be a real result needed by the rule that is calling. */
  return(NULL);
  };



/* <Bit Exp> ::= <Negate Exp> */
char *Rule_BitExp(struct TokenStruct *Token, int Debug, int Indent) {
  /* Arguments: 1 */
  struct TokenStruct *TokenArg;
  char *Value;

  /* This is just an example of how to fetch the arguments. Usually
     you won't need them all. Remember to free() all the results! */
  TokenArg = Token->Reduction->TokenStack;
  while (TokenArg != NULL) {
    Value = Interpret(TokenArg,Debug,Indent);
    if (Value != NULL) free(Value);
    TokenArg = TokenArg->NextToken;
    }

  /* Return a result. In this example it's always NULL, but it could
     be a real result needed by the rule that is calling. */
  return(NULL);
  };



/* <Negate Exp> ::= '-' <Value> */
char *Rule_NegateExp_Minus(struct TokenStruct *Token, int Debug, int Indent) {
  /* Arguments: 2 */
  struct TokenStruct *TokenArg;
  char *Value;

  /* This is just an example of how to fetch the arguments. Usually
     you won't need them all. Remember to free() all the results! */
  TokenArg = Token->Reduction->TokenStack;
  while (TokenArg != NULL) {
    Value = Interpret(TokenArg,Debug,Indent);
    if (Value != NULL) free(Value);
    TokenArg = TokenArg->NextToken;
    }

  /* Return a result. In this example it's always NULL, but it could
     be a real result needed by the rule that is calling. */
  return(NULL);
  };



/* <Negate Exp> ::= <Value> */
char *Rule_NegateExp(struct TokenStruct *Token, int Debug, int Indent) {
  /* Arguments: 1 */
  struct TokenStruct *TokenArg;
  char *Value;

  /* This is just an example of how to fetch the arguments. Usually
     you won't need them all. Remember to free() all the results! */
  TokenArg = Token->Reduction->TokenStack;
  while (TokenArg != NULL) {
    Value = Interpret(TokenArg,Debug,Indent);
    if (Value != NULL) free(Value);
    TokenArg = TokenArg->NextToken;
    }

  /* Return a result. In this example it's always NULL, but it could
     be a real result needed by the rule that is calling. */
  return(NULL);
  };



/* <Value> ::= Identifier */
char *Rule_Value_Identifier(struct TokenStruct *Token, int Debug, int Indent) {
  /* Arguments: 1 */
  struct TokenStruct *TokenArg;
  char *Value;

  /* This is just an example of how to fetch the arguments. Usually
     you won't need them all. Remember to free() all the results! */
  TokenArg = Token->Reduction->TokenStack;
  while (TokenArg != NULL) {
    Value = Interpret(TokenArg,Debug,Indent);
    if (Value != NULL) free(Value);
    TokenArg = TokenArg->NextToken;
    }

  /* Return a result. In this example it's always NULL, but it could
     be a real result needed by the rule that is calling. */
  return(NULL);
  };



/* <Value> ::= StringLiteral */
char *Rule_Value_StringLiteral(struct TokenStruct *Token, int Debug, int Indent) {
  /* Arguments: 1 */
  struct TokenStruct *TokenArg;
  char *Value;

  /* This is just an example of how to fetch the arguments. Usually
     you won't need them all. Remember to free() all the results! */
  TokenArg = Token->Reduction->TokenStack;
  while (TokenArg != NULL) {
    Value = Interpret(TokenArg,Debug,Indent);
    if (Value != NULL) free(Value);
    TokenArg = TokenArg->NextToken;
    }

  /* Return a result. In this example it's always NULL, but it could
     be a real result needed by the rule that is calling. */
  return(NULL);
  };



/* <Value> ::= DecimalNumber */
char *Rule_Value_DecimalNumber(struct TokenStruct *Token, int Debug, int Indent) {
  /* Arguments: 1 */
  struct TokenStruct *TokenArg;
  char *Value;

  /* This is just an example of how to fetch the arguments. Usually
     you won't need them all. Remember to free() all the results! */
  TokenArg = Token->Reduction->TokenStack;
  while (TokenArg != NULL) {
    Value = Interpret(TokenArg,Debug,Indent);
    if (Value != NULL) free(Value);
    TokenArg = TokenArg->NextToken;
    }

  /* Return a result. In this example it's always NULL, but it could
     be a real result needed by the rule that is calling. */
  return(NULL);
  };



/* <Value> ::= '(' <Cmp Exp> ')' */
char *Rule_Value_LParan_RParan(struct TokenStruct *Token, int Debug, int Indent) {
  /* Arguments: 3 */
  struct TokenStruct *TokenArg;
  char *Value;

  /* This is just an example of how to fetch the arguments. Usually
     you won't need them all. Remember to free() all the results! */
  TokenArg = Token->Reduction->TokenStack;
  while (TokenArg != NULL) {
    Value = Interpret(TokenArg,Debug,Indent);
    if (Value != NULL) free(Value);
    TokenArg = TokenArg->NextToken;
    }

  /* Return a result. In this example it's always NULL, but it could
     be a real result needed by the rule that is calling. */
  return(NULL);
  };



/* <Value> ::= BooleanLiteral */
char *Rule_Value_BooleanLiteral(struct TokenStruct *Token, int Debug, int Indent) {
  /* Arguments: 1 */
  struct TokenStruct *TokenArg;
  char *Value;

  /* This is just an example of how to fetch the arguments. Usually
     you won't need them all. Remember to free() all the results! */
  TokenArg = Token->Reduction->TokenStack;
  while (TokenArg != NULL) {
    Value = Interpret(TokenArg,Debug,Indent);
    if (Value != NULL) free(Value);
    TokenArg = TokenArg->NextToken;
    }

  /* Return a result. In this example it's always NULL, but it could
     be a real result needed by the rule that is calling. */
  return(NULL);
  };



char *(*RuleJumpTable[])(struct TokenStruct *Token, int Debug, int Indent) = {

  /* 0. <Program> ::= <Boolean Exp> */
  Rule_Program,

  /* 1. <Boolean Exp> ::= <Boolean Exp> '||' <Cmp Exp> */
  Rule_BooleanExp_PipePipe,

  /* 2. <Boolean Exp> ::= <Boolean Exp> '&&' <Cmp Exp> */
  Rule_BooleanExp_AmpAmp,

  /* 3. <Boolean Exp> ::= <Cmp Exp> */
  Rule_BooleanExp,

  /* 4. <Cmp Exp> ::= <Cmp Exp> '>' <Add Exp> */
  Rule_CmpExp_Gt,

  /* 5. <Cmp Exp> ::= <Cmp Exp> '<' <Add Exp> */
  Rule_CmpExp_Lt,

  /* 6. <Cmp Exp> ::= <Cmp Exp> '<=' <Add Exp> */
  Rule_CmpExp_LtEq,

  /* 7. <Cmp Exp> ::= <Cmp Exp> '>=' <Add Exp> */
  Rule_CmpExp_GtEq,

  /* 8. <Cmp Exp> ::= <Cmp Exp> '==' <Add Exp> */
  Rule_CmpExp_EqEq,

  /* 9. <Cmp Exp> ::= <Cmp Exp> '!=' <Add Exp> */
  Rule_CmpExp_ExclamEq,

  /* 10. <Cmp Exp> ::= <Add Exp> */
  Rule_CmpExp,

  /* 11. <Add Exp> ::= <Add Exp> '+' <Mult Exp> */
  Rule_AddExp_Plus,

  /* 12. <Add Exp> ::= <Add Exp> '-' <Mult Exp> */
  Rule_AddExp_Minus,

  /* 13. <Add Exp> ::= <Mult Exp> */
  Rule_AddExp,

  /* 14. <Mult Exp> ::= <Mult Exp> '*' <Negate Exp> */
  Rule_MultExp_Times,

  /* 15. <Mult Exp> ::= <Mult Exp> '/' <Negate Exp> */
  Rule_MultExp_Div,

  /* 16. <Mult Exp> ::= <Bit Exp> */
  Rule_MultExp,

  /* 17. <Bit Exp> ::= <Bit Exp> '&' <Negate Exp> */
  Rule_BitExp_Amp,

  /* 18. <Bit Exp> ::= <Bit Exp> '|' <Negate Exp> */
  Rule_BitExp_Pipe,

  /* 19. <Bit Exp> ::= <Negate Exp> */
  Rule_BitExp,

  /* 20. <Negate Exp> ::= '-' <Value> */
  Rule_NegateExp_Minus,

  /* 21. <Negate Exp> ::= <Value> */
  Rule_NegateExp,

  /* 22. <Value> ::= Identifier */
  Rule_Value_Identifier,

  /* 23. <Value> ::= StringLiteral */
  Rule_Value_StringLiteral,

  /* 24. <Value> ::= DecimalNumber */
  Rule_Value_DecimalNumber,

  /* 25. <Value> ::= '(' <Cmp Exp> ')' */
  Rule_Value_LParan_RParan,

  /* 26. <Value> ::= BooleanLiteral */
  Rule_Value_BooleanLiteral 
  };




/* Walk through the parsed input script and interpret (execute, evaluate)
   all the statements. This is the heart of the beast and it's executed a
   great many times. You will want to keep it as light as possible.
   Input is a Token on the TokenStack, output is a string. The Token can
   be:
   - A symbol: return the Data of the Token.
   - A reduction (a Rule with all it's arguments): call the Rule's
     function and return it's result.
   */
char *Interpret(struct TokenStruct *Token, int Debug, int Indent) {
  /* Sanity check. */
  if (Token == NULL) return(NULL);

  /* If it's a symbol then return the value. */
  if (Token->Reduction == NULL) {
    if (Debug > 0) {
      ShowIndent(Indent);
      fprintf(stdout,"%s\n",Token->Data);
      }
    return(strdup(Token->Data));
    }

  /* Sanity check. */
  if (Token->Reduction->TokenStack == NULL) return(NULL);

  /* Call the rule and return the result. */
  if (Debug > 0) {
    ShowIndent(Indent);
    fprintf(stdout,"%s\n",Grammar.RuleArray[Token->Reduction->Rule].Description);
    }
  return(RuleJumpTable[Token->Reduction->Rule](Token,Debug,Indent + 1));
  }





int main(int argc, char *argv[]){
  char *InputBuf;
  struct ParseDataStruct *Data;
  struct TokenStruct *Token;
  short Result;

  /* Load the inputfile into memory. */
  InputBuf = LoadInputFile("input1");
  if (InputBuf == NULL) exit(1);

  /* Initialize the parser. */
  Data = ParserInitialize(InputBuf,strlen(InputBuf));

  /* Run the Parser. */
  Result = Parse(Data,TRIMREDUCTIONS,0);

  /* Show a message about the Parser result. */
  switch(Result) {
    case MESSAGEINTERNALERROR:
      fprintf(stdout,"Error: not enough memory to initialize the parser.\n");
      break;
    case MESSAGELEXICALERROR:
      ShowErrorMessage(Data,"Lexical error");
      break;
    case MESSAGECOMMENTERROR:
      ShowErrorMessage(Data,"Comment error");
      break;
    case MESSAGETOKENERROR:
      ShowErrorMessage(Data,"Tokenizer error");
      break;
    case MESSAGESYNTAXERROR:
      ShowErrorMessage(Data,"Syntax error");
      break;
    case MESSAGEACCEPT:
      /* Call the interpreter for all the tokens on the TokenStack. */
      for (Token = Data->TokenStack; Token != NULL; Token = Token->NextToken) {
        if (Debug > 0) fprintf(stdout,"\nStarting to interpret:\n");
        Interpret(Token,Debug,-1);
        }
      break;
    }

  /* Cleanup. */
  ParserCleanup(Data);
  free(InputBuf);
  }
