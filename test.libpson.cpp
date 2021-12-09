//
// Created by Lech on 07.12.2021.
//

#include <iostream>
#include <filesystem>
#include <fstream>
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
            auto obj = PSON::Object(content.c_str());
            cout << obj << '\n';
        }
    }
    catch(filesystem::filesystem_error& e){
        cout<<"filesystem error!\n";
    }
    return 0;
}
