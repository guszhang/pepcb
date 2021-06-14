#pragma once

#include <vector>
#include <map>

namespace Pepcb
{
    namespace Base
    {
        typedef struct
        {
            int x;
            int y;
        } TVertex;

        typedef struct
        {
            int layer;
            std::vector<TVertex> vertex_list;
        } TPolygon;

        enum ELayer
        {
            COLLISION_BOUNDARY,
            TOP_COPPER,
            BOTTOM_COPPER,
            TOP_SILK,
            BOTTOM_SILK
        };

        enum EPLayer
        {
            TOP,
            BOTTOM,
        };
    }
    namespace CoreCircuit
    {
        typedef struct
        {
            int net_id;
            int connection_id;
            int part1_id;
            int part1_pad_id;
            int part2_id;
            int part2_pad_id;
            double weight;
        } TConnection;

        typedef struct
        {
            Base::TVertex origin; // relative position to the part's origin
            std::multimap<Base::ELayer, Base::TPolygon> geometry_list;
        } TPad;

        typedef struct
        {
            char *name; // for printing
            std::vector<TPad> pad_list;
            std::multimap<Base::ELayer, Base::TPolygon> geometry_list;
        } TParts;

        class CircuitDetails
        {
        public:
            std::vector<TConnection> connection_list;
            std::vector<TParts> part_list;
            std::vector<char *> net_list;
        };

    }

    namespace GA
    {
        typedef struct
        {
            Base::TVertex origin;
            float angle;
            Base::EPLayer layer;
        } TPartPlacement;

        typedef struct
        {
            std::vector<Base::TPolygon> polygon_list;
        } TCopperPieces;

        class candidate
        {
        private:
            std::map<int, TPartPlacement> part_list; // key: part_id
            std::map<int, TCopperPieces> net_list;   // key: net_id
        };
    }
}