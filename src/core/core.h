#pragma once

#include <vector>
#include <map>
#include <unordered_map>
#include <string>

namespace PEPCB
{
    namespace Base
    {
        enum ELayer
        {
            COLLISION_BOUNDARY,
            TOP_COPPER,
            ROUTE2_COPPER,
            ROUTE3_COPPER,
            ROUTE4_COPPER,
            ROUTE5_COPPER,
            ROUTE6_COPPER,
            ROUTE7_COPPER,
            ROUTE8_COPPER,
            ROUTE9_COPPER,
            ROUTE10_COPPER,
            ROUTE11_COPPER,
            ROUTE12_COPPER,
            ROUTE13_COPPER,
            ROUTE14_COPPER,
            ROUTE15_COPPER,
            BOTTOM_COPPER,
            TOP_SILK,
            BOTTOM_SILK,
            TOP_MASK,
            BOTTOM_MASK,
            TOP_PASTE,
            BOTTOM_PASTE,
            VIA,
            EDGE,
            DRILL,
            ANNOTATION,
        };

        enum EPLayer
        {
            TOP,
            BOTTOM,
        };

        enum EGeometryType
        {
            LINE,
            LINE_STRIP,
            LOOP,
            CIRCLE,
            POLYGON,
        };

        typedef struct
        {
            uint8_t r;
            uint8_t g;
            uint8_t b;
            uint8_t a;
        } TColor;

        typedef struct
        {
            int64_t X;
            int64_t Y;
        } TVertex; // Vertex positions are stored in integers, with resolution of 1 nm.

        // typedef struct
        // {
        //     ELayer layer;
        //     EGeometryType type;
        //     std::vector<TVertex> vertex_list;
        // } TGeometry;

        class TGeometry
        {
        public:
            EGeometryType type;
        };

        class TPolygon : public TGeometry
        {
        public:
            std::vector<TVertex> outer_vertex_list;
            std::vector<std::vector<TVertex>> inner_vertex_list_list;
        };

        class TLine : public TGeometry
        {
        public:
            TVertex a, b;
            uint64_t width;
        };

        class TLineStrip : public TGeometry
        {
        public:
            std::vector<TVertex> vertex_list;
            uint64_t width;
        };

        class TLoop : public TGeometry
        {
        public:
            std::vector<TVertex> vertex_list;
            uint64_t width;
        };

        class TCircleNonfill : public TGeometry
        {
        public:
            TVertex centre;
            uint64_t radius, width;
        };

        class TCircle : public TGeometry
        {
        public:
            TVertex centre;
            uint64_t radius;
        };

        class TArc : public TGeometry
        {
        public:
            TVertex a, b;
            double angle;
            uint64_t width;
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
            int net_id;
            std::string name; // for printing
            TVertex anchor; // geometrical center for simplified connection binding
            std::multimap<Base::ELayer, Base::TGeometry> geometry_list;
        } TPad;

        typedef struct
        {
            std::string name; // for printing
            std::string value; // for printing
            std::vector<TPad> pad_list;
            std::multimap<Base::ELayer, Base::TGeometry> geometry_list;
        } TPart;

        class CircuitDetails
        {
        public:
            std::vector<TConnection> connection_list;
            std::vector<TPart> part_list;
            std::unordered_map<std::string, int> part_ref;
            std::vector<std::string> net_list;
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
            Base::TVertex center;
            uint64_t pad_diameter, drill_diameter;
        } TVia;
        
        typedef struct
        {
            std::vector<std::multimap<Base::ELayer, Base::TPolygon>> polygon_list;
            std::vector<TVia> via_list; // through holes only.
        } TCopperPieces;

        class candidate
        {
        private:
            std::map<int, TPartPlacement> part_list; // key: part_id
            std::map<int, TCopperPieces> net_list;   // key: net_id
        };
    }

    namespace FS
    {
        std::string readFromFile(std::string _Filename);
    }
}