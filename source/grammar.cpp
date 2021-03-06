/*
    Written by Wyatt Turner for CSE 355 @ Arizona State University, Spring 2020, Dr. Ben Amor

*/
#include "grammar.h"
#include "stdlib.h"
#include "time.h"

using namespace std;

Grammar::Grammar()
{
    lexer = new Lexer();

    //Manually fills Grammar with $ and epsilon symbol
        //Note: hard coded so $ is 0th index and EPSILON is 1st index.
    element* dollar = new element;
    dollar->value = "$";
    dollar->isTerminal = true;
    dollar->rhsList = {};
    this->symbols.push_back(dollar);

    element* epsilon = new element;
    epsilon->value = "EPSILON";
    epsilon->isTerminal = true;
    epsilon->rhsList = {};
    this->symbols.push_back(epsilon);
}

/*
    @param index of element within grammar's symbols
    @return pointer to the element, if no element with given index returns NULL
*/
//TODO: Assuming my refactoring works this function is obsolete
Grammar::element* Grammar::getElement(int index)
{
    element* toReturn = NULL;       //Null if not within symbols

    if(index >= 0 && index <= symbols.size()-1)
    {
        toReturn = symbols[index];
    }

    return toReturn;
}

/*
    Generates string recognized by CFG
*/
void Grammar::generateString()
{
    cout <<endl;

    resolveNonTerminal(symbols[2]);    //calls helper function beginning at start symbol.

    cout << endl;

}

/*
    Helper function for generateString(), given a nonTerminal element will resolve and print the corresponding
    sequence of terminals.
    @param nonTerminal to print terminals of

*/
void Grammar::resolveNonTerminal(element* nonTerminal)
{

    //chooses pseudo random rhs rule application of nonTerminal
    int numOfRHSs = nonTerminal->rhsList.size();
    int indexOfRHS = rand() % numOfRHSs + 1;
    int index = indexOfRHS - 1;                 //had to shift this way otherwise previous line would have div by 0
    vector<element*> rhs = nonTerminal->rhsList[index];

    for(int i = 0; i < rhs.size(); i++)
    {
        element* currentElem = rhs[i];
        if(currentElem->isTerminal)     //if current element on rhs is a terminal
        {

            if(currentElem->value == "EPSILON")
            {
                break;  //breaks RHS, epsilon is optional so move on.
            }

            cout << " " << currentElem->value << " ";
        }
        else    //another nonTerminal present
        {
            resolveNonTerminal(currentElem);
        }
        
    }
}


/*
    This method is a helper function for some logic such as getting generating symbols
    @param element to get index in grammar's symbols
    @return index of input element in grammar's symbol
*/
int Grammar::getElemIndex(Grammar::element* element)
{
    int index = -1;

    for(int i = 0; i < symbols.size(); i++)
    {
        if(symbols[i] == element)
        {
            index = i;
            break;
        }
    }

    return index;
}

/*
    Prints the symbols in Grammar. First prints the rules then the terminals
*/
void Grammar::printSymbols()
{
    //Rule loop
    cout << "NonTerminals within Grammar: \n";
    for(int i = 2; i < symbols.size(); i++)
    {
        element* currentElem = symbols[i];

        if(!currentElem->isTerminal)
        {
            cout << " " << currentElem->value << endl;
        }
    }

    //Terminal loop
    cout << endl;
    cout << "Terminals within Grammar: \n";
    for(int i = 2; i < symbols.size(); i++)
    {
        element* currentElem = symbols[i];

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

    for(int i = 2; i < symbols.size(); i++)
    {
        element* currentElem = symbols[i];

        if(!currentElem->isTerminal)        //if current element is a nonterminal
        {
            cout << currentElem->value << " -> ";
            printRHS(&(currentElem->rhsList));
            cout << endl;
        }
    }

}

/*
    Given the RHS of a rule it will output all the different RHS's seperated by | character
    @param address of RHS of a nonterminal which is stored as a 2-dimensional integer vector
*/
void Grammar::printRHS(vector< vector<element*>> * rhsList)
{
    //iterates through list of different RHS's
    for(int i = (*rhsList).size() - 1; i >= 0; i--) //goes in reverse because rhs's added in reverse
    {
        if(i < ((*rhsList).size()-1)) cout << " |";                         //if multiple rhs options

        vector<element*> currentRHS = (*rhsList)[i];
        for(int j = 0; j < currentRHS.size(); j++)      //iterates through elements appearing on current rhs
        {
            element* elemOnRHS = currentRHS[j];
            //element* elemOnRHS = getElement(currentRHS[j]);

            cout << " " << elemOnRHS->value; 
        }

    }
}



/*
    ---------------------------------------------------------

    Logic dealing with removing useless symbols from grammar

    ---------------------------------------------------------

*/

/*
    Will update symbols to contain only useful symbols in grammar.
    If none present, program will exit.
*/
void Grammar::removeUselessSyms()
{
    getGenSymbols();

    getReachableSyms();
}

/*
    Removes Useless symbols in grammar. Useless symbols are symbols that either never end or are unreachable.

*/
vector<bool> Grammar::iniGenSyms()
{
    vector<bool> genArray(symbols.size());

    genArray[0] = false;        //$ used later, will be ignored but not considered generating.
    genArray[1] = true;        //EPSILON, which is generating since it is a terminal
    for(int i = 2; i < symbols.size(); i++)
    {
        if(symbols[i]->isTerminal)
        {
            genArray[i] = true;
        }
        else genArray[i] = false;
    }

    return genArray;
}

void Grammar::getGenSymbols()
{
    bool change = false;

    vector<bool> genArray = iniGenSyms();

    //loops through grammar, for symbol to be generating only 1 rhs must be generating
    //  however ALL elements on that right hand side must be generating.
    do {

        change = false;

        for(int i = 2; i < symbols.size(); i++)
        {
            element* currentElem = symbols[i];
            if(!currentElem->isTerminal)        //if current element is a nonTerminal
            {
                for(int j = 0; j < currentElem->rhsList.size(); j++)    //iterates over the different RHS
                {
                    vector<element*> rhs = currentElem->rhsList[j];

                    if(isRHSGen(genArray, rhs))  //if rhs is generating then whole symbol is
                    {
                        if(genArray[i] == false)    //if current symbol is set as nonGen
                        {
                            change = true;
                            genArray[i] = true;
                        }
                    }
                }
            }
        }
    }while(change);

    //Removes nongenerating RHS's from grammar
    //  since I group rules together with their different RHS, some RHSs may be nongenerating
    //  but LHS still is generating.
    removeNonGenRHS(genArray);


    //Removes nongenerating LHS's (nonterminals) from grammar
    removeNonGenSym(genArray);

}


/*
    Helper method for getGenSymbols
    @param genArray is a bool vector corresponding to whether symbols of Grammar are generating, rhs is the right hand side of a rule
    @return whether a given rhs of a nonTerminal is generating
*/
bool Grammar::isRHSGen(vector<bool> genArray, 
                       vector<Grammar::element*> rhs){
    bool isGen = true;

    for(int i = 0; i < rhs.size(); i++)
    {
        int index = getElemIndex(rhs[i]);
        if(!genArray[index])               //if current element in rhs is NOT generating
        {
            isGen = false;
        }
    }

    return isGen;

}

/*
    Helper method to getGenSymbols, this should only be called after generating nonTerminals have been determined

    @param vector of generating symbols that was calculated in getGenSymbols()

*/
void Grammar::removeNonGenRHS(vector<bool> genArray)
{
    for(int i = 2; i < symbols.size(); i++)    //iterates through symbols
    {
        element* currentElem = symbols[i];
        if(!currentElem->isTerminal)        //nonTerminal
        {
            vector< vector<element*>> newRHSList = {};
            for(int j = 0; j < currentElem->rhsList.size(); j++) //iterates through RHS
            {
                vector<element*> rhs = currentElem->rhsList[j];
                if(isRHSGen(genArray, rhs))   //if RHS of nonTerminal is generating
                {
                    
                    //currentElem->rhsList.erase( currentElem->rhsList.begin() + j);  //removes the nongenerating rhs from currentElem
                    newRHSList.push_back(rhs);
                }
            }
            currentElem->rhsList = newRHSList;
        }
    }
}

/*
    Helper method of getGenSymbols(), removes non generating LHS (Nonterminals) from symbols
    @param vector of generating symbol values as calculated in getGenSymbols()
*/  
void Grammar::removeNonGenSym(vector<bool> genArray)
{

    //instead of actual deletion of current universe elements, will just change universe

    vector<element*> newSymbols = {};
    newSymbols.push_back(symbols[0]);
    newSymbols.push_back(symbols[1]);

    for(int i = 2; i < symbols.size(); i++)
    {
        element* currentElem = symbols[i];
        if(!currentElem->isTerminal && !genArray[i])    //if currentElem is both nonTerminal and nonGenerating
        {
            delete currentElem;

        }
        else
        {
            newSymbols.push_back(symbols[i]);
        }
        
    }

    symbols = newSymbols;
}

/*
    Used to get only usefule symbols in grammar. Requires that getGenSymbols() has already ran. After
    running only symbols which can be reached from start variable (first nonterminal) will be present in
    grammar.
*/
void Grammar::getReachableSyms()
{   
    vector<bool> reachable = iniReachableSyms();    //initial boolean vector corresponding to reachable symbols.

    bool change = false;

    do{
        change = false;
        for(int i = 2; i < symbols.size(); i++)
        {
            element* currentElem = symbols[i];

            if(!currentElem->isTerminal && reachable[i])    //if currentElem is a nonTerminal and is reachable
            {
                for(int j = 0; j < currentElem->rhsList.size(); j++)
                {
                    vector<element*> rhs = currentElem->rhsList[j];

                    if(unreachPresentInRHS(&reachable, rhs))
                    {
                        change = true;
                    }
                }
            }
        }

    }while(change);

    removeUnreachSyms(reachable);

}

/*
    Helper function for getReachableSyms()
    @return bool vector corresponding to symbols of whether they are reachable (only start variable)
*/
vector<bool> Grammar::iniReachableSyms()
{
    vector<bool> reachable(symbols.size());

    if(symbols.size() > 2)
    { 
        reachable[2] = true;
        for(int i = 0; i < reachable.size(); i++)
        {
            if(i == 2)
            {
                reachable[2] = true;
            }
            else
            {
                reachable[i] = false;
            }
            
        }
    }
    else
    {
        cout << "No longer any symbols even in the grammar. Nothing really interesting can happen now.\n";
        cout << "Exiting the program" << endl;
        exit(1);
    }

    return reachable;
    
}

/*
    Helper function of getReachableSyms(), called on the rhs of a reachable rule. If a nonreachable nonTerminal is present updates
    the value of that symbol in reachable vector. NOTE: reachable is being updating outside of this scope
    @param vector of bools correpsonding to symbols reachability
    @param the rhs of a reachable rule
    @return whether a nonreachable symbol has been updated in rhs to reachable
*/
bool Grammar::unreachPresentInRHS(vector<bool> * reachable, 
                                  vector<element*> rhs)
{

    bool changed = false;

    for(int i = 0; i < rhs.size(); i++)
    {
        element* currentElem = rhs[i];
        int indexInUni = getElemIndex(currentElem);
        //TODO: Do I need to do terminals as well?
        //if(!currentElem->isTerminal && !(*reachable)[indexInUni])  //if element is nonTerminal and not reachable 
        if(!(*reachable)[indexInUni])
        {
            (*reachable)[indexInUni] = true;
            changed = true;
        }
    }

    return changed;
}


/*
    Helper function of getReachableSyms() called after reachable symbols have been calculated.
    Updates symbols to only include reachable nonTerminals.
*/
void Grammar::removeUnreachSyms(vector<bool> reachable)
{
    vector<element*> newSymbols;
    newSymbols.push_back(symbols[0]);
    newSymbols.push_back(symbols[1]);

    for(int i = 2; i < symbols.size(); i++)
    {
        element* currentElem = symbols[i];
        if(reachable[i])
        {
            newSymbols.push_back(currentElem);
        }
        else
        {
            delete currentElem;
        }
        
    }

    symbols = newSymbols;
}


/*
    ------------------------------------------



    ------------------------------------------
*/
