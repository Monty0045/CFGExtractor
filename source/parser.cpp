/*
    Written by Wyatt Turner for CSE355 @ Arizona State University

    This file implements methods and functionality for parsing a grammar.
    
    The Grammar object can be found in the grammer.h file, other functionality
    after having parsed and created necessary data structures for a Grammar are
    implemented in grammar.cpp
*/



#include "grammar.h"

using namespace std;

/*
    If something unexpected occurs during execution, this method is called and program exits
*/
void Grammar::syntax_error(int lineNum = -1)
{
    cout << "Syntax Error";
    if(lineNum != -1) cout << " on line " << lineNum;
    cout << "! Program exiting."<< endl;

    exit(1);
}

/*
    @return Next token in input, does not consume the token
*/
Token Grammar::peek_token()
{
    Token toReturn = lexer->getToken();
    lexer->ungetToken(toReturn);        //adds Token back into input buffer, however copy still available
    return toReturn;
}

/*
    @param Expected type of next token
    @return Token with that type, otherwise syntax error is thrown here
*/
Token Grammar::expect_token(TokenType expectedType)
{
    Token toCheck = lexer->getToken();
    if(toCheck.type == expectedType) return toCheck;

    syntax_error(toCheck.line_num);
}

/*
    Given the input file with a defined CFG, will parse and return the created Grammar object.

*/
void Grammar::parse()
{
    parse_grammar();
}


void Grammar::parse_grammar()
{
    //grammar -> rules

    parse_rules();
}

void Grammar::parse_rules()
{
    //rules -> RuleSection
    //rules -> RULES LEFTCURL ruleList RIGHTCURL TerminalRules
    //Second rule can be rewritten as 'RuleSection TerminalRules' but this is more clear as...
    //  Rule section by itself can be defined with simply ruleList

    parse_ruleSection();

    Token t = peek_token();

    if(t.type == TERMINALS)
    {
        parse_terminalRules();
    }


}

void Grammar::parse_ruleSection()
{
    //ruleSection -> RULES LEFTCURL ruleList RIGHTCURL
    //ruleSection -> ruleList

    Token t = peek_token();

    if(t.type == RULES)
    {
        expect_token(RULES);
        expect_token(LEFTCURL);
        parse_ruleList();
        expect_token(RIGHTCURL);
    }
    else
    {
        parse_ruleList();
    }
    
}

void Grammar::parse_ruleList()
{
    //ruleList -> rule
    //ruleList -> rule ruleList

    parse_rule();

    Token t = peek_token();
    if(t.type == ID)
    {
        parse_ruleList();
    }
    

}

void Grammar::parse_rule()
{
    //rule -> ID ARROW rhsList SEMICOLON
}




