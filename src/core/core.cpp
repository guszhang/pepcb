#include "core.h"

#include <fstream>
#include <iostream>

static std::string PEPCB::FS::readFromFile(std::string _Filename){
    std::string full_text="";
    std::string line_buffer;
    std::ifstream in_stream(_Filename);
    if(in_stream.is_open()){
        while(getline(in_stream, line_buffer)){
            full_text.append(line_buffer);
        }
    }
    else{
        std::cerr << "Error: Cannot open file " << _Filename <<std::endl;
        exit(-1);
    }
    in_stream.close();
    return full_text;
}