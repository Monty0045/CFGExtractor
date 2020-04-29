
#include <iostream>
#include <string>
#include <vector>
#include "lexer.h"

#pragma once


class Grammar
{

    private:


       //Element which is either a rule or Terminal of the grammar.
        struct element
        {
            string value; 
            bool isTerminal = true;
            vector< vector <int> > rhs;        //list of right hand sides, each rhs will be indexes of elements in Grammar's universe
        };

        vector<element*> universe;              //every terminal/rule element is stored here (including epsilon and $)
        bool isAmbigious;
        bool predictiveParser;

        Lexer* lexer;

        vector<Token>

        /*
            parser methods, implemented in parser.cpp
        */
        void parse_grammar();
        void parse_rules();
        void parse_ruleSection();
        void parse_ruleList();
        void parse_rule();
        void parse_rhsList();
        void parse_rhs();
        void parse_terminalRules();
        void parse_terminal_ruleList();
        void parse_terminal_rule();
        void parse_terminal_rhsList();
        void parse_terminal_rhs();
        void syntax_error(int);
        Token expect_token(TokenType);
        Token peek_token();
        int elementLookup(std::string);


    public:

        void parse();
        Grammar();


};