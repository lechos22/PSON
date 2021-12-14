#include <PSON.hpp>
#include <string>
#include <cstring>

using std::string;
using std::ostream;
using std::nothrow;

PSON::Array::Array(int size) {
    resize(size);
}

PSON::Object& PSON::Object::operator= (const int& value){
    type = INT;
    val.i = value;
    return *this;
}

PSON::Object& PSON::Object::operator= (const float& value){
    type = FLOAT;
    val.f = value;
    return *this;
}

PSON::Object& PSON::Object::operator= (const double& value){
    type = FLOAT;
    val.f = value;
    return *this;
}

PSON::Object& PSON::Object::operator= (const string& value){
    type = STRING;
    val.str = new(nothrow) string(value);
    return *this;
}

PSON::Object& PSON::Object::operator= (const char*& value){
    type = STRING;
    val.str = new(nothrow) string(value);
    return *this;
}

PSON::Object& PSON::Object::operator= (PSON::Map* value){
    type = MAP;
    val.map = value;
    return *this;
}

PSON::Object& PSON::Object::operator= (PSON::Array* value){
    type = ARRAY;
    val.array = value;
    return *this;
}

PSON::Object& PSON::Object::operator= (const nullptr_t& value){
    type = NONE;
    val.none = value;
    return *this;
}

PSON::Object& PSON::Object::operator[] (int i) const{
    return val.array->operator[](i);
}

PSON::Object &PSON::Object::operator[] (const string& k) const {
    return val.map->operator[](k);
}

PSON::Object::Object() {
    type = NONE;
    val.none = nullptr;
}

PSON::Object::~Object() {
    switch (type) {
        case STRING:
            delete val.str;
            break;
        case MAP: // may cause segfault :)
            delete val.map;
            break;
        case ARRAY:
            delete val.array;
            break;
        default:
            break;
    }
}

const char *PSON::SyntaxError::what() const noexcept{
    return "PSON::SyntaxError";
}

bool PSON::verify(const char* src){
    return src;
}

ostream& operator<< (ostream& ostream1, PSON::Object& obj){
    switch (obj.type) {
        case PSON::Object::NONE:
            ostream1 << nullptr;
            break;
        case PSON::Object::INT:
            ostream1 << obj.val.i;
            break;
        case PSON::Object::FLOAT:
            ostream1 << obj.val.f;
            break;
        case PSON::Object::STRING:
            ostream1 << * obj.val.str;
            break;
        default:
            break;
    }
    return ostream1;
}
