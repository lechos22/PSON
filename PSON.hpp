//
// Created by Lech on 07.12.2021.
//

#ifndef PSON_LIBPSON_H
#define PSON_LIBPSON_H

#include <cstdint>
#include <ostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <exception>

namespace PSON {
    using std::string;
    using std::vector;
    using std::unordered_map;
    using std::nullptr_t;
    // Structure declarations
    struct Map;
    struct Array;
    struct Object;
    // Structure definitions
    struct Object {
        enum type_t : char{
            NONE = '0',
            INT = 'i',
            FLOAT = 'f',
            STRING = 's',
            END = 'Z',
            MAP = 'M',
            ARRAY = 'A'
        } type;
        union val_t{
            int i;
            double f;
            std::string *str;
            Map *map;
            Array *array;
            nullptr_t none;
            nullptr_t end;
        }val;

        Object& operator= (const int& value);
        Object& operator= (const float& value);
        Object& operator= (const double& value);
        Object& operator= (const string& value);
        Object& operator= (const char*& value);
        Object& operator= (Map* value);
        Object& operator= (Array* value);
        Object& operator= (const nullptr_t& value);
        Object& operator[] (int i) const;
        Object& operator[] (const string& k) const;

        Object();

        ~Object();
    };

    struct SyntaxError :std::exception{
        [[nodiscard]] const char * what() const noexcept override;
    };
    struct Map : public unordered_map<string,Object>{};
    struct Array: public vector<Object>{
        explicit Array(int size);
        Array() = default;
    };

    Object parseArray(const char *src, bool verify=true);

    bool verify(const char* src);
}

std::ostream& operator<<(std::ostream& os, PSON::Object& obj);

#endif //PSON_LIBPSON_H
