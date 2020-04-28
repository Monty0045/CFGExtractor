/*
    Written by Wyatt Turner for CSE 355 @ Arizona State University
    Spring 2020, Dr. Ben Amor
*/

#include "lexer.h"


using namespace std;



//INPUTBUFFER IMPLEMENTATION


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


/*
    @return is next character as a string from input

*/
std::string InputBuffer::getChar()
{
    string toReturn = "";

    if(inputs.size() == 0)    //if input buffer empty
    {
        if(cin.eof())
        {
        
            return "EOF";
        }

        char tempChar;
        cin.get(tempChar);  //puts character from input stream into tempChar

        if( int(tempChar) == 10 || int(tempChar) == 0) //if new line character, or null charcter (FIGURE OUT WHY THIS HAPPENS)
        {
            return "\n";
        }

    
        toReturn = toReturn + tempChar;

    }
    else    //characters present in input buffer
    {
       toReturn = inputs[inputs.size()-1]; //gets last element
       
       inputs.pop_back();                  //decreases vector by 1
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
    if(inputs.size() > 0)
    {
        if(inputs[inputs.size() - 1] == "EOF") return true;
        return false;
    }
    else
    {
        return cin.eof();
    }
    
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
    2return true if next two symbols in input buffer form an Arrow ( -> )
*/
bool Lexer::isArrow()
{
    bool isArrow = false;

    std::string possibleDash = buffer->getChar();
    if(possibleDash == "-")
    {
        std::string possibleGreaterThan = buffer->getChar();
        if(possibleGreaterThan == ">") isArrow = true;
        buffer->ungetChar(possibleGreaterThan);
    }
    buffer->ungetChar(possibleDash);

    return isArrow;
}


void Lexer::consumeSpace()
{
    std::string inputChar = buffer->getChar();

    while(inputChar == " " || inputChar == "\n")
    {
        if(inputChar == "\n")
        {
            line_num++;
        }

        inputChar = buffer->getChar(); //consume next space
    }

    buffer->ungetChar(inputChar);
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


    consumeSpace();

    //if endOfFile is reached then return EOF token.
    if(buffer->isEndOfFile())
    {
        toReturn.line_num = line_num;
        toReturn.type = END_OF_FILE;
        toReturn.value = "EOF";
        return toReturn;
    }

    //if still tokens that were given back then return most recent
    if(tokenList.size() > 0)
    {
        toReturn = tokenList[tokenList.size() - 1];
        tokenList.pop_back();
        return toReturn;
    }


    if(isAlphaNum()) //Incase of ID (Rules/Terminals defined in grammer) or EPSILON
    {

        toReturn.value = getID();
        toReturn.type = ID;
        toReturn.line_num = line_num;


        //This handles the three reserved words defined in program
        if(toReturn.value == "EPSILON")
        {
            toReturn.type = EPSILON;
        }
        else if(toReturn.value == "Rules")
        {
            toReturn.type = RULES;
        }
        else if(toReturn.value == "Terminals")
        {
            toReturn.value = TERMINALS;
        }
    }
    else if(isArrow())
    {
        std::string inputChars = buffer->getChar();
        inputChars = inputChars + buffer->getChar();

        toReturn.value = inputChars;
        toReturn.type = ARROW;
        toReturn.line_num = line_num;
    }
    else
    {
        std::string inputChar = buffer->getChar();
        toReturn.line_num = line_num;
        toReturn.value = inputChar;


        if(inputChar == ",") toReturn.type = COMMA;
        else if(inputChar == "{") toReturn.type = LEFTCURL;
        else if(inputChar == "}") toReturn.type = RIGHTCURL;
        else if(inputChar == "|") toReturn.type = LINE;
        else if(inputChar == ";") toReturn.type = SEMICOLON;
        else if(inputChar == "EOF") toReturn.type = END_OF_FILE;
        else toReturn.type = ERROR; //unrecognized symbol
        
    }
    
    return toReturn;

}

/*
    Given a token, will add it back into the stack of 'unget' tokens which can be grabbed
    again later.

    @param Token to return to lexer until needed later.

*/
void Lexer::ungetToken(Token token)
{
    tokenList.push_back(token);
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