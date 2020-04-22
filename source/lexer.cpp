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
    char tempChar;

    if(inputs.size() == 0)    //if input buffer empty
    {
        cin.get(tempChar);  //puts character from input stream into tempChar
        string toReturn = "" + tempChar;
        return toReturn;
    }
    else    //characters present in input buffer
    {
       string toReturn = inputs[inputs.size()-1]; //gets last element
       inputs.pop_back();                         //decreases vector by 1
       return toReturn; 
    }
    
}

/*
    @param given a character will add that character back to input buffer
*/
void InputBuffer::ungetChar(std::string giveBack)
{
    inputs.push_back(giveBack);
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
    string valuesFromInput;

    //Perhaps call consumeSpace here?

}