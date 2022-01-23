#ifndef PSON__LEXER_HPP
#define PSON__LEXER_HPP

#include <istream>

#include "symbol.hpp"

namespace PSON {

    class Lexer {
    private:
        std::istream *is;
        Symbol next_token;

        bool skip();

    public:
        Lexer(std::istream *is); // Construct a lexer with given input stream
        Symbol preview_next(); // Preview next token
        Symbol next(); // Skip and return a token
        bool eof() const; // Check if any tokens are available
    };

};

#endif
