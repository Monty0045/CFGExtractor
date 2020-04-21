
#include <string>
#include <iostream>
#include <vector>


/*
Lexer
    Class deals with making sense of input file. Digests input string segments
    into correct Tokens in which the parser can use to check for types and values.

*/
class Lexer
{

    enum TokenType = {ID, COMMA, LEFTCURL, RIGHTCURL, LINE, SEMICOLON, ERROR, EPSILON};


    /*
    Input Buffer
        Class handles getting characters from input file
    */
    class InputBuffer
    {
        std::vector<int> inputs = {};   //holds 'ungetted' characters when that character wasn't needed

    public:
        inputBuffer();
        std::string getChar();          //returns one character (however in form of string)
        void ungetChar(std::string giveBack);   //returns character(s)
    };



    void consumeSpace();

public:

    //Token which are the different logical segments of input
    typedef struct Token
    {
        std::string value;
        TokenType type;
        int line_num;

    };
    
    Lexer();
    Token getToken();
    void  returnToken(Token token);

};