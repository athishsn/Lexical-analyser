#include "Lexer.h"
#include "Lexer.cpp"

int main() {
    Lexer lex("program.txt");

    lex.Tokenize();

    return 1;

    // TODO: Allow user to input own file name from command line / CL args
    // TODO: Implement token type for read back list
}
