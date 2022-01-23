#include <string>
#include <sstream>
#include <iostream>
#include "../lexer.hpp"
#include "../parser.hpp"

using namespace std;

struct Error : public exception {
    string msg;

    explicit Error(string msg) {
        this->msg = std::move(msg);
    }

    const char *what() const noexcept override {
        return msg.c_str();
    }
};

static inline void assertEqual(auto left, auto right) {
    if (left != right)
        throw Error("Failed to assert: „" + left + "“ is not equal to „" + right + "“\n");
}

void test_0() {
    stringstream input;
    input << R"([1 "2" [3.14 4 (a=5 b=6)]])";
    PSON::Lexer lexer(&input);
    PSON::Parser parser(lexer);
    assertEqual(input.str(), (string) (*(parser.result)));
}

void test_1() {
    stringstream input;
    input << R"([1 "2" [3.14 4 (a=5 b=6)]])";
    PSON::Lexer lexer(&input);
    PSON::Parser parser(lexer);
    assertEqual(input.str(), (string) (*(parser.result)));
}

void test_2() {
    stringstream input;
    input << R"((a=ALFA b=BETA))";
    PSON::Lexer lexer(&input);
    PSON::Parser parser(lexer);
    assertEqual(input.str(), (string) (*(parser.result)));
}

int main() {
    try {
        test_0();
    } catch (Error &e) {
        cerr << e.what();
        exit(1);
    }
}
