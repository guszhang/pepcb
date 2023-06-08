#pragma once

#include "../../core/core.h"
#include <GLFW/glfw3.h>

namespace PEPCB
{
    namespace UI
    {

        class LayersRenderer
        {
        public:
            std::map<PEPCB::Base::ELayer, PEPCB::Base::TColor> layer_color_map;
            std::map<PEPCB::Base::ELayer, int> layer_draw_rank_map;
            std::map<PEPCB::Base::ELayer, std::vector<PEPCB::Base::TVertex>> vertex_map;
            std::map<PEPCB::Base::ELayer, std::vector<uint32_t>> index_map;
            uint32_t total_vertex_numbers;
            uint32_t total_index_numbers;
            GLfloat* vertex_buffer_data;
            GLuint* index_buffer_data;
            LayersRenderer();
            ~LayersRenderer();
            void addGeometry(PEPCB::Base::TGeometry polygon, PEPCB::Base::ELayer layer);
            void updateBuffer();
        private:
        };
    }
}