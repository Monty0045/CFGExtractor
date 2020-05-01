/*
    Written by Wyatt Turner for CSE355 @ Arizona State University
    Spring 2020, Dr. Ben Amor

*/

#include "grammar.h"
#include "stdlib.h"

using namespace std;

int main()
{
    srand(time(NULL));                  //used to get random strings in language, used in grammar.cpp

    Grammar * grammar = new Grammar();

    grammar->parse();
    cout << "Parsed properly!!\n" << endl;

    grammar->printRules();

    cout << "\n\nGenerating string within this language: \n" << endl;
    
    for(int i = 0; i < 5; i++)
    {
        grammar->generateString();
    }

    return 0;
}