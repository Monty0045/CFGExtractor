/*
    Written by Wyatt Turner for CSE355 @ Arizona State University

    This file implements methods and functionality for parsing a grammar.
    
    The Grammar object can be found in the grammer.h file, other functionality
    after having parsed and created necessary data structures for a Grammar are
    implemented in grammar.cpp
*/



#include "grammar.h"

using namespace std;

/*
    If something unexpected occurs during execution, this method is called and program exits
*/
void Grammar::syntax_error(int lineNum = -1)
{
    cout << "Syntax Error";
    if(lineNum != -1) cout << " on line " << lineNum;
    cout << "! Program exiting."<< endl;

    exit(1);
}

/*
    @return Next token in input, does not consume the token
*/
Token Grammar::peek_token()
{
    Token toReturn = lexer->getToken();
    lexer->ungetToken(toReturn);        //adds Token back into token buffer, however copy still available
    return toReturn;
}

/*
    @param Expected type of next token
    @return Token with that type, otherwise syntax error is thrown and program exits
*/
Token Grammar::expect_token(TokenType expectedType)
{
    Token toCheck = lexer->getToken();

    if(toCheck.type == expectedType) return toCheck;

    //cout << "This makes no sense..." << endl;
    //cout << toCheck.value << " " << toCheck.type << " "<< expectedType << endl;
    syntax_error(toCheck.line_num);
}


/*
    @param name of ID which is either a rule or terminal in grammar
    @return index of that element in grammar's universe

*/
Grammar::element* Grammar::elementLookup(std::string name)
{
    element* toReturn;
    bool isFound = false;
    //int index;

    //first 2 elements of grammar are automatically epsilon and $
    for(int i = 2; i < universe.size(); i++)
    {

        if(universe[i]->value == name)
        {
            isFound = true;
            toReturn = universe[i];
            break;
        }

    }

    //if not found we will need to create a new element.
    if(isFound == false)
    {
        toReturn = new element;
        toReturn->value = name;
        toReturn->isTerminal = true;  //by default new elements are terminals
        toReturn->rhsList = {};
        universe.push_back(toReturn);
        //index = universe.size() - 1;    //index of new element
    }

    return toReturn;
}


/*
    Used when getting the RHS of a rule and applying it to said rule. Since nonterminal can appear
    multiple times simply setting rhs is not enough and must be appended to present rhs.
    @param address of element's rhs, address of new rhs to be appended

*/
void Grammar::combineRHS(vector< vector<Grammar::element*>>* original, 
                         vector< vector<Grammar::element*>>* newRHS)
{

    //okay so I lied there is going to be a copy having to be generated in order
    //for the ordering of rules to be the same as what the user inputs            

    for(int i = 0; i < (*original).size(); i++)
    {
        //Since we want everything in reverse we actually add the original to the later new rhs
        (*newRHS).push_back( (*original)[i] );

    }

    (*original) = (*newRHS); //sets element's original rhs to equal this new one with everything
}



/*
    After parsing program this is called to ensure no nonTerminals are on RHS of terminal rules list

*/
void Grammar::terminalCheck()
{
    for(int i = 2; i < universe.size(); i++)
    {
        if(universe[i]->isTerminalRule)
        {
            terminalRHSCheck(&(universe[i]->rhsList));  //passes in the rhsList of current Terminal rule
        }
    }
}

/*
    Helper method for terminalCheck()
    Makes sure all items on RHS of a terminal rule is in fact a terminal.
*/
void Grammar::terminalRHSCheck(vector< vector<Grammar::element*> > * rhsList)
{
    for(int i = 0; i < (*rhsList).size(); i++)
    {
        vector<element*> rhs = (*rhsList)[i];
        for(int j = 0; j < rhs.size(); j++) //this should just be 1 item but may change grammar and this would accomodate
        {
            element* rhsItem = rhs[j];
            if(!rhsItem->isTerminal)
            {
                cout << "NonTerminal declared on RHS of Terminal rule!!" << endl;
                cout << "The NonTerminal is: " << rhsItem->value << endl;
                syntax_error();
            }
        }
    }
}

/*
    ---------------------------
    PARSING METHODS
    ---------------------------
*/


/*
    Given the input file with a defined CFG, will parse and return the created Grammar object.

*/
void Grammar::parse()
{
    parse_grammar();

    //Semantic checks now applied as well as understanding properties of this grammar.
    terminalCheck();

}

void Grammar::parse_grammar()
{
    //grammar -> rules

    parse_rules();
    expect_token(END_OF_FILE);  //by now all of input should've been consumed

}

void Grammar::parse_rules()
{
    //rules -> RuleSection
    //rules -> RULES LEFTCURL ruleList RIGHTCURL TerminalRules
    //Second rule can be rewritten as 'RuleSection TerminalRules' but this is more clear as...
    //  Rule section by itself can be defined with simply ruleList

    parse_ruleSection();

    Token t = peek_token();
    
    if(t.type == TERMINALS)
    {
        
        parse_terminalRules();
    }


}

void Grammar::parse_ruleSection()
{
    //ruleSection -> RULES LEFTCURL ruleList RIGHTCURL
    //ruleSection -> ruleList

    Token t = peek_token();


    if(t.type == RULES)
    {
        expect_token(RULES);
        expect_token(LEFTCURL);
        parse_ruleList();
        expect_token(RIGHTCURL);
        
    }
    else
    {
        parse_ruleList();
    }
    
}

void Grammar::parse_ruleList()
{
    //ruleList -> rule
    //ruleList -> rule ruleList

    parse_rule();


    Token t = peek_token();
    
    if(t.type == ID)        //if an ID is seen then safe to assume it is the name of another rule
    {
        parse_ruleList();
    }

}

void Grammar::parse_rule()
{
    //rule -> ID ARROW rhsList SEMICOLON
    Token ruleToken = expect_token(ID);

    element* ruleElement = elementLookup(ruleToken.value);
    //element* ruleElement = getElement(indexOfElem);
    ruleElement->value = ruleToken.value;
    ruleElement->isTerminal = false;
    ruleElement->isTerminalRule = false;


    expect_token(ARROW);

    //appends parsed rhs to currentRule RHS
    //  This is because multiple occurences of lhs nonterminal can appear throughout
    //  the defined grammar and you don't want to reset it. I use addresses here because
    //  passing by value two 2-dimensional vectors felt gross.
    vector< vector<element*> > newRHS = parse_rhsList();

    combineRHS( &(ruleElement->rhsList) , &newRHS );

    //ruleElement->rhs = parse_rhsList();

    expect_token(SEMICOLON);

}

/*
    @return vector of all right hand side's of a rule, each rhs is also a vector of ints
    corresponding to index in Grammar's universe of symbols
*/
vector< vector<Grammar::element*>> Grammar::parse_rhsList()
{
    //rhsList -> rhs
    //rhsList -> rhs LINE rhsList
    vector<element*> rhs = parse_rhs();

    Token possLine = peek_token();

    vector< vector<element*> > allRHSs = {};

    if(possLine.type == LINE)
    {
        Token test = lexer->getToken();

        if(test.type != LINE) //this is redundent but just a sanity check
        {
            syntax_error(test.line_num);
        }

        allRHSs = parse_rhsList();
    }

    allRHSs.push_back(rhs);

    return allRHSs;
}

/*
    Parses the rhs of a rule, if more then a single element it will be called recursively.
    @return vector of indexes of corresponding symbols in grammar's universe
*/
vector<Grammar::element*> Grammar::parse_rhs()
{
    //rhs -> ID
    //rhs -> EPSILON
    //rhs -> ID rhs

    Token token = peek_token();

    if(token.type == EPSILON)
    {
        lexer->getToken();      //consumes EPSILON
        vector<element*> justEps = {universe[1]};
        return justEps;
    }

    vector<element*> rhs = {};

    Token t = expect_token(ID);           //either the name of a rule or terminal
    element* newElem = elementLookup(t.value);
    rhs.push_back(newElem);

    token = peek_token();
    if(token.type == ID)
    {
        rhs = (* parse_rhs(&rhs));
    }

    return rhs;

}


/*
    Overloaded version of parse_rhs(), used when multiple elements in a rule's rhs. All calls should call parse_rhs().
    @param vector of indexes corresponding to symbols in universe. This is only used to make recursion easier, initially should be empty
    @return vector of indexes of corresponding symbols in grammar's universe
*/
vector<Grammar::element * >* Grammar::parse_rhs(vector<Grammar::element*>* rhs)
{
    //rhs -> ID
    
    //rhs -> ID rhs


    Token t = expect_token(ID);           //either the name of a rule or terminal
    element* newElem = elementLookup(t.value);
    rhs->push_back(newElem);

    Token token = peek_token();
    if(token.type == ID)
    {
        rhs = parse_rhs(rhs);
    }

    return rhs;

}


void Grammar::parse_terminalRules()
{
    //terminalRules -> TERMINALS LEFTCURL terminal_ruleList RIGHTCURL

    expect_token(TERMINALS);
    expect_token(LEFTCURL);
    
    parse_terminal_ruleList();

    expect_token(RIGHTCURL);

}


void Grammar::parse_terminal_ruleList()
{
    //terminal_ruleList -> terminal_rule
    //terminal_ruleList -> terminal_rule terminal_ruleList

    parse_terminal_rule();

    Token token = peek_token();
    if(token.type == ID)
    {
        parse_terminal_ruleList();
    }


}


void Grammar::parse_terminal_rule()
{
    //terminal_rule -> ID ARROW term_rhsList SEMICOLON

    //TODO : Return element and set values accordingly

    Token t = expect_token(ID);
    element* currentElement = elementLookup(t.value);

    //Set properties of current terminal rule
    //element* currentRule = getElement(indexOfElement);
    currentElement->isTerminal = false;
    currentElement->isTerminalRule = true;

    expect_token(ARROW);

    //appends parsed rhs to currentRule RHS
    //  This is because multiple occurences of lhs nonterminal can appear throughout
    //  the defined grammar and you don't want to reset it. I use addresses here because
    //  passing by value two 2-dimensional vectors felt gross. 
    vector< vector<element*> > newRHS = parse_terminal_rhsList();
    combineRHS( &(currentElement->rhsList) , &(newRHS) );

    //currentRule->rhs.push_back(parse_terminal_rhsList());

    expect_token(SEMICOLON);
}

/*
    @return A vector of the different RHS's of a terminal only rule. Each rhs is a vector with index of corresponding symbol in universe
*/
vector< vector<Grammar::element*> > Grammar::parse_terminal_rhsList()
{
    //terminal_rhsList -> rhs
    //terminal_rhsList -> rhs LINE terminal_rhsList
    //terminal_rhsList -> rhs COMMA terminal_rhsList

    //TODO: Consider making this more efficient instead of copying a potentionally really large matrix over and over 

    vector<element*> rhs = parse_terminal_rhs();
    vector< vector<element*> > listOfRHS = {};

    Token t = peek_token();
    if(t.type == LINE || t.type == COMMA)
    {
        t = lexer->getToken();       //actually consumes the LINE or COMMA

        if(t.type != LINE && t.type != COMMA) //this is redundent but just a sanity check
        {
            syntax_error(t.line_num);
        }

        listOfRHS = parse_terminal_rhsList();
    }

    listOfRHS.push_back(rhs);   //order in which rules get added doesn't actually matter but rules end up getting applied in reverse

    return listOfRHS;

}

/*
    @return a vector of one element which is the index of the corresponding symbol in grammar's universe
*/
vector<Grammar::element*> Grammar::parse_terminal_rhs()
{
    //terminal_rhs -> ID

    //Terminal rules and regular rules are structurally same object only terminal rules have additional..
    //logic checks applied to them. Even though any given singular rhs will only have 1 element, it still needs to..
    //be a vector.

    //TODO: Apply logic check to ensure none of the terminals here were previous rules.

    Token token = expect_token(ID);
    element* newElem = elementLookup(token.value);   //index of found/created element in universe
    vector<element*> rhs = {newElem};

    //element* rhsElem = getElement(indexOfElem);


    return rhs;

}




