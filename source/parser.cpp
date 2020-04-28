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
    lexer->ungetToken(toReturn);        //adds Token back into token buffer, however copy still available
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
    if(t.type == ID)        //if an ID is seen then safe to assume it is the name of another rule
    {
        parse_ruleList();
    }
    

}

void Grammar::parse_rule()
{
    //rule -> ID ARROW rhsList SEMICOLON
    Token ruleToken = expect_token(ID);
    expect_token(ARROW);
    parse_rhsList();
    expect_token(SEMICOLON);

}

void Grammar::parse_rhsList()
{
    //rhsList -> rhs
    //rhsList -> rhs LINE rhsList
    parse_rhs();

    Token possLine = peek_token();
    if(possLine.type == LINE)
    {
        expect_token(LINE);
        parse_rhsList();
    }
}



void Grammar::parse_rhs()
{
    //rhs -> ID
    //rhs -> ID rhs

    expect_token(ID);           //either the name of a rule or terminal

    Token token = peek_token();
    if(token.type == ID)
    {
        parse_rhs();
    }

}


void Grammar::parse_terminalRules()
{
    //terminalRules -> TERMINALS LEFTCURL terminal_ruleList RIGHTCURL

    expect_token(TERMINALS);
    expect_token(LEFTCURL);
    
    parse_terminal_ruleList();

    expect_token(RIGHTCURL);

}


void Grammar::parse_terminal_ruleList()
{
    //terminal_ruleList -> terminal_rule
    //terminal_ruleList -> terminal_rule terminal_ruleList

    parse_terminal_rule();

    Token token = peek_token();
    if(token.type == ID)
    {
        parse_terminal_ruleList();
    }


}


void Grammar::parse_terminal_rule()
{
    //terminal_rule -> ID ARROW term_rhsList SEMICOLON

    expect_token(ID);
    expect_token(ARROW);

    parse_terminal_rhsList();

    expect_token(SEMICOLON);
}


void Grammar::parse_terminal_rhsList()
{
    //terminal_rhsList -> rhs
    //terminal_rhsList -> rhs LINE terminal_rhsList
    //terminal_rhsList -> rhs COMMA terminal_rhsList

    parse_rhs();

    Token t = peek_token();
    if(t.type == LINE || t.type == COMMA)
    {
        t = lexer.getToken();       //actually consumes the LINE or COMMA
        parse_terminal_rhsList();
    }

}




