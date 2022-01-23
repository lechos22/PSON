#include <sstream>
#include "PSON.hpp"

namespace PSON {

    Object::Object(std::nullptr_t val) {
        t = NUL;
        v.NUL = val;
    }

    Object::Object(std::string *val) {
        t = STR;
        v.STR = val;
    }

    Object::Object(int val) {
        t = INT;
        v.INT = val;
    }

    Object::Object(float val) {
        t = FP;
        v.FP = val;
    }

    Object::Object(Array *val) {
        t = ARRAY;
        v.ARRAY = val;
    }

    Object::Object(Map *val) {
        t = MAP;
        v.MAP = val;
    }

    Object::operator std::string() const {
        std::stringstream ss;
        switch (t) {
            case INT:
                ss << v.INT;
                return ss.str();
            case NUL:
                return "null";
            case STR:
                return "\"" + *(v.STR) + "\""; //TODO escape
            case FP:
                ss << v.FP;
                return ss.str();
            case ARRAY:
                for (auto &i: *(v.ARRAY))
                    ss << (std::string) (*i) << ' ';
                return "[" + ss.str().substr(0, ss.str().length() - 1) + "]";
            case MAP:
                for (auto &i: *(v.MAP))
                    ss << i.first << "=" << (std::string) (*(i.second)) << ' ';
                return "(" + ss.str().substr(0, ss.str().length() - 1) + ")";
            default:
                throw std::exception();
        }
    }

    Object::~Object() {
        switch (t) {
            case STR: delete(v.STR); break;
            case ARRAY: delete(v.ARRAY); break;
            case MAP: delete(v.MAP); break;
            default: break;
        }
    }

}
