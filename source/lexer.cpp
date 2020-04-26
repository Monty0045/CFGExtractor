/*
    Written by Wyatt Turner for CSE 355 @ Arizona State University
    Spring 2020, Dr. Ben Amor
*/

#include "lexer.h"


using namespace std;



//INPUTBUFFER IMPLEMENTATION


/*
    @return is next character as a string from input

*/
std::string InputBuffer::getChar()
{
    string toReturn;

    if(inputs.size() == 0)    //if input buffer empty
    {
        char tempChar;
        cin.get(tempChar);  //puts character from input stream into tempChar
        toReturn = "" + tempChar;

        if(tempChar == EOF)
        {
            toReturn = "EOF";
        }
    }
    else    //characters present in input buffer
    {
       toReturn = inputs[inputs.size()-1]; //gets last element
       inputs.pop_back();                         //decreases vector by 1
    }
    
    return toReturn;
}

/*
    @param given a character will add that character back to input buffer
*/
void InputBuffer::ungetChar(std::string giveBack)
{
    inputs.push_back(giveBack);
}

bool InputBuffer::isEndOfFile()
{
    if(buffer.size() > 0)
    {
        return false;
    }
    else
    {
        return cin.eof();
    }
    
}

/*
    Constructor
*/
InputBuffer::InputBuffer()
{

}

/*
    Destructor
*/
InputBuffer::~InputBuffer()
{

}







//LEXER IMPLEMENTATION


Lexer::Lexer()
{
    buffer = new InputBuffer();
}


Lexer::~Lexer()
{
    delete buffer;
}


/*
    Reads character from input buffer and checks if it is an alpha-numerical value (for IDs and Epsilon)
    @return true if alphanumerical, false if not
*/
bool Lexer::isAlphaNum()
{

    if(buffer->isEndOfFile())
    {
        return false;
    }

    string current = buffer->getChar();

    bool lowerCaseLetter = (current >= "a" && current <= "z");
    bool upperCaseLetter = (current >= "A" && current <= "Z");
    bool digit = (current >= "0" && current <= "9");

    if(lowerCaseLetter || upperCaseLetter || digit)
    {
        buffer->ungetChar(current);
        return true;
    }
    else
    {
        buffer->ungetChar(current);
        return false;
    }
    
}



/*
    This method grabs the next Token from an input, if unrecognized as any defined
    Token this method will call call an error and exit the program.

    @return Token from the input file denoting a sort of logical 'segment'

*/
Lexer::Token Lexer::getToken()
{
    //string valuesFromInput;
    Token toReturn;

    //if still tokens that were given back then return most recent
    if(tokenList.size > 0)
    {
        toReturn = tokenList[tokenList.size() - 1];
        tokenList.pop_back();
        return toReturn;
    }

    //if endOfFile is reached then return that token.
    if(buffer->isEndOfFile())
    {
        toReturn.line_num = line_num;
        toReturn.type = END_OF_FILE;
        toReturn.value = "";
    }

    
    //Perhaps call consumeSpace here?
    consumeSpace();

    string valueofToken;

    if(isAlphaNum()) 
    {
        //int line_num = cin.
        toReturn.value = getId();
        toReturn.type = ID;
        toReturn.line_num;
    }
    else
    {
        std::string inputChar = buffer->getChar();
        switch(inputChar)
        {
            case ",":
                toReturn.value = ",";
                toReturn.type = COMMA;
                toReturn.line_num = line_num;
                break;
            case "{":
                toReturn.value = "{";
                toReturn.type = LEFTCURL;
                toReturn.line_num = line_num;
                break;
            case "{":
                toReturn.value = "{";
                toReturn.type = LEFTCURL;
                toReturn.line_num = line_num;
                break;    
        }
        
    }
    
    

}

/*
    Called when input is determined to be an ID and reads input until the end of
    that ID.

    @return ID value as a string
*/
std::string Lexer::getID()
{

    std::string idValue = "";

    while(isAlphaNum())
    {
        idValue = idValue + buffer->getChar();
    }
    
    return idValue;    
}