#include <libpson.hpp>
#include <stack>
#include <string>
#include <cstring>
#include <sstream>

extern "C++" {
using std::string;

static char* string_copy(const char *from) {
    if(from == nullptr)
        return nullptr;
    unsigned size = strlen(from)+1;
    if(size == 1)
        return nullptr;
    char *to = new(std::nothrow) char[size];
    #ifndef __STDC_SECURE_LIB__
    strcpy(to, (const char*)from);
    #else
    strcpy_s(to, size, (const char*)from);
    #endif
    return to;
}

static char* string_escape(const char *from) {
    if(from == nullptr)
        return nullptr;
    unsigned size = 1;
    for(unsigned i = 0; from[i]; i++){
        switch (from[i]) {
            case '\\':
            case '\n':
            case '\r':
            case '\t':
            case '\a':
                size+=2;
                break;
            default:
                size++;
                break;
        }
    }
    if(size == 1)
        return nullptr;
    char* to = new(std::nothrow) char[size];
    for(unsigned ti=0,fi=0; from[fi]; ti++,fi++){
        switch (from[fi]) {
            case '\\':
                to[ti] = '\\';
                to[++ti] = '\\';
                break;
            case '\'':
                to[ti] = '\\';
                to[++ti] = '\'';
                break;
            case '\n':
                to[ti] = '\\';
                to[++ti] = 'n';
                break;
            case '\r':
                to[ti] = '\\';
                to[++ti] = 'r';
                break;
            case '\t':
                to[ti] = '\\';
                to[++ti] = 't';
                break;
            case '\a':
                to[ti] = '\\';
                to[++ti] = 'a';
                break;
            default:
                to[ti] = from[fi];
                break;
        }
    }
    to[size-1] = 0;
    return to;
}

PSON::Array::Array() = default;

PSON::Array::Array(unsigned int size) {
    this->size = size;
    this->value = new(std::nothrow) PSON::Object[size];
}

PSON::Object::Object() = default;

PSON::Object::Object(const char *name, int value) {
    this->type = INT;
    this->name = string_copy(name);
    this->i = value;
}

PSON::Object::Object(const char *name, float value) {
    this->type = FLOAT;
    this->name = string_copy(name);
    this->f = value;
}

PSON::Object::Object(const char *name, char* value) {
    this->type = STRING;
    this->name = string_copy(name);
    this->str = string_copy(value);
}

PSON::Object::Object(const char *name, Array* value) {
    this->type = ARRAY;
    this->name = string_copy(name);
    this->array = value;
}

PSON::Object PSON::parse(const char *src) {
    if(!PSON::verify(src)){
        throw PSON::SyntaxError();
    }
    enum state_t : std::uint8_t{
        PARSER_LIST,
        PARSER_ID,
        PARSER_VALUE,
        PARSER_TYPE
    }state = PARSER_LIST;
    std::stack<PSON::Object> objects;
    std::stack<unsigned> sizes;
    sizes.push(0);
    std::stack<string> parser_id;
    parser_id.push("__root__");
    parser_id.push("");
    string parser_value;

    for(unsigned i = 0; src[i]; i++){
        switch(state){
            case PARSER_LIST:
                if(
                        ('a' <= src[i] and src[i] <= 'z')
                        or ('A' <= src[i] and src[i] <= 'Z')
                        or src[i] == '_'
                        ){
                    state = PARSER_ID;
                    parser_id.top() = src[i];
                }
                else if(src[i] == '\'') {
                    state = PARSER_VALUE;
                    parser_value="";
                }
                else if(src[i] == '('){
                    parser_id.push("");
                    sizes.push(0);
                }
                else if(src[i] == ')'){
                    parser_id.pop();
                    PSON::Object up(parser_id.top().c_str(), new Array(sizes.top()));
                    for(unsigned j = 1; j <= sizes.top(); j++){
                        up.array->value[sizes.top() - j] = objects.top();
                        objects.pop();
                    }
                    objects.push(up);
                    parser_id.top() = "";
                    sizes.pop();
                    sizes.top()++;
                }
                break;
            case PARSER_ID:
                if(
                        ('a'<=src[i] and src[i]<='z')
                        or ('A'<=src[i] and src[i]<='Z')
                        or src[i]=='_'
                        )
                    parser_id.top() += src[i];
                else{
                    state = PARSER_LIST;
                    i--;
                }
                break;
            case PARSER_VALUE:
                if(src[i] == '\'')
                    state = PARSER_TYPE;
                else if(src[i] == '\\'){
                    i++;
                    switch(src[i]){
                        case '\'':
                            parser_value += '\'';
                            break;
                        case '\\':
                            parser_value += '\\';
                            break;
                        case 'n':
                            parser_value += '\n';
                            break;
                        case 'r':
                            parser_value += '\r';
                            break;
                        case 't':
                            parser_value += '\t';
                            break;
                        case 'a':
                            parser_value += '\a';
                            break;
                    }
                }else{
                    parser_value += src[i];
                }
                break;
            case PARSER_TYPE:
                switch (src[i]) {
                    case Object::INT:
                        objects.push(PSON::Object(parser_id.top().c_str(), std::stoi(parser_value)));
                        break;
                    case Object::FLOAT:
                        objects.push(PSON::Object(parser_id.top().c_str(), std::stof(parser_value)));
                        break;
                    case Object::STRING:
                        objects.push(PSON::Object(parser_id.top().c_str(), (char*)parser_value.c_str()));
                        break;
                }
                parser_id.top() = "";
                sizes.top()++;
                state = PARSER_LIST;
                break;
        }
    }
    parser_id.pop();
    PSON::Object up(parser_id.top().c_str(), new Array(sizes.top()));
    for(unsigned i = 1; i <= sizes.top(); i++){
        up.array->value[sizes.top() - i] = objects.top();
        objects.pop();
    }
    sizes.pop();
    return up;
}

const char *PSON::SyntaxError::what() const {
    return "PSON::SyntaxError";
}

bool PSON::verify(const char* src){
    unsigned len = strlen(src);
    int depth = 0;
    bool val_def = false;
    for(int i=0; i<len; i++){
        if(!val_def){
            if(src[i] == '(')
                depth++;
            else if(src[i] == ')'){
                if(--depth < 0)
                    return false;
            }
        }
        else if(src[i] == '\\'){
            i++;
        }
        if(src[i] == '\''){
            if(val_def){
                switch (src[++i]) {
                    case PSON::Object::STRING:
                    case PSON::Object::INT:
                    case PSON::Object::FLOAT:
                        val_def = false;
                        break;
                    default:
                        return false;
                }
            }else val_def = true;
        }
    }
    if(depth or val_def)
        return false;
    return true;
}

PSON::Object::operator std::string() const {
    std::stringstream ss;
    if(this->name == nullptr || strcmp(this->name,"__root__") != 0){
        if(this->name)
            ss << this->name;
        if(this->type == PSON::Object::INT)
            ss << '\'' << this->i << '\'' << (char)this->type;
        else if(this->type == PSON::Object::FLOAT)
            ss << "\'" << this->f << "\'" << (char)this->type;
        else if(this->type == PSON::Object::STRING){
            if(this->str){
                char* tmp = string_escape(this->str);
                ss << '\'' << (string)tmp << '\'' << (char)this->type;
                delete[] tmp;
            }else{
                ss << "\'\'" << (char)this->type;
            }
        }
        else if(this->type == PSON::Object::ARRAY){
            ss << '(';
            for(unsigned i=0; i < this->array->size - 1; i++){
                ss << this->array->value[i] << ' ';
            }
            ss << this->array->value[this->array->size - 1] << ')';
        }
    }else{
        for(unsigned i=0; i < this->array->size - 1; i++){
            ss << this->array->value[i] << ' ';
        }
        ss << this->array->value[this->array->size - 1];
    }
    return ss.str();
}

std::ostream& operator<<(std::ostream& os, PSON::Object obj){
    return os<<(string)obj;
}

}

