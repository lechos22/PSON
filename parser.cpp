#include <sstream>
#include "parser.hpp"
#define STRINGIFY(var) #var

static inline std::string toString(auto n){
    std::stringstream ss;
    ss<<n;
    return ss.str();
}

namespace PSON{
    Symbol::operator std::string(){
        std::string result;
        result += (std::string)
            "["
            + (
                (this->t == NUL) ? STRINGIFY(NUL) :
                (this->t == EQ) ? STRINGIFY(EQ) :
                (this->t == UN_MIN) ? STRINGIFY(UN_MIN) :
                (this->t == L_RND) ? STRINGIFY(L_RND) :
                (this->t == L_SQ) ? STRINGIFY(L_SQ) :
                (this->t == R_RND) ? STRINGIFY(R_RND) :
                (this->t == R_SQ) ? STRINGIFY(R_SQ) :
                (this->t == NUC_ID) ? STRINGIFY(NUC_ID) :
                (this->t == UC_ID) ? STRINGIFY(UC_ID) :
                (this->t == STR) ? STRINGIFY(STR) :
                (this->t == INT) ? STRINGIFY(INT) :
                (this->t == FP) ? STRINGIFY(FP) :
                (this->t == ARRAY) ? STRINGIFY(ARRAY) :
                (this->t == MAP) ? STRINGIFY(MAP) :
                (this->t == OBJECT) ? STRINGIFY(OBJECT) :
                "null"
            )
            + " "
            + (
                (this->t == NUL) ? "null" :
                (this->t == NUC_ID) ? "\"" + *(this->v.NUC_ID) + "\"" :
                (this->t == UC_ID) ? *(this->v.UC_ID) :
                (this->t == STR) ? "\"" + *(this->v.STR) + "\"" :
                (this->t == INT) ? toString(this->v.INT) :
                (this->t == FP) ? toString(this->v.FP) :
                // (this->t == ARRAY) ? STRINGIFY(ARRAY) :
                // (this->t == MAP) ? STRINGIFY(MAP) :
                // (this->t == OBJECT) ? STRINGIFY(OBJECT) :
                "null"
            )
            + "]"
        ;
        return result;
    }
};
