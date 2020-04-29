#include "grammar.h"



Grammar::Grammar()
{
    lexer = new Lexer();
}

/*
    @param index of element within grammar's universe
    @return pointer to the element, if no element with given index returns NULL
*/
Grammar::element* Grammar::getElement(int index)
{
    element* toReturn = NULL;       //Null if not within universe

    if(index >= 0 && index <= universe.size()-1)
    {
        toReturn = universe[index];
    }

    return toReturn;
}




