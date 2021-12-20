#include <PSON.hpp>
#include <string>
#include <stack>
#include <iostream>

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
    val.str = new(nothrow) string;
    *(val.str) = value;
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
        case MAP:
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
    using std::stack;
    stack<char> mode_stack;
    for(int i=0; src[i]; i++){
        if(mode_stack.empty() || mode_stack.top() != '\''){
            if(src[i] == '\'')
                mode_stack.push('\'');
            else if(src[i] == '(')
                mode_stack.push('(');
            else if(src[i] == '[')
                mode_stack.push('[');
            else if(src[i] == ')'){
                if(mode_stack.top() == '(')
                    mode_stack.pop();
                else
                    return false;
            }
            else if(src[i] == ']'){
                if(mode_stack.top() == '[')
                    mode_stack.pop();
                else
                    return false;
            }
        }else{
            if(src[i] == '\\')
                i++;
            else if(src[i] == '\'')
                mode_stack.pop();
        }
    }
    return mode_stack.empty();
}

PSON::Object PSON::parseArray(const char *src, bool verify){
    using std::stack;
    if(verify && !PSON::verify(src))
        throw PSON::SyntaxError();
    PSON::Object result;
    result = new PSON::Array;
    stack<char> mode_stack;
    mode_stack.push('[');
    stack<PSON::Object*> context_stack;
    context_stack.push(&result);
    string name;
    string val_tmp;
    for(int i=0; src[i]; i++){
        if(mode_stack.top() != '\''){
            if(src[i] == '\''){
                if(mode_stack.top() == '['){
                    context_stack.top()->val.array->push_back(PSON::Object());
                    context_stack.push(&(context_stack.top()->val.array->back()));
                }
                else if(mode_stack.top() == '('){
                    context_stack.top()->operator[](name) = nullptr;
                    context_stack.push(&(context_stack.top()->operator[](name)));
                }
                mode_stack.push('\'');
            }
            else if(src[i] == '('){
                if(mode_stack.top() == '['){
                    context_stack.top()->val.array->push_back(PSON::Object());
                    context_stack.push(&(context_stack.top()->val.array->back()));
                    *(context_stack.top()) = (new PSON::Array(0));
                }
                else if(mode_stack.top() == '('){
                    context_stack.top()->operator[](name) = new PSON::Array;
                    context_stack.push(&(context_stack.top()->operator[](name)));
                }
                mode_stack.push('(');
            }
            else if(src[i] == '['){
                if(mode_stack.top() == '['){
                    context_stack.top()->val.array->push_back(PSON::Object());
                    context_stack.push(&(context_stack.top()->val.array->back()));
                    *(context_stack.top()) = (new PSON::Map);
                }
                else if(mode_stack.top() == '('){
                    context_stack.top()->operator[](name) = new PSON::Map;
                    context_stack.push(&(context_stack.top()->operator[](name)));
                }
                mode_stack.push('[');
            }
            else if(src[i] == ')' or src[i] == ']') {
                context_stack.pop();
                mode_stack.pop();
            }
            else if(
                    (src[i] >= 'A' and src[i] <= 'Z')
                    or (src[i] >= 'a' and src[i] <= 'z')
                    or src[i] == '_'
                    )
                name += src[i];
            else if(src[i] == ' ')
                name.clear();
        }else{
            if(src[i] == '\\')
                i++;
            else if(src[i] == '\''){
                mode_stack.pop();
                switch (src[++i]) {
                    case Object::INT:
                        *(context_stack.top()) = std::stoi(val_tmp);
                        break;
                    case Object::FLOAT:
                        *(context_stack.top()) = std::stof(val_tmp);
                        break;
                    case Object::STRING:
                        *(context_stack.top()) = val_tmp;
                        break;
                }
                val_tmp.clear();
                context_stack.pop();
            }
            else
                val_tmp += src[i];
        }
    }
    return result;
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
