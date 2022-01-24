//
// Created by Lech on 07.12.2021.
//

#ifndef PSON__PSON_H
#define PSON__PSON_H

#include <cstdint>
#include <string>
#include <vector>
#include <map>

namespace PSON {

// Structure declarations
    struct Object;
    typedef std::map<std::string, Object*> Map;
    typedef std::vector<Object*> Array;

// Structure definitions
    struct Object {
        enum type_enum {
            NUL,
            STR,
            INT,
            FP,
            ARRAY,
            MAP
        } t;
        union {
            std::nullptr_t NUL;
            std::string *STR;
            int INT;
            float FP;
            Array *ARRAY;
            Map *MAP;
        } v;

        Object() = default;

        explicit Object(std::nullptr_t val);

        explicit Object(std::string *val);

        explicit Object(int val);

        explicit Object(float val);

        explicit Object(Array *val);

        explicit Object(Map *val);

        ~Object();

        explicit operator std::string() const;
    };

}

#endif
