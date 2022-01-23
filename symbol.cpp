#include "symbol.hpp"

#include <sstream>

#define STRINGIFY(var) #var

static inline std::string toString(auto n) {
    std::stringstream ss;
    ss << n;
    return ss.str();
}

namespace PSON {

    Symbol::operator std::string() {
        std::string result;
        result += (std::string)
                          "["
                  + (
                          (t == NUL) ? STRINGIFY(NUL) :
                          (t == EQ) ? STRINGIFY(EQ) :
                          (t == UN_MIN) ? STRINGIFY(UN_MIN) :
                          (t == L_RND) ? STRINGIFY(L_RND) :
                          (t == L_SQ) ? STRINGIFY(L_SQ) :
                          (t == R_RND) ? STRINGIFY(R_RND) :
                          (t == R_SQ) ? STRINGIFY(R_SQ) :
                          (t == NUC_ID) ? STRINGIFY(NUC_ID) :
                          (t == UC_ID) ? STRINGIFY(UC_ID) :
                          (t == STR) ? STRINGIFY(STR) :
                          (t == INT) ? STRINGIFY(INT) :
                          (t == FP) ? STRINGIFY(FP) :
                          (t == ARRAY) ? STRINGIFY(ARRAY) :
                          (t == MAP) ? STRINGIFY(MAP) :
                          (t == OBJECT) ? STRINGIFY(OBJECT) :
                          "null"
                  )
                  + " "
                  + (
                          (t == NUL) ? "null" :
                          (t == NUC_ID) ? "\"" + *(this->v.NUC_ID) + "\"" :
                          (t == UC_ID) ? *(this->v.UC_ID) :
                          (t == STR) ? "\"" + *(this->v.STR) + "\"" :
                          (t == INT) ? toString(this->v.INT) :
                          (t == FP) ? toString(this->v.FP) :
                          (t == ARRAY) ? STRINGIFY(ARRAY) :
                          (t == MAP) ? STRINGIFY(MAP) :
                          (t == OBJECT) ? (std::string) (*(v.object)) :
                          "null"
                  )
                  + "]";
        return result;
    }

    Symbol::~Symbol() = default;

}
