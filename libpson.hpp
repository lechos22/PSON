//
// Created by Lech on 07.12.2021.
//

#ifndef PSON_LIBPSON_H
#define PSON_LIBPSON_H

#include <cstdint>
#include <ostream>

extern "C++" {
class PSON {
public:
    // Structure declarations
    struct Array;
    struct Object;
    // Structure definitions
    struct Array {
        std::uint32_t size;
        PSON::Object *value;

        Array();

        explicit Array(unsigned int size);
    };
    struct Object {
        enum type_t : char{
            INT = 'i',
            FLOAT = 'f',
            STRING = 's',
            ARRAY = 'A'
        } type;
        char *name;
        union {
            std::int32_t i;
            float f;
            char *str;
            Array *array;
        };

        Object();

        Object(const char* name, int value);

        Object(const char* name, float value);

        Object(const char* name, char* value);

        Object(const char* name, Array* value);

        explicit Object(const char *src);

        friend std::ostream& operator<<(std::ostream& os, PSON::Object obj);
    };

};

}

#endif //PSON_LIBPSON_H