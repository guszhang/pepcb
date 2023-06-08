#include "core.h"

#include <fstream>
#include <iostream>
#include <cmath>

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

PEPCB::Base::TVertex PEPCB::Base::rotate(PEPCB::Base::TVertex _v, PEPCB::Base::TAngle _angle)
{
    double angle = _angle / 1800 * M_PI;
    double cos_a = cos(angle);
    double sin_a = sin(angle);
    PEPCB::Base::TVertex new_v;
    new_v.X = (PEPCB::Base::TDim)(_v.X * cos_a) - (PEPCB::Base::TDim)(_v.Y * sin_a);
    new_v.Y = (PEPCB::Base::TDim)(_v.X * sin_a) - (PEPCB::Base::TDim)(_v.Y * cos_a);
    return new_v;
}

bool isCopperLayer(PEPCB::Base::ELayer _layer)
{
    switch (_layer)
    {
    case PEPCB::Base::F_CU... PEPCB::Base::IN30_CU:
    case PEPCB::Base::B_CU:
        return true;
        break;
    default:
        return false;
        break;
    }
}
void PEPCB::Base::TCopper::addPolygon(PEPCB::Base::ELayer _layer, PEPCB::Base::TPolygon _polygon)
{
    if (isCopperLayer(_layer))
    {
        if (this->polygon_list.find(_layer) == this->polygon_list.end())
        {
            std::vector<PEPCB::Base::TPolygon> polygons;
            polygons.push_back(_polygon);
            this->polygon_list.insert(std::pair<PEPCB::Base::ELayer, std::vector<PEPCB::Base::TPolygon>>(_layer, polygons));
        }
        else
        {
            this->polygon_list[_layer].push_back(_polygon);
        }
    }
}

void PEPCB::Base::TGeometry::log(std::string name)
{
    std::cout << "Geometry: " << name << std::endl;
    std::cout << "Outer Vertices: " << this->outer_vertex_list.size() << std::endl;
    for (auto it = this->outer_vertex_list.begin(); it < this->outer_vertex_list.end(); it++)
    {
        std::cout << "(" << it->X << ", " << it->Y << ")" << std::endl;
    }
    std::cout << "Holes: " << this->inner_vertex_list_list.size() << std::endl;
    for (auto it = this->inner_vertex_list_list.begin(); it < this->inner_vertex_list_list.end(); it++)
    {
        std::cout << "Inner Vertices: " << it->size() << std::endl;
        for (auto it_vertex = it->begin(); it_vertex < it->end(); it_vertex++)
        {
            std::cout << "(" << it_vertex->X << ", " << it_vertex->Y << ")";
        }
    }
}