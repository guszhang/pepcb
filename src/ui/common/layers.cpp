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
        {TOP_COPPER, {231, 76, 60}},
        {TOP_SILK, {242, 243, 244}},
        {BOTTOM_COPPER, {52, 152, 219}},
        {BOTTOM_SILK, {229, 231, 233}},
        {COLLISION_BOUNDARY, {255, 255, 255}},
        {VIA, {26, 188, 156}},
        {EDGE, {255, 255, 128}},
        {HOLE, {0, 0, 0}},
    };

    this->layer_draw_rank_map = {
        {TOP_COPPER, 50},
        {TOP_SILK, 90},
        {BOTTOM_COPPER, 20},
        {BOTTOM_SILK, 10},
        {COLLISION_BOUNDARY, 99},
        {VIA, 75},
        {EDGE, 100},
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

void LayersRenderer::addPolygon(TPolygon polygon, ELayer layer)
{
    std::vector<std::vector<TVertex>> buf_polygon;
    buf_polygon.push_back(polygon.outer_vertex_list);
    for (auto it = polygon.inner_vertex_list_list.begin(); it < polygon.inner_vertex_list_list.end(); it++)
    {
        buf_polygon.push_back(*it);
    }
    std::vector<uint32_t> indicies = mapbox::earcut<uint32_t>(buf_polygon);
    uint32_t index_map_offset = this->vertex_map.size();
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

void LayersRenderer::updateBuffer()
{
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
    for (auto it_l = this->vertex_map.begin(); it_l != this->vertex_map.end(); it_l++)
    {
        for (auto it_v = it_l->second.begin(); it_v < it_l->second.end(); it_v++)
        {
            this->vertex_buffer_data[vertex_i++] = (GLfloat)it_v->X;
            this->vertex_buffer_data[vertex_i++] = (GLfloat)it_v->Y;
            this->vertex_buffer_data[vertex_i++] = (GLfloat)(this->layer_color_map[it_l->first].r / 256.0f);
            this->vertex_buffer_data[vertex_i++] = (GLfloat)(this->layer_color_map[it_l->first].g / 256.0f);
            this->vertex_buffer_data[vertex_i++] = (GLfloat)(this->layer_color_map[it_l->first].b / 256.0f);
        }
        for (auto it_i = this->index_map[it_l->first].begin(); it_i < this->index_map[it_l->first].end(); it_i++)
        {
            this->index_buffer_data[index_i++] = *it_i;
        }
    }

    // for (int i = 0; i < this->total_vertex_numbers*5; i++)
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