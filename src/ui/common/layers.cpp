#include "layers.hpp"
#include "earcut.hpp"

#include <iostream>

using namespace PEPCB::UI;
using namespace PEPCB::Base;

// struct IntPoint {
//     int64_t X, Y;
// };

namespace mapbox
{
    namespace util
    {

        template <>
        struct nth<0, TVertex>
        {
            inline static auto get(const TVertex &t)
            {
                return t.X;
            };
        };
        template <>
        struct nth<1, TVertex>
        {
            inline static auto get(const TVertex &t)
            {
                return t.Y;
            };
        };

    } // namespace util
} // namespace mapbox

LayersRenderer::LayersRenderer()
{
    this->layer_color_map = {
        {F_COURTYARD, {255, 255, 255}},
        {F_CU, {231, 76, 60}},
        {F_SILKSCREEN, {242, 243, 244}},
        {B_CU, {52, 152, 219}},
        {B_SILKSCREEN, {229, 231, 233}},
        {B_COURTYARD, {128, 128, 128}},
        {VIA, {26, 188, 156}},
        {EDGE_CUTS, {255, 255, 128}},
        {HOLE, {0, 0, 0}},
    };

    this->layer_draw_rank_map = {
        {F_CU, 50},
        {F_SILKSCREEN, 90},
        {B_CU, 20},
        {B_SILKSCREEN, 10},
        {F_COURTYARD, 99},
        {VIA, 75},
        {EDGE_CUTS, 100},
        {HOLE, 85},
    };

    this->index_buffer_data = nullptr;
    this->vertex_buffer_data = nullptr;
    this->total_index_numbers = 0;
    this->total_vertex_numbers = 0;
}

LayersRenderer::~LayersRenderer()
{
    if (this->index_buffer_data != nullptr)
    {
        delete[] this->index_buffer_data;
    }
    if (this->vertex_buffer_data != nullptr)
    {
        delete[] this->vertex_buffer_data;
    }
}

void LayersRenderer::addGeometry(TGeometry polygon, ELayer layer)
{
    std::vector<std::vector<TVertex>> buf_polygon;
    buf_polygon.push_back(polygon.outer_vertex_list);
    for (auto it = polygon.inner_vertex_list_list.begin(); it < polygon.inner_vertex_list_list.end(); it++)
    {
        buf_polygon.push_back(*it);
    }
    std::vector<uint32_t> indicies = mapbox::earcut<uint32_t>(buf_polygon);
    uint32_t index_map_offset = this->vertex_map[layer].size();
    for (auto it = buf_polygon.begin(); it < buf_polygon.end(); it++)
    {
        this->vertex_map[layer].insert(this->vertex_map[layer].end(), it->begin(), it->end());
        this->total_vertex_numbers += it->size();
    }
    for (auto it = indicies.begin(); it < indicies.end(); it++)
    {
        this->index_map[layer].push_back((*it) + index_map_offset);
    }
    this->total_index_numbers += indicies.size();
    // for(int i = 0; i<indicies.size(); i++){
    //     if(i%3 == 2)
    //         std::cout << indicies[i] << std::endl;
    //     else
    //         std::cout << indicies[i] << ", ";
    // }
}

bool cmp_layer_order(std::pair<ELayer, int> &a, std::pair<ELayer, int> &b)
{
    return a.second < b.second;
}

void LayersRenderer::updateBuffer()
{
    std::vector<std::pair<ELayer, int>> layer_sort_list;

    for (auto it_l = this->vertex_map.begin(); it_l != this->vertex_map.end(); it_l++)
    {
        layer_sort_list.push_back(std::pair<ELayer, int>(it_l->first, this->layer_draw_rank_map[it_l->first]));
    }

    std::sort(layer_sort_list.begin(), layer_sort_list.end(), cmp_layer_order);

    if (this->index_buffer_data)
    {
        delete[] this->index_buffer_data;
    }
    if (this->vertex_buffer_data)
    {
        delete[] this->vertex_buffer_data;
    }

    this->index_buffer_data = new GLuint[total_index_numbers];
    this->vertex_buffer_data = new GLfloat[total_vertex_numbers * 5];

    uint32_t vertex_i = 0;
    uint32_t index_i = 0;
    for (auto it_l = layer_sort_list.begin(); it_l < layer_sort_list.end(); it_l++)
    {
        for (auto it_i = this->index_map[it_l->first].begin(); it_i < this->index_map[it_l->first].end(); it_i++)
        {
            this->index_buffer_data[index_i++] = (*it_i) + (vertex_i / 5);
        }
        for (auto it_v = this->vertex_map[it_l->first].begin(); it_v < this->vertex_map[it_l->first].end(); it_v++)
        {
            this->vertex_buffer_data[vertex_i++] = (GLfloat)it_v->X;
            this->vertex_buffer_data[vertex_i++] = (GLfloat)it_v->Y;
            this->vertex_buffer_data[vertex_i++] = (GLfloat)(this->layer_color_map[it_l->first].r / 256.0f);
            this->vertex_buffer_data[vertex_i++] = (GLfloat)(this->layer_color_map[it_l->first].g / 256.0f);
            this->vertex_buffer_data[vertex_i++] = (GLfloat)(this->layer_color_map[it_l->first].b / 256.0f);
        }
    }

    // for (int i = 0; i < this->total_vertex_numbers * 5; i++)
    // {
    //     if (i % 5 == 4)
    //     {
    //         std::cout << this->vertex_buffer_data[i] << std::endl;
    //     }
    //     else
    //     {
    //         std::cout << this->vertex_buffer_data[i] << " ";
    //     }
    // }

    // std::cout << std::endl;

    // for (int i = 0; i < this->total_index_numbers; i++)
    // {
    //     if (i % 3 == 2)
    //     {
    //         std::cout << this->index_buffer_data[i] << std::endl;
    //     }
    //     else
    //     {
    //         std::cout << this->index_buffer_data[i] << " ";
    //     }
    // }
}