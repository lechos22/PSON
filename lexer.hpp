#ifndef PSON__LEXER_HPP
#define PSON__LEXER_HPP

#include <istream>

#include "parser.hpp"

namespace PSON {

class Lexer {
private:
    std::istream* is;
    Symbol next_token;
    void skip();
public:
    Lexer(std::istream* is); // Construct a lexer with given input stream
    Symbol preview_next(); // Preview next token
    Symbol next(); // Skip and return a token
    bool eof(); // Check if any tokens are available
};

};

#endif
