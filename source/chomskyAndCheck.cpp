
#include "grammar.h"

using namespace std;

/*
    Converts rules in grammar to follow chomsky normal form
    After calling this the symbols vector will change.
    This form is convient for future operations.
*/
void Grammar::convertToChomsky()
{
    //vector<element*> newSymbols = iniChomsky();
    symbols = iniChomsky();


}


vector<element*> Grammar::iniChomsky()
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

}


void Grammar::removeUnitRules()
{

}

void Grammar::convertRulesCNF()
{
    
}