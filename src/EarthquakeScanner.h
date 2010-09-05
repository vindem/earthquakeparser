#pragma once

// Only include FlexLexer.h if it hasn't been already included
#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

// Override the interface for yylex since we namespaced it
#undef YY_DECL
#define YY_DECL int Earthquake::FlexScanner::yylex()

// Include Bison for types / tokens
#include "normeantisismiche.tab.hh"
#include <FlexLexer.h>
#include <iostream>
using namespace std;
namespace Earthquake {
	class FlexScanner : public yyFlexLexer {
		public:
			FlexScanner();
			FlexScanner(istream* arg_yyin, ostream* arg_yyout);
			~FlexScanner();
			// save the pointer to yylval so we can change it, and invoke scanner
			int yylex(Earthquake::BisonParser::semantic_type * lval) {
				yylval = lval;
				return yylex();
			}
		
		private:
			// Scanning function created by Flex; make this private to force usage
			// of the overloaded method so we can get a pointer to Bison's yylval
			int yylex();
			
			// point to yylval (provided by Bison in overloaded yylex)
			Earthquake::BisonParser::semantic_type * yylval;
	};

};
