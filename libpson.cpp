#include <libpson.hpp>
#include <stack>
#include <string>
#include <cstring>

extern "C++" {

static char* string_copy(const char *from) {
    if(from!=nullptr){
        unsigned size = strlen(from)+1;
        char *to = new(std::nothrow) char[size];
        //for(unsigned i = 0; i < size; i++)
        //    to[i] = from[i];
        strcpy(to, size, from);
        return to;
    }
    return nullptr;
}

static char* string_escape(const char *from) {
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

PSON::Object::Object(const char *src) {
    enum state_t : std::uint8_t{
        PARSER_LIST,
        PARSER_ID,
        PARSER_VALUE,
        PARSER_TYPE
    }state = PARSER_LIST;
    std::stack<PSON::Object> objects;
    std::stack<unsigned> sizes;
    sizes.push(0);
    std::stack<std::string> parser_id;
    parser_id.push("__root__");
    parser_id.push("");
    std::string parser_value;

    for(unsigned i = 0; src[i]; i++){
        switch(state){
            case PARSER_LIST:
                if('a'<=src[i] && src[i]<='z' || 'A'<=src[i] && src[i]<='Z' || src[i]=='_') {
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
                if('a'<=src[i] && src[i]<='z' || 'A'<=src[i] && src[i]<='Z' || src[i]=='_')
                    parser_id.top() += src[i];
                else{
                    state = PARSER_LIST;
                    i--;
                }
                break;
            case PARSER_VALUE:
                if(src[i]=='\'')
                    state = PARSER_TYPE;
                else if(src[i]=='\\'){
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
                    case INT:
                        objects.push(PSON::Object(parser_id.top().c_str(), std::stoi(parser_value)));
                        break;
                    case FLOAT:
                        objects.push(PSON::Object(parser_id.top().c_str(), std::stof(parser_value)));
                        break;
                    case STRING:
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
    *this=up;
    sizes.pop();
}

std::ostream& operator<<(std::ostream& os, PSON::Object obj) {
    if(strcmp(obj.name,"__root__") != 0){
        os << obj.name;
        if(obj.type == PSON::Object::INT)
            os << '\'' << obj.i << '\'' << (char)obj.type;
        else if(obj.type == PSON::Object::FLOAT)
            os << '\'' << obj.f << '\'' << (char)obj.type;
        else if(obj.type == PSON::Object::STRING){
            char* tmp = string_escape(obj.str);
            os << '\'' << tmp << '\'' << (char)obj.type;
            delete[] tmp;
        }
        else if(obj.type == PSON::Object::ARRAY){
            os << '(';
            for(unsigned i=0; i < obj.array->size - 1; i++){
                os << obj.array->value[i] << ' ';
            }
            os << obj.array->value[obj.array->size - 1] << ')';
        }
    }else{
        for(unsigned i=0; i < obj.array->size - 1; i++){
            os << obj.array->value[i] << ' ';
        }
        os << obj.array->value[obj.array->size - 1];
    }
    return os;
}

}
