//
// Created by Lech on 07.12.2021.
//

#include <iostream>
#include <filesystem>
#include <fstream>
#include <chrono>
#include <vector>
#include <libpson.hpp>

using namespace std;

int main(){
    int exit_code = 0;
    try{
        for(auto& entry : filesystem::directory_iterator("./examples")){
            ifstream file;
            file.open(entry.path());
            string content;
            while(file){
                content += (char)file.get();
            }
            file.close();
            try{
                auto time_before = chrono::high_resolution_clock::now();
                auto obj = PSON::parse(content.c_str());
                auto time_after = chrono::high_resolution_clock::now();
                if(entry.path().filename().c_str()[0] != '.')
                    cout << "Run " << entry.path().filename().string() << " in "
                         << duration_cast<chrono::duration<double>>(time_after - time_before).count() * 1000
                         << " ms \n\tResult: " << obj << '\n';
                else
                    cout << "Run " << entry.path().filename().string() << " in "
                         << duration_cast<chrono::duration<double>>(time_after - time_before).count() * 1000
                         << " ms" << '\n';
            }
            catch(PSON::SyntaxError& e){
                cout << "Run " << entry.path().filename().string()
                     << "\n\tFailed!" << '\n';
                exit_code = 1;
            }
        }
    }
    catch(filesystem::filesystem_error& e){
        cout << "Filesystem error!\n";
        exit(1);
    }
    vector<const char*> bad_tests = {"(", "(\')\'", "\'", ")", "\'\'"};
    for(auto& i :bad_tests){
        try{
            PSON::parse(i);
            cout << "\"" << i << "\" Failed!\n";
            exit_code = 1;
        }
        catch(PSON::SyntaxError& e){
            cout << "\"" << i << "\" Passed!\n";
        }
    }
    return exit_code;
}
