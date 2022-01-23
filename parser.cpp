#include <cmath>
#include "parser.hpp"

namespace PSON {

    bool Parser::reduce() {
        // todo semantics
        int length = (int) (buf.size());
        if (length >= 1 and buf.back().t == Symbol::L_SQ) {
            buf.back().t = Symbol::ARRAY;
            buf.back().v.array = new Array;
        } else if (length >= 2 and buf[length - 2].t == Symbol::ARRAY and buf.back().t == Symbol::OBJECT) {
            buf[length - 2].v.array->push_back(buf.back().v.object);
            buf.pop_back();
        } else if (length >= 2 and buf[length - 2].t == Symbol::ARRAY and buf.back().t == Symbol::R_SQ) {
            buf.pop_back();
            buf.back().t = Symbol::OBJECT;
            buf.back().v.object = new Object(buf.back().v.array);
        } else if (length >= 1 and buf.back().t == Symbol::L_RND) {
            buf.back().t = Symbol::MAP;
            buf.back().v.map = new Map;
        } else if (length >= 4 and buf[length - 4].t == Symbol::MAP and
                   (buf[length - 3].t == Symbol::NUC_ID or buf[length - 3].t == Symbol::UC_ID) and
                   buf[length - 2].t == Symbol::EQ and buf.back().t == Symbol::OBJECT) {
            (*(buf[length - 4].v.map))[*(buf[length - 3].v.NUC_ID)] = buf.back().v.object;
            delete buf[length - 3].v.NUC_ID;
            buf.pop_back();
            buf.pop_back();
            buf.pop_back();
        } else if (length >= 2 and buf[length - 2].t == Symbol::MAP and buf.back().t == Symbol::R_RND) {
            buf.pop_back();
            buf.back().t = Symbol::OBJECT;
            buf.back().v.object = new Object(buf.back().v.map);
        } else if (length >= 2 and buf[length - 2].t == Symbol::UN_MIN and buf.back().t == Symbol::OBJECT) {
            buf[length - 2].t = Symbol::OBJECT;
            buf[length - 2].v.object =
                    (buf.back().t == Symbol::INT) ? new Object(buf.back().v.object->INT) :
                    (buf.back().t == Symbol::INT) ? new Object(buf.back().v.object->FP) :
                    new Object(FP_NAN);
            buf.pop_back();
        } else if (length >= 1 and buf.back().t == Symbol::STR) {
            buf.back().t = Symbol::OBJECT;
            buf.back().v.object = new Object(buf.back().v.STR);
        } else if (length >= 1 and buf.back().t == Symbol::INT) {
            buf.back().t = Symbol::OBJECT;
            buf.back().v.object = new Object(buf.back().v.INT);
        } else if (length >= 1 and buf.back().t == Symbol::UC_ID and lexer_p->preview_next().t != Symbol::EQ) {
            buf.back().t = Symbol::OBJECT;
            buf.back().v.object = new Object(buf.back().v.UC_ID);
        } else if (length >= 1 and buf.back().t == Symbol::FP) {
            buf.back().t = Symbol::OBJECT;
            buf.back().v.object = new Object(buf.back().v.FP);
        } else if (length >= 1 and buf.back().t == Symbol::NUL) {
            buf.back().t = Symbol::OBJECT;
            buf.back().v.object = new Object(nullptr);
        } else {
            return false;
        }
        return true;
    }

    Parser::Parser(Lexer &lexer) {
        lexer_p = &lexer;
        for (Symbol s = lexer.next(); !(lexer.eof()); s = lexer.next()) {
            buf.push_back(s);
            while (reduce());
        }
        Symbol s = lexer.preview_next();
        buf.push_back(s);
        while (reduce());
        if(buf.size() != 1) throw std::exception();
        if(buf.front().t != Symbol::OBJECT) throw std::exception();
        result = buf.front().v.object;
    }

    Parser::~Parser() = default;

}
