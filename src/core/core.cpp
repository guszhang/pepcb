#include "core.h"

#include <fstream>
#include <iostream>

std::string PEPCB::FS::readFromFile(std::string _Filename)
{
    std::string full_text = "";
    std::string line_buffer;
    std::ifstream in_stream(_Filename);
    if (in_stream.is_open())
    {
        while (getline(in_stream, line_buffer))
        {
            full_text.append(line_buffer);
            full_text.push_back('\n');
        }
    }
    else
    {
        std::cerr << "Error: Cannot open file " << _Filename << std::endl;
        exit(-1);
    }
    in_stream.close();
    return full_text;
}

void PEPCB::Base::TFootprint::insertGeometry(ELayer _layer, TGeometry _geometry)
{
    if (this->layer_list.find(_layer) == this->layer_list.end())
    {
        std::vector<TGeometry> geometry_list;
        this->layer_list.insert(std::pair<ELayer, std::vector<TGeometry>>(_layer, geometry_list));
    }
    this->layer_list[_layer].push_back(_geometry);
}
