%code requires{

  #include "ast.hpp"

  #include <cassert>

  extern const Expression *g_root; // A way of getting the AST out

  //! This is to fix problems when generating C++
  // We are declaring the functions provided by Flex, so
  // that Bison generated code can call them.
  int yylex(void);
  void yyerror(const char *);
}

// Represents the value associated with any kind of
// AST node.
%union{
  const Expression *expr;
  double number;
  std::string *string;
}

%token T_TIMES T_DIVIDE T_PLUS T_MINUS T_EXPONENT
%token T_LBRACKET T_RBRACKET
%token T_LOG T_EXP T_SQRT
%token T_NUMBER T_VARIABLE

%type <expr> EXPR TERM DEGREE FACTOR 
%type <number> T_NUMBER
%type <string> T_VARIABLE T_LOG T_EXP T_SQRT FUNCTION_NAME

%start ROOT

%%

/* The TODO notes a are just a guide, and are non-exhaustive.
   The expectation is that you do each one, then compile and test.
   Testing should be done using patterns that target the specific
   feature; the testbench is there to make sure that you haven't
   broken anything while you added it.
*/

ROOT : EXPR { g_root = $1; }

/* TODO-3 : Add support for (x+6) and (10-y). You'll need to add production rules, and create an AddOperator or
            SubOperator. */
EXPR : EXPR T_PLUS TERM { $$ = new AddOperator($1, $3); }
      | EXPR T_MINUS TERM { $$ = new SubOperator($1, $3); }
      | TERM                 { $$ = $1; }
      

/* TODO-4 : Add support (x*6) and (z/11). */
TERM : TERM T_TIMES DEGREE { $$ = new MulOperator($1, $3); }
      | TERM T_DIVIDE DEGREE { $$ = new DivOperator($1, $3); }
      | DEGREE {$$ = $1;}

DEGREE : FACTOR T_EXPONENT DEGREE {$$ = new ExpOperator($1, $3);}
      | FACTOR {$$ = $1;}

/* TODO-2 : Add a rule for variable, based on the pattern of number. */
FACTOR : T_LBRACKET EXPR T_RBRACKET { $$ = $2; } // why hast he added a new expression?
        | T_EXP T_LBRACKET EXPR T_RBRACKET {$$ = new ExpFunction($3); }
        | T_LOG T_LBRACKET EXPR T_RBRACKET {$$ = new LogFunction($3); }
        | T_SQRT T_LBRACKET EXPR T_RBRACKET {$$ = new SqrtFunction($3); }
        | T_VARIABLE         {$$ = new Variable (*$1); }
        | T_NUMBER           {  $$ = new Number ($1); }
       

/* TODO-5 : Add support log(x), by modifying the rule for FACTOR. */


/* TODO-6 : Extend support to other functions. Requires modifications here, and to FACTOR. */
FUNCTION_NAME : T_LOG { $$ = new std::string("log"); }
              | T_EXP { $$ = new std::string("exp"); }
              | T_SQRT { $$ = new std::string("sqrt"); }

%%

const Expression *g_root; // Definition of variable (to match declaration earlier)

const Expression *parseAST()
{
  g_root=0;
  yyparse();
  return g_root;
}
