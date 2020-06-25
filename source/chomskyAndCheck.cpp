/*
    Written by Wyatt Turner for CSE 355 @ Arizona State University, Spring 2020, Dr. Ben Amor
*/

#include "grammar.h"

using namespace std;

/*
    Converts rules in grammar to follow chomsky normal form
    After calling this the symbols vector will change.
    This form is convient for future operations.
*/
void Grammar::convertToChomsky()
{
    symbols = iniChomsky();


}


vector<Grammar::element*> Grammar::iniChomsky()
{
    vector<element*> newSymbols = {};

    for(int i = 0; i < symbols.size(); i++)
    {
        if(i == 2)
        {
            //create new start symbol
            element* newStart = new element;
            newStart->value = "new" + symbols[2]->value;    //has new infront of old start symbols name
            newStart->rhsList = {{symbols[2]}};
            newStart->isTerminalRule = false;
            newStart->isTerminal = false;

            newSymbols.push_back(newStart); //now newStart is the new start symbol
            newSymbols.push_back(symbols[2]);   //add old start symbol

        }
        else
        {
            newSymbols.push_back(symbols[i]);
        }
        
    }

    return newSymbols;
}

/*
    Helper method to convertChomsky, removes EPSILON from rules and replaces
    occurences where EPSILON occurs in a rule to version where that nonTerminal isn't there
*/
void Grammar::removeEPSILONrhs()
{
    for(int i = 2; i < symbols.size(); i++)
    {
        element* currentElem = symbols[i];

        if(!currentElem->isTerminal)
        {
            bool wasEpPresent = removeEPSILON(currentElem); //removes EPSILON from currentElem

            //now must go through all rules of Grammar and make copies of rhs where currentElem existed to version without
            if(wasEpPresent)
            {
                
            }

        }

    }
}


bool Grammar::removeEPSILON(Grammar::element* currentElem)
{
    vector< vector<element*> > newRHSList = {};
    bool wasEpPresent = false;

    for(int i = 0; i < currentElem->rhsList.size(); i++)    //iterates through RHSs of currentElem
    {
        if(currentElem->rhsList[i][0] == symbols[1]) //if current RHS is EPSILON
        {
            wasEpPresent = true;
        }
        else
        {
            newRHSList.push_back(currentElem->rhsList[i]);
        }
        
    }

    return wasEpPresent;
}

/*

*/
void Grammar::epCopier(element* wherePresent)
{

}

void Grammar::removeUnitRules()
{

}

void Grammar::convertRulesCNF()
{

}