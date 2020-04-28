
#include <iostream>
#include <string>
#include <vector>
#include "lexer.h"

#pragma once


class Grammar
{
    private:

        Lexer* lexer;
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
        void syntax_error(int);
        Token expect_token(TokenType);
        Token peek_token();



    public:

        void parse();
        Grammar();


};