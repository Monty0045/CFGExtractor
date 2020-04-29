
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
            std::string value; 
            bool isTerminal = true;
            bool isTerminalRule =  false;                  //used for some semantic checking
            std::vector< std::vector <int> > rhs;        //list of right hand sides, each rhs will be indexes of elements in Grammar's universe
        };

        std::vector<element*> universe;              //every terminal/rule element is stored here (including epsilon and $)
        bool isAmbigious;
        bool predictiveParser;

        Lexer* lexer;

        //std::vector<Token>;

        /*
            parser methods, implemented in parser.cpp
        */
        void parse_grammar();
        void parse_rules();
        void parse_ruleSection();
        void parse_ruleList();
        void parse_rule();
        void parse_rhsList();
        std::vector<int> parse_rhs();
        std::vector<int>* parse_rhs(std::vector<int>*); //overloaded to handle recursive case easier
        void parse_terminalRules();
        void parse_terminal_ruleList();
        void parse_terminal_rule();
        std::vector< std::vector<int> > parse_terminal_rhsList();
        std::vector<int> parse_terminal_rhs();
        void syntax_error(int);
        Token expect_token(TokenType);
        Token peek_token();
        int elementLookup(std::string);


        /*
            
        */
        element* getElement(int);


    public:

        void parse();
        Grammar();


};