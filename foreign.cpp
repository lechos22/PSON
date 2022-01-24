#include "foreign.hpp"
#include "PSON_PY.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include <sstream>

extern void* parse(const char* s){
    std::stringstream ss;
    ss << s;
    PSON::Lexer lexer(&ss);
    PSON::Parser parser(lexer);
    return parser.result;
}

extern char get_type(void* obj){
    auto* object = (PSON::Object*)obj;
    switch(object->t){
        case PSON::Object::INT: return 'i';
        case PSON::Object::FP: return 'f';
        case PSON::Object::NUL: return '0';
        case PSON::Object::STR: return 'S';
        case PSON::Object::ARRAY: return 'A';
        case PSON::Object::MAP: return 'M';
        default: return '0';
    }
}

extern int get_int(void* obj){
    return ((PSON::Object*)obj)->v.INT;
}

extern float get_float(void* obj){
    return ((PSON::Object*)obj)->v.FP;
}

extern const char* get_str(void* obj){
    return ((PSON::Object*)obj)->v.STR->c_str();
}

extern unsigned get_len(void* obj){
    return (unsigned)(((PSON::Object*)obj)->v.ARRAY->size());
}

extern void* get_at(void* obj, unsigned idx){
    return (void*)(((PSON::Object*)obj)->v.ARRAY->at(idx));
}

extern void* get_map_iter(void* obj){
    auto* object = (PSON::Object*)obj;
    auto iter = new std::map<std::string, PSON::Object*>::iterator;
    *iter = object->v.MAP->begin();
    return iter;
}

extern const char* get_map_next(void* iter){
    (*((std::map<std::string, PSON::Object*>::iterator*)(iter)))++;
    return (*((std::map<std::string, PSON::Object*>::iterator*)iter))->first.c_str();
}

extern bool get_map_iter_alive(void* obj, void* iter){
    return (*((std::map<std::string, PSON::Object*>::iterator*)iter)) == ((PSON::Object*)(obj))->v.MAP->end();
}

extern bool get_map_contains(void* obj, const char* idx){
    auto* object = (PSON::Object*)obj;
    return object->v.MAP->contains(idx);
}

extern void* get_map_at(void* obj, const char* idx){
    auto* object = (PSON::Object*)obj;
    return (void*)(object->v.MAP->at(idx));
}

extern void destroy_iter(void* iter){
    delete (std::map<std::string, PSON::Object*>::iterator*)iter;
}

extern void destroy(void* obj){
    delete (PSON::Object*)obj;
}
