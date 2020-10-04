# CFGExtractor
##### This program is written by Wyatt Turner as an Extra Credit Project for CSE 355 with Dr. Ben Amor @ Arizona State University, Spring 2020

### Context Free Grammar Extractor parses a definition of a context-free grammar from a text file and outputs 5 strings with in that language. Currently a proof of concept, further features such as seeing if a string is within a grammar and arbitrary outputs are desired.

Grammar defined in text file can have following form
  (Without 'Terminal Rules'):
  
      Rules {
        rule -> TERMINAL otherrule alsoATerminal;
        rule -> thisRuleHasTwoRHS;
        otherrule -> thisRuleAlsoHas | twoRHSFromTheOR;
      }
      
      
  Alternative Version (Without 'Terminal Rules'):
    
    rule -> TERMINAL otherrule alsoATerminal;
    rule -> thisRuleHasTwoRHS;
    otherrule -> thisRuleAlsoHas | twoRHSFromTheOR;
    
   With 'Terminal Rules': 
    
    Rules {
      rule -> TERMINAL terminalRule rule | TERMINAL;
    }
    Terminals {
      terminalRule -> Everything | OnRHS | Is | A | Terminal;
    }
    
    
  ## Note about language of defining Context Free Grammars: 
  * The 'Rules {}' brackets is entirely optional however the 'Terminal {}' is not should you desire to include hard defined 'Terminal Rules'.
  * Terminal Rules are rules where everything on RHS is a terminal. This is enforced during parsing, should a rule appear on RHS a syntax error is thrown.
  * Everything appearing on the RHS is a terminal unless it appears somewhere on the LHS of rule declarations.
  
  ## Using Program
  *Note: This is a rather hackish program currently, only tested to run on my Linux Mint 20.04 machine.*
  1. Run make in directory CFGExtractor/source will output the binary.
  2. Run the binary with corresponding textfile from terminal. For example on binary 'a.out' and a textfile of name 'sentence':
      > ./a.out < sentence
