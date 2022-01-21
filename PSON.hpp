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
struct Map;
struct Array;
struct Object;

// Structure definitions
struct Object{};
struct Map : public std::map<std::string,Object>{};
struct Array : public std::vector<Object>{};

}

#endif
