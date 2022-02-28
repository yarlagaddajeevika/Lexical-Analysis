
//implementation of Lexer using Finite State Machine.

//list of all libraries
#include <iostream>
#include <cstdlib>
#include <cctype>
#include <string>
#include <vector>
#include <fstream>
#include<bits/stdc++.h>
using namespace std;
 
/* FSM array holds the numerical order of the transition states*/
//listing all with the enums
enum FSM_TRANSITIONS_ARRAY
{
    REJECT = 0,
    INTEGER,
    REAL,
    OPERATOR,
    IDENTIFIER,
    UNKNOWN,
    SPACE
};
 
/* This is Finite State Machine Table
   zero represents a place holder
   array positions representation - integer,  real,  operator,  identifier,  unknown,  space */
int stateTableArray[][7] = {{0, INTEGER,  REAL,  OPERATOR,  IDENTIFIER, UNKNOWN,  SPACE},
/* STATE 1 */   {INTEGER,  INTEGER,  REAL,   REJECT,   REJECT,  REJECT,  REJECT},
/* STATE 2 */   {REAL,       REAL,  UNKNOWN, REJECT,   REJECT,  REJECT,  REJECT},
/* STATE 3 */   {OPERATOR,  REJECT, REJECT,  REJECT,   IDENTIFIER,  REJECT,  REJECT},
/* STATE 4 */   {IDENTIFIER,    IDENTIFIER, REJECT,  IDENTIFIER,   IDENTIFIER,  REJECT,  REJECT},
/* STATE 5 */   {UNKNOWN,  UNKNOWN, UNKNOWN, UNKNOWN,  UNKNOWN, UNKNOWN, REJECT},
/* STATE 6 */   {SPACE,     REJECT, REJECT,  REJECT,   REJECT,  REJECT,  REJECT}};
 
 
// struct to hold token information
struct Token
{
    string token; // define token
    int lexeme; // lexeme
    string lexemeName; //name of lexeme
};
 
// prototypes of all functions used below
vector<Token> Lexer(string expression); //Lexer 
int Get_FSM_Col(char currentCharacter);
string GetLexemeName(int lexeme);
bool checkKeyword(string word);

int main()
{
    // declare variables needed
    ifstream file;
    string fileName = ""; //name of the file
    string expression = "";
    vector<Token> tokens; //vector of tokens - dynamic storage
 
    // get file name from user for the input
    cout<<"\nPlease enter the name of the file where your input is stored: ";
    getline(cin, fileName);
 
    //open the file
    file.open(fileName.c_str());
 
    //failed to open a file, return an error
    if(file.fail())
    {
        cout<<"\n** ERROR - the file \""<<fileName<<"\" cannot be found!\n\n";
        exit(1);
    }
    ofstream outputFile;
    // read each line in the file
    while(getline(file, expression))
    {
        // use the "Lexer" function to isolate integer, real, operator,string, and unknown tokens
        tokens = Lexer(expression);

        outputFile.open ("output_Jeevika.txt");

        for(unsigned x = 0; x < tokens.size(); ++x)
            {
                if(checkKeyword(tokens[x].token)){
                outputFile<<"KEYWORD"<<"  \t"
                    <<tokens[x].token<<endl;
                }else{
                outputFile<<tokens[x].lexemeName<<"  \t"
                    <<tokens[x].token<<endl;
                }
            }
        }
 
    //close the file
    file.close();
    outputFile.close();

    cout<< "\nPlease open the file \"output_Jeevika.txt\" to view the output\n";

    return 0;
}// end of main

//checks if the token is a keyword
bool checkKeyword(string tokenName){
    //string token = ::tolower(tokenName);
    transform(tokenName.begin(), tokenName.end(), tokenName.begin(), ::tolower);
    if(tokenName == "while"){
           return true;
    }else if(tokenName == "if"){
           return true;
    }else if(tokenName == "else"){
           return true;
    }else if(tokenName == "for"){
           return true;
    }else if(tokenName == "do"){
           return true;
    }else if(tokenName == "then"){
           return true;
    }else if(tokenName == "elseif"){
           return true;
    }else if(tokenName == "elif"){
           return true;
    }else if(tokenName == "elsif"){
           return true;
    }else {
           return false;
    }
}

/**
* Parses the "expression" string using the Finite State Machine to isolate each individual token and lexeme name in the expression.
* @param expression - A std::string containing text.
* @return - Returns a vector containing the tokens found in the string
*/
vector<Token> Lexer(string expression)
{
    Token access;
    vector<Token> tokens;
    char currentCharacter = ' ';
    int col = REJECT; //enum and holds 0
    int currentState = REJECT;
    int prevState = REJECT;
    string currentToken = "";
 
    // use an FSM to parse the expression
    for(unsigned x = 0; x < expression.length();)
    {
        currentCharacter = expression[x];
        // get the column number for current character
        col = Get_FSM_Col(currentCharacter);
 
        // get the current state of the expression
        currentState = stateTableArray[currentState][col];
 
        if(currentState == REJECT)
        {
            if(prevState != SPACE) // ignore whitespace
            {
                access.token = currentToken;
                access.lexeme = prevState;
                access.lexemeName = GetLexemeName(access.lexeme);
                tokens.push_back(access);
            }
            currentToken = "";
        }
        else
        {
            currentToken += currentCharacter;
            ++x;
        }
        prevState = currentState;
 
    }
    // this ensures the last token gets saved when
    // we reach the end of the loop (if a valid token exists)
    if(currentState != SPACE && currentToken != "")
    {// ignore whitespace
        access.token = currentToken;
        access.lexeme = currentState;
        access.lexemeName = GetLexemeName(access.lexeme);
        tokens.push_back(access);
    }
    return tokens;
}// end of Lexer
 
/**
* Determines the identifier.
* @param currentCharacter - A character.
* @return - Returns identifier type.
*/
int Get_FSM_Col(char currentCharacter)
{
    // check for whitespace
    if(isspace(currentCharacter))
    {
        return SPACE;
    }
 
    // check for integer numbers
    else if(isdigit(currentCharacter))
    {
        return INTEGER;
    }
 
    // check for real numbers
    else if(currentCharacter == '.')
    {
        return REAL;
    }
 
    // check for characters
    else if(isalpha(currentCharacter))
    {
        return IDENTIFIER;
    }
 
    // check for operators
    else if(ispunct(currentCharacter))
    {
        return OPERATOR;
    }
    return UNKNOWN;
}// end of Get_FSM_Col
 
/**
* Switch to return the string equivalent lexeme token type.
* @param lexeme - An integer lexeme token type.
* @return - string representing the name of the integer lexeme token type.
*/
string GetLexemeName(int lexeme)
{
    switch(lexeme)
    {
        case INTEGER:
           return "INTEGER";
           break;
        case REAL:
           return "REAL  ";
           break;
        case OPERATOR:
           return "OPERATOR";
           break;
        case IDENTIFIER:
           return "IDENTIFIER";
           break;
        case UNKNOWN:
           return "UNKNOWN";
           break;
        case SPACE:
           return "SPACE";
           break;
        default:
           return "ERROR";
           break;
    }
}