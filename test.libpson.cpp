//
// Created by Lech on 07.12.2021.
//

#include <iostream>
#include <filesystem>
#include <fstream>
#include <chrono>
#include <libpson.hpp>

using namespace std;

int main(){
    try{
        for(auto& entry : filesystem::directory_iterator("./examples")){
            ifstream file;
            file.open(entry.path());
            string content;
            while(file){
                content += (char)file.get();
            }
            file.close();
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
    }
    catch(filesystem::filesystem_error& e){
        cout << "Filesystem error!\n";
    }
    return 0;
}
