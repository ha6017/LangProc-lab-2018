%option noyywrap

%{
/* Now in a section of C that will be embedded
   into the auto-generated code. Flex will not
   try to interpret code surrounded by %{ ... %} */

/* Bring in our declarations for token types and
   the yylval variable. */
#include "histogram.hpp"
#include <string>
#include <sstream>


// This is to work around an irritating bug in Flex
// https://stackoverflow.com/questions/46213840/get-rid-of-warning-implicit-declaration-of-function-fileno-in-flex
extern "C" int fileno(FILE *stream);

/* End the embedded code section. */
%}


%%

-?[0-9]+(\.[0-9]*)?          { fprintf(stderr, "Number : %s\n", yytext);
                              /* TODO: get value out of yytext and into yylval.numberValue */;
                              yylval.numberValue = std::stod(yytext);
                              return Number;
                            }

-?[0-9]+\/[0-9]+            {
                              fprintf(stderr, "Number : %s\n", yytext);
                              std::istringstream iss(yytext);

                              std::string numerator, denominator;
                              std::getline(iss, numerator, '/');
                              std::getline(iss, denominator);

                              double temp = std::stod(numerator);
                              double temp2 = std::stod(denominator);

                              yylval.numberValue= temp/temp2;
                              return Number;

                            }

\[[^\n\]]*\]                {  fprintf(stderr, "Word : %s\n", yytext);
                                /* TODO: get value out of yytext and into yylval.wordValue */;
                                std::string temp = yytext;
                                temp.erase(0,1);
                                temp.erase(temp.size()-1);
                                yylval.wordValue = new std::string(temp);
                                return Word;
                            }


[a-zA-Z]+          {  fprintf(stderr, "Word : %s\n", yytext);
                      /* TODO: get value out of yytext and into yylval.wordValue */;
                      yylval.wordValue = new std::string(yytext);
                      return Word;
                   }

\n              { fprintf(stderr, "Newline\n"); }

. {}

%%

/* Error handler. This will get called if none of the rules match. */
void yyerror (char const *s)
{
  fprintf (stderr, "Flex Error: %s\n", s); /* s is the text that wasn't matched */
  exit(1);
}
