#ifndef PSON__SYMBOL_H
#define PSON__SYMBOL_H

#include <string>
#include "PSON.hpp"

namespace PSON {

    struct Symbol {
        enum type_enum {
            NUL,
            EQ,
            UN_MIN,
            L_RND,
            L_SQ,
            R_RND,
            R_SQ,
            NUC_ID,
            UC_ID,
            STR,
            INT,
            FP,
            ARRAY,
            MAP,
            OBJECT
        } t;
        union {
            Object *object;
            Array *array;
            Map *map;
            std::string *UC_ID;
            std::string *NUC_ID;
            std::string *STR;
            float FP;
            int INT;
        } v;

        explicit operator std::string();

        ~Symbol();
    };

}

#endif
