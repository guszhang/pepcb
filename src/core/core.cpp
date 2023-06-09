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

PEPCB::Base::TVertex PEPCB::Base::rotate(PEPCB::Base::TVertex _v, double _angle)
{
    // std::cout << "Angle of pad: " << angle << std::endl;

    double cos_a = cos(_angle);
    double sin_a = sin(_angle);
    PEPCB::Base::TVertex new_v;
    new_v.X = (PEPCB::Base::TDim)(_v.X * cos_a) - (PEPCB::Base::TDim)(_v.Y * sin_a);
    new_v.Y = (PEPCB::Base::TDim)(_v.X * sin_a) + (PEPCB::Base::TDim)(_v.Y * cos_a);
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
void PEPCB::Base::TCopper::addGeometry(PEPCB::Base::ELayer _layer, PEPCB::Base::TGeometry _polygon)
{
    if (isCopperLayer(_layer))
    {
        if (this->polygon_list.find(_layer) == this->polygon_list.end())
        {
            std::vector<PEPCB::Base::TGeometry> polygons;
            polygons.push_back(_polygon);
            this->polygon_list.insert(std::pair<PEPCB::Base::ELayer, std::vector<PEPCB::Base::TGeometry>>(_layer, polygons));
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

PEPCB::Base::TRect::TRect(PEPCB::Base::TVertex _centre, PEPCB::Base::TDim _sizeX, PEPCB::Base::TDim _sizeY, PEPCB::Base::TAngle _angle)
{
    double angle = _angle / 1800.0 * M_PI;
    // Rectangle - only outer contour - anti-clockwise
    PEPCB::Base::TVertex v;
    v.X = +_sizeX / 2;
    v.Y = +_sizeY / 2;
    v = PEPCB::Base::rotate(v, angle);
    v.X += _centre.X;
    v.Y += _centre.Y;
    this->outer_vertex_list.push_back(v);
    v.X = -_sizeX / 2;
    v.Y = +_sizeY / 2;
    v = PEPCB::Base::rotate(v, angle);
    v.X += _centre.X;
    v.Y += _centre.Y;
    this->outer_vertex_list.push_back(v);
    v.X = -_sizeX / 2;
    v.Y = -_sizeY / 2;
    v = PEPCB::Base::rotate(v, angle);
    v.X += _centre.X;
    v.Y += _centre.Y;
    this->outer_vertex_list.push_back(v);
    v.X = +_sizeX / 2;
    v.Y = -_sizeY / 2;
    v = PEPCB::Base::rotate(v, angle);
    v.X += _centre.X;
    v.Y += _centre.Y;
    this->outer_vertex_list.push_back(v);
}

PEPCB::Base::TLine::TLine(PEPCB::Base::TVertex _a, PEPCB::Base::TVertex _b, PEPCB::Base::TDim _width)
{
    PEPCB::Base::TDim radius = _width / 2;
    PEPCB::Base::TVertex v;
    double slope_angle = atan2(_b.Y - _a.Y, _b.X - _a.X);
    double increment_angle = 2 * M_PI / CIRCLE_EDGES;
    // std::cout << "Line: a(" << _a.X << ", " << _a.Y << ") b(" << _b.X << ", " << _b.Y << ")" << std::endl;
    for (int i = 0; i < CIRCLE_EDGES / 2 + 1; i++)
    {
        v = {radius, 0};
        v = PEPCB::Base::rotate(v, slope_angle + M_PI_2 + increment_angle * i);
        v.X += _a.X;
        v.Y += _a.Y;
        this->outer_vertex_list.push_back(v);
    }
    for (int i = 0; i < CIRCLE_EDGES / 2 + 1; i++)
    {
        v = {-radius, 0};
        v = PEPCB::Base::rotate(v, slope_angle + M_PI_2 + increment_angle * i);
        v.X += _b.X;
        v.Y += _b.Y;
        this->outer_vertex_list.push_back(v);
    }
}

PEPCB::Base::TRectRound::TRectRound(PEPCB::Base::TVertex _centre, PEPCB::Base::TDim _sizeX, PEPCB::Base::TDim _sizeY, PEPCB::Base::TAngle _angle, double _round_ratio)
{
    PEPCB::Base::TDim short_edge = _sizeX > _sizeY ? _sizeY : _sizeX;
    double round_ratio = _round_ratio;
    if (round_ratio >= 0.5)
        round_ratio = 0.4999999999999999;
    PEPCB::Base::TDim radius = (short_edge)*round_ratio;
    PEPCB::Base::TVertex v;
    double angle = _angle / 1800.0 * M_PI;
    double increment_angle = 2 * M_PI / CIRCLE_EDGES;
    for (int i = 0; i < CIRCLE_EDGES / 4 + 1; i++)
    {
        v = {radius, 0};
        v = PEPCB::Base::rotate(v, increment_angle * i);
        v.X += _sizeX / 2 - radius;
        v.Y += _sizeY / 2 - radius;
        v = PEPCB::Base::rotate(v, angle);
        v.X += _centre.X;
        v.Y += _centre.Y;
        this->outer_vertex_list.push_back(v);
    }
    for (int i = 0; i < CIRCLE_EDGES / 4 + 1; i++)
    {
        v = {0, radius};
        v = PEPCB::Base::rotate(v, increment_angle * i);
        v.X -= _sizeX / 2 - radius;
        v.Y += _sizeY / 2 - radius;
        v = PEPCB::Base::rotate(v, angle);
        v.X += _centre.X;
        v.Y += _centre.Y;
        this->outer_vertex_list.push_back(v);
    }
    for (int i = 0; i < CIRCLE_EDGES / 4 + 1; i++)
    {
        v = {-radius, 0};
        v = PEPCB::Base::rotate(v, increment_angle * i);
        v.X -= _sizeX / 2 - radius;
        v.Y -= _sizeY / 2 - radius;
        v = PEPCB::Base::rotate(v, angle);
        v.X += _centre.X;
        v.Y += _centre.Y;
        this->outer_vertex_list.push_back(v);
    }
    for (int i = 0; i < CIRCLE_EDGES / 4 + 1; i++)
    {
        v = {0, -radius};
        v = PEPCB::Base::rotate(v, increment_angle * i);
        v.X += _sizeX / 2 - radius;
        v.Y -= _sizeY / 2 - radius;
        v = PEPCB::Base::rotate(v, angle);
        v.X += _centre.X;
        v.Y += _centre.Y;
        this->outer_vertex_list.push_back(v);
    }
}