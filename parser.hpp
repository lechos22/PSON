#ifndef PSON__PARSER_HPP
#define PSON__PARSER_HPP

#include "lexer.hpp"
#include "symbol.hpp"

namespace PSON {

    class Parser {
        Lexer* lexer_p;
    public:
        std::vector<Symbol> buf;
        Object* result;

        bool reduce();

        explicit Parser(Lexer &lexer);
        ~Parser();
    };

}

#endif