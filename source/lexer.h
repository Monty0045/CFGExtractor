/*
    Written by Wyatt Turner for CSE 355 @ Arizona State University
    Spring 2020, Dr. Ben Amor
*/


#include <string>
#include <iostream>
#include <vector>

#pragma once

/*
Input Buffer
    Class handles getting characters from input file
*/
class InputBuffer
{
    std::vector<std::string> inputs = {};   //holds 'ungetted' characters when that character wasn't needed
public:
    InputBuffer();
    ~InputBuffer();
    std::string getChar();          //returns one character (however in form of string)
    void ungetChar(std::string giveBack);   //returns character(s)
    bool isEndOfFile();
};



enum TokenType {ID = 0, COMMA, ARROW, LEFTCURL, RIGHTCURL, LINE, SEMICOLON, ERROR, EPSILON, END_OF_FILE, RULES, TERMINALS};

 //Token which are the different logical segments of input
typedef struct Token
{
    std::string value;
    TokenType type;
    int line_num;

}Token;

/*
Lexer
    Class deals with making sense of input file. Digests input string segments
    into correct Tokens in which the parser can use to check for types and values.

*/
class Lexer
{

    

    struct Token;
    InputBuffer* buffer;
    int line_num = 1;
    std::vector<Token> tokenList;

    void consumeSpace();
    std::string getID();
    bool isSpace();
    bool isAlphaNum();
    bool isArrow();

public:
    
    Lexer();
    ~Lexer();
    Token getToken();
    void  ungetToken(Token token);
   

};