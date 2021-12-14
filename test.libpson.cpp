//
// Created by Lech on 07.12.2021.
//

#include <iostream>
#include <PSON.hpp>

using namespace std;

int main(){
    int exit_code = 0;
    PSON::Array array(3);
    array[0] = "1";
    array[1] = 2;
    array[2] = new PSON::Array(2);
    array[2][0] = 3.0;
    array[2][1] = new PSON::Map;
    array[2][1]["four"] = "4";
    cout << array[0] << endl;
    cout << array[1] << endl;
    cout << array[2][0] << endl;
    cout << array[2][1]["four"] << endl;
    return exit_code;
}
