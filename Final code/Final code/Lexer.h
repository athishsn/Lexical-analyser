#ifndef LEXER_H
#define LEXER_H

#include<iostream>
#include<fstream>
#include<unordered_set>
#include<string>
#include<vector>

class Lexer {
private:
    std::string filename;
    int LineNumber;
    int TokenEndsInPos;
    int PosInLine;
    char ch;
    std::string buffer;
    bool TokenizingComment;
    bool TokenizingString;
    std::vector<std::string> tokens; // TODO: Add vector as return token list
public:
    Lexer(std::string source = "testing.txt");
    bool FoundKeyword();
    bool FoundOperator();
    bool FoundDelimiter();
    bool FoundWhitespace();
    bool FoundComment();
    bool FoundLiteral();
    bool FoundString();
    bool FoundIdentifier();
    void TestIfToken();
    void Tokenize();
};

#endif

// TODO: Document lexer header
