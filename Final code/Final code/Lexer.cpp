#include <iostream>
#include <fstream>
#include <unordered_set>
#include <string>
#include <vector>
#include "Lexer.h"
#include<regex>


Lexer::Lexer(std::string source) {
    filename = source;
    LineNumber = 1;
    TokenEndsInPos = 1;
    PosInLine = 1;
    buffer = "";
    TokenizingString = false;
    TokenizingComment = false;
}

std::regex iden("[a-zA-Z][a-zA-Z0-9_]*");
std::regex lit("^[0-9]+$");


bool Lexer::FoundKeyword() {
    static const std::unordered_set <std::string> keywords = {
        "alignas",
        "alignof",
        "and",
        "and_eq",
        "asm",
        "atomic_cancel",
        "atomic_commit",
        "atomic_noexcept",
        "auto",
        "bitand",
        "bitor",
        "bool",
        "break",
        "case",
        "catch",
        "char",
        "char8_t",
        "char16_t",
        "char32_t",
        "class",
        "compl",
        "concept",
        "const",
        "consteval",
        "constexpr",
        "constinit",
        "const_cast",
        "continue",
        "co_await",
        "co_return",
        "co_yield",
        "decltype",
        "default",
        "delete",
        "do",
        "double",
        "dynamic_cast",
        "else",
        "enum",
        "explicit",
        "export",
        "extern",
        "false",
        "float",
        "for",
        "friend",
        "goto",
        "if",
        "inline",
        "int",
        "long",
        "mutable",
        "main()"
        "namespace",
        "new",
        "noexcept",
        "not",
        "not_eq",
        "nullptr",
        "operator",
        "or",
        "or_eq",
        "private",
        "protected",
        "public",
        "reflexpr",
        "register",
        "reinterpret_cast",
        "requires",
        "return",
        "short",
        "signed",
        "sizeof",
        "static",
        "static_assert",
        "static_cast",
        "struct",
        "switch",
        "synchronized",
        "template",
        "this",
        "thread_local",
        "throw",
        "true",
        "try",
        "typedef",
        "typeid",
        "typename",
        "union",
        "unsigned",
        "using",
        "virtual",
        "void",
        "volatile",
        "wchar_t",
        "while",
        "xor",
        "xor_eq",
        "string", // not c++ specific keyword
    };

    // if buffer in set, return true, else false
    return (keywords.find(buffer) != keywords.end() ? true : false);

}
bool Lexer::FoundOperator() {
    static const std::unordered_set <std::string> operators = {
        "+", "-", "*", "/", "%",
        "=", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", ">>=", "<<=",
        "==", "!=", ">", "<", ">=", "<=",
        "&&", "||", "!", "?",
        ">>", "<<",
    };

    return (operators.find(buffer) != operators.end() ? true : false);
}
bool Lexer::FoundDelimiter() {
    static const std::unordered_set <std::string> delimiters = {
        "(", ")",
        "{", "}",
        "[", "]",
        ";", ",",
    };

    return (delimiters.find(buffer) != delimiters.end() ? true : false);
}
bool Lexer::FoundWhitespace() {
    static const std::unordered_set <char> whitespace = {
        ' ',        // space
        '\f', '\n', // form feed, line feed
        '\r',       // carriage return
        '\t', '\v', // horizontal tab, vertical tab
    };

    return (whitespace.find(ch) != whitespace.end() ? true : false);
}
bool Lexer::FoundComment() {
    if(buffer.substr(0, 2) == "//"){
        if(buffer.back() == '\n' || buffer.back() == '\r') {
            TokenizingComment = false;
            return true;
        }
        else {
            TokenizingComment = true;
            return false;
        }
    }
    else {
        return false;
    }
    // TODO: Include multiline comments
}
bool Lexer::FoundLiteral() {
    if(isdigit(buffer[0])) {
        return true;
    }
    else {
        return false;
    }
}
bool Lexer::FoundString() {
    if(buffer[0] == '"') {
        if(buffer.back() == '"') {
            TokenizingString = false;
            return true;
        }
        else {
            TokenizingString = true;
            return false;
        }
    }
    else if(buffer[0] == '\'') {
        if(buffer.back() == '\'') {
            TokenizingString = false;
            return true;
        }
        else {
            TokenizingString = true;
            return false;
        }
    }
    else {
        return false;
    }
}
bool Lexer::FoundIdentifier() {

    if(buffer.length() <= 255) {
        if(isalpha(buffer[0]) or buffer[0] == '_') {
            if(!Lexer::FoundKeyword()){
                return true;
            }
        }
    }
    return false;
}
void Lexer::TestIfToken() {
    if (FoundKeyword())
        //std::cout << buffer <<std::endl;
        std::cout << buffer << " is keyword, line: " << LineNumber << ":" << PosInLine << std::endl;
    else if (FoundOperator())
        //std::cout << buffer <<std::endl;
        std::cout << buffer << " is operator, line: " << LineNumber << ":" << PosInLine << std::endl;
    else if (FoundDelimiter())
        //std::cout << buffer <<std::endl;
        std::cout << buffer << " is delimiter, line: " << LineNumber << ":" << PosInLine << std::endl;
    else if (FoundComment())
        //std::cout << buffer <<std::endl;
        std::cout << buffer << " is comment, line: " << LineNumber << ":" << PosInLine << std::endl;
    else if (FoundLiteral())
        //std::cout << buffer <<std::endl;
        std::cout << buffer << " is literal, line: " << LineNumber << ":" << PosInLine << std::endl;
    else if (FoundString())
        //std::cout << buffer <<std::endl;
        std::cout << buffer << " is string, line: " << LineNumber << ":" << PosInLine << std::endl;
    else if (FoundIdentifier())
        //std::cout << buffer <<std::endl;
        std::cout << buffer << " is identifier, line: " << LineNumber << ":" << PosInLine << std::endl;
    else{
        // std::cout << buffer << " is not recognized, line: " << LineNumber << ":" << PosInLine << std::endl;
        // TODO: Uncomment token not found message
        // TODO: Add way to not re-print comments, strings
    }
}
void Lexer::Tokenize() {
    std::ifstream file(filename, std::ios::in);
    if (file.fail()) {
        std::cout << "Cant open file." << std::endl;
        exit(0);
    }
    while (file >> std::noskipws >> ch) {
        TokenEndsInPos++;
        if(TokenizingComment) {
            if (ch == '\n' || ch == '\r') {
                buffer += ch;
                TestIfToken();
                buffer = "";
                LineNumber++;
                TokenEndsInPos = 1;
                TokenizingComment = false;
            }
            else {
                buffer += ch;
                PosInLine = TokenEndsInPos - buffer.length();
            }
        }
        else if(TokenizingString) {
            if (ch == '"' || ch == '\'') {
                buffer += ch;
                TestIfToken();
                buffer = "";
                TokenizingString = false;
            }
            else {
                buffer += ch;
                PosInLine = TokenEndsInPos - buffer.length();
            }
        }
        else if (!FoundWhitespace()) {
            buffer += ch;
            PosInLine = TokenEndsInPos - buffer.length();
        }
        else {
            if(buffer != "") {
                TestIfToken();
                if(!TokenizingComment && !TokenizingString){
                    buffer = "";
                }
                else {
                    buffer += " ";
                }
            }
            if (ch == '\n' || ch == '\r') {
                LineNumber++;
                TokenEndsInPos = 1;
            }
        }
    }
    file.close();
}
