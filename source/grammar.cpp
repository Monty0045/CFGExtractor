#include "grammar.h"

using namespace std;

Grammar::Grammar()
{
    lexer = new Lexer();

    //Manually fills Grammar with $ and epsilon symbol
        //Note: hard coded so $ is 0th index and EPSILON is 1st index.
    element* dollar = new element;
    dollar->value = "$";
    dollar->isTerminal = true;
    dollar->rhs = {};
    this->universe.push_back(dollar);

    element* epsilon = new element;
    epsilon->value = "EPSILON";
    epsilon->isTerminal = true;
    epsilon->rhs = {};
    this->universe.push_back(epsilon);
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

/*
    Prints the symbols in the Grammar's universe. First prints the rules then the terminals
*/
void Grammar::printSymbols()
{
    //Rule loop
    cout << "NonTerminals within Grammar: \n";
    for(int i = 2; i < universe.size(); i++)
    {
        element* currentElem = universe[i];

        if(!currentElem->isTerminal)
        {
            cout << " " << currentElem->value << endl;
        }
    }

    //Terminal loop
    cout << endl;
    cout << "Terminals within Grammar: \n";
    for(int i = 2; i < universe.size(); i++)
    {
        element* currentElem = universe[i];

        if(currentElem->isTerminal)
        {
            cout << " " << currentElem->value << endl;
        }
    }

}




