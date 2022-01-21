#include "lexer.hpp"
#include <cmath>
#include <iostream>

static inline bool is_upper(const std::string& s){
    return std::all_of(s.begin(), s.end(), [](char c){return 'A' <= c and c <= 'Z';});
}

static inline bool is_float(const std::string& s){
    return std::any_of(s.begin(), s.end(), [](char c){return c == '.';});
}

static inline bool is_ws(char c){
    return c == ' ' or c == '\n' or c == '\r' or c == '\t';
}

namespace PSON {

bool Lexer::skip(){
    char next_char = (char)(this->is->get());
    if(next_char == -1){
        is = nullptr;
        return false;
    }
    std::string buffer;
    // Read to the end of the token
    if((next_char >= 'A' and next_char <= 'Z') or (next_char >= 'a' and next_char <= 'z') or next_char == '_'){
        while((next_char >= 'A' and next_char <= 'Z') or (next_char >= 'a' and next_char <= 'z') or (next_char >= '0' and next_char <= '9') or next_char == '_'){
            buffer.push_back(next_char);
            next_char = (char)(this->is->get());
            if(this->is->eof()) goto after_putback;
        }
        this->is->putback(next_char);
    } else if(next_char >= '0' and next_char <= '9'){
        while((next_char >= '0' and next_char <= '9') or next_char == '.'){
            buffer.push_back(next_char);
            next_char = (char)(this->is->get());
            if(this->is->eof()) goto after_putback;
        }
        this->is->putback(next_char);
    } else if(next_char == '"'){
        do{
            buffer.push_back(next_char);
            next_char = (char)(this->is->get());
            if(this->is->eof()) goto after_putback;
        }while(next_char != '"');
    } else {
        buffer.push_back(next_char);
    }
    after_putback:;
    if(is_ws(next_char)){
        // Skip whitespace
        while(is_ws(next_char) && !is->eof()){
            next_char = (char)(this->is->get());
        }
        if(!is_ws(next_char))
            this->is->putback(next_char);
    }
    { // Recognize token type and store value
        if(buffer == "nan"){
            next_token.t = Symbol::FP;
            next_token.v.FP = NAN;
        }else if(buffer == "inf"){
            next_token.t = Symbol::FP;
            next_token.v.FP = INFINITY;
        }else if(buffer == "null"){
            next_token.t = Symbol::NUL;
        }else if(buffer == "-"){
            next_token.t = Symbol::UN_MIN;
        }else if(buffer == "="){
            next_token.t = Symbol::EQ;
        }else if(buffer == "("){
            next_token.t = Symbol::L_RND;
        }else if(buffer == ")"){
            next_token.t = Symbol::R_RND;
        }else if(buffer == "["){
            next_token.t = Symbol::L_SQ;
        }else if(buffer == "]"){
            next_token.t = Symbol::R_SQ;
        }else{
            const auto at0 = buffer[0];
            if((at0 >= 'A' and at0 <= 'Z') or (at0 >= 'a' and at0 <= 'z') or at0 == '_'){
                if(is_upper(buffer)){
                    next_token.t = Symbol::UC_ID;
                    next_token.v.UC_ID = new std::string(buffer);
                }else{
                    next_token.t = Symbol::NUC_ID;
                    next_token.v.NUC_ID = new std::string(buffer);
                }
            }else if(at0 >= '0' and at0 <= '9'){
                if(is_float(buffer)){
                    next_token.t = Symbol::FP;
                    next_token.v.FP = std::stof(buffer);
                }else{
                    next_token.t = Symbol::INT;
                    next_token.v.INT = std::stoi(buffer);
                }
            }else if(at0 == '"'){
                next_token.t = Symbol::STR;
                // TODO unescape this string
                next_token.v.STR = new std::string(buffer.substr(1));
            }else{
                return false;
            }
        }
    }
    return true;
}

Lexer::Lexer(std::istream* is){
    this->is = is;
    char c = (char)(this->is->get());
    if(is_ws(c)){
        while(is_ws(c)){
            c = (char)(this->is->get());
        }
    }
    this->is->putback(c);
    this->skip();
}

Symbol Lexer::preview_next(){
    return this->next_token;
}

Symbol Lexer::next(){
    if(!is || is->eof()){
        return next_token;
    }
    Symbol retval = this->next_token;
    while(!skip() && is && !this->is->eof());
    return retval;
}

bool Lexer::eof() const{
    return !is || is->eof();
}

};
