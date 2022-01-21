#include <string>
#include <sstream>
#include <iostream>
#include "../lexer.hpp"

using namespace std;

struct Error: public exception{
    string msg;
    explicit Error(string msg){
        this->msg = std::move(msg);
    }
    [[nodiscard]] const char* what() const noexcept override{
        return msg.c_str();
    }
};

static inline void assertEqual(auto left, auto right){
    if(left != right)
        throw Error("Failed to assert: „" + left + "“ is not equal to „" + right + "“\n");
}

void test_0() {
    stringstream input;
    input << R"([1 "2" 3.14])";
    PSON::Lexer lexer(&input);
    stringstream output;
    while(!(lexer.eof()))
        output << (string)lexer.next();
    assertEqual(output.str(), R"([L_SQ null][INT 1][STR "2"][FP 3.14][R_SQ null])");
}

void test_1() {
    stringstream input;
    input << R"((a=2 b=2 pi=3.14))";
    PSON::Lexer lexer(&input);
    stringstream output;
    while(!(lexer.eof()))
        output << (string) lexer.next();
    assertEqual(output.str(), R"([L_RND null][NUC_ID "a"][EQ null][INT 2][NUC_ID "b"][EQ null][INT 2][NUC_ID "pi"][EQ null][FP 3.14][R_RND null])");
}

void test_2() {
    stringstream input;
    input << R"("2")";
    PSON::Lexer lexer(&input);
    stringstream output;
    while(!(lexer.eof()))
        output << (string)lexer.next();
    assertEqual(output.str(), R"([STR "2"])");
}

int main() {
    try{
        test_0();
        test_1();
        test_2();
    }catch(Error& e){
        cerr << e.what();
        exit(1);
    }
}
