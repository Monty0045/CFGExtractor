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

/*
    Prints the rules of a grammar along with their RHS
*/
void Grammar::printRules()
{

    for(int i = 2; i < universe.size(); i++)
    {
        element* currentElem = universe[i];

        if(!currentElem->isTerminal)        //if current element is a nonterminal
        {
            cout << currentElem->value << " -> ";
            printRHS(&(currentElem->rhs));
            cout << endl;
        }
    }

}

/*
    Given the RHS of a rule it will output all the different RHS's seperated by | character
    @param address of RHS of a nonterminal which is stored as a 2-dimensional integer vector
*/
void Grammar::printRHS(vector< vector<int>> * rhsList)
{
    //iterates through list of different RHS's
    for(int i = (*rhsList).size() - 1; i >= 0; i--) //goes in reverse because rhs's added in reverse
    {
        if(i < ((*rhsList).size()-1)) cout << " |";                         //if multiple rhs options

        vector<int> currentRHS = (*rhsList)[i];
        for(int j = 0; j < currentRHS.size(); j++)      //iterates through elements appearing on current rhs
        {
            element* elemOnRHS = getElement(currentRHS[j]);

            cout << " " << elemOnRHS->value; 
        }

    }
}


