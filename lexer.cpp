#include "lexer.hpp"
#include <cmath>

static inline bool is_upper(std::string s){
    for(auto& i: s){
        if(i != std::toupper(i))
            return false;
    }
    return true;
}

static inline bool is_float(std::string s){
    for(auto& i: s){
        if(i == '.')
            return true;
    }
    return false;
}

static inline bool is_ws(char c){
    return c == ' ' or c == '\n' or c == '\r' or c == '\t';
}

namespace PSON {

void Lexer::skip(){
    char next_char = this->is->get();
    std::string buffer;
    // Read to the end of the token
    if((next_char >= 'A' and next_char <= 'Z') or (next_char >= 'a' and next_char <= 'z') or next_char == '_'){
        while((next_char >= 'A' and next_char <= 'Z') or (next_char >= 'a' and next_char <= 'z') or (next_char >= '0' and next_char <= '9') or next_char == '_'){
            buffer.push_back(next_char);
            next_char = this->is->get();
            if(this->is->eof()) goto after_putback;
        }
        this->is->putback(next_char);
    } else if(next_char >= '0' and next_char <= '9'){
        while((next_char >= '0' and next_char <= '9') or next_char == '.'){
            buffer.push_back(next_char);
            next_char = this->is->get();
            if(this->is->eof()) goto after_putback;
        }
        this->is->putback(next_char);
    } else if(next_char == '"'){
        do{
            buffer.push_back(next_char);
            if(next_char == '\\'){
                next_char = this->is->get();
                buffer.push_back(next_char);
            }
            next_char = this->is->get();
            if(this->is->eof()) throw std::exception();
        }while(next_char != '"');
        this->is->get();
    } else {
        buffer.push_back(next_char);
    }
    after_putback:;
    if(is_ws(next_char)){
        // Skip whitespace
        while(is_ws(next_char)){
            next_char = this->is->get();
        }
        this->is->putback(next_char);
    }
    { // Recognize token type and store value
        if(buffer == "nan"){
            this->next_token.t = Symbol::FP;
            this->next_token.v.FP = NAN;
        }else if(buffer == "inf"){
            this->next_token.t = Symbol::FP;
            this->next_token.v.FP = INFINITY;
        }else if(buffer == "null"){
            this->next_token.t = Symbol::NUL;
        }else if(buffer == "-"){
            this->next_token.t = Symbol::UN_MIN;
        }else if(buffer == "="){
            this->next_token.t = Symbol::EQ;
        }else if(buffer == "("){
            this->next_token.t = Symbol::L_RND;
        }else if(buffer == ")"){
            this->next_token.t = Symbol::R_RND;
        }else if(buffer == "["){
            this->next_token.t = Symbol::L_SQ;
        }else if(buffer == "]"){
            this->next_token.t = Symbol::R_SQ;
        }else{
            const auto at0 = buffer[0];
            if((at0 >= 'A' and at0 <= 'Z') or (at0 >= 'a' and at0 <= 'z') or at0 == '_'){
                if(is_upper(buffer)){
                    this->next_token.t = Symbol::UC_ID;
                    this->next_token.v.UC_ID = new std::string(buffer);
                }else{
                    this->next_token.t = Symbol::NUC_ID;
                    this->next_token.v.NUC_ID = new std::string(buffer);
                }
            }else if(at0 >= '0' and at0 <= '9'){
                if(is_float(buffer)){
                    this->next_token.t = Symbol::FP;
                    this->next_token.v.FP = std::stof(buffer);
                }else{
                    this->next_token.t = Symbol::INT;
                    this->next_token.v.INT = std::stoi(buffer);
                }
            }else if(at0 == '"'){
                this->next_token.t = Symbol::STR;
                // TODO unescape this string
                this->next_token.v.STR = new std::string(buffer.substr(1));
            }
        }
    }
}

Lexer::Lexer(std::istream* is){
    this->is = is;
    this->skip();
}

Symbol Lexer::preview_next(){
    return this->next_token;
}

Symbol Lexer::next(){
    Symbol retval = this->next_token;
    this->skip();
    return retval;
}

bool Lexer::eof(){
    return this->is->eof();
}

};
