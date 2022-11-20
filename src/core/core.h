// PEPCB core.h
// Date: 03 May 2022
// University of Manchester, Gus Zhang <cheng.zhang@manchester.ac.uk>

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
            F_CU,
            IN1_CU,
            IN2_CU,
            IN3_CU,
            IN4_CU,
            IN5_CU,
            IN6_CU,
            IN7_CU,
            IN8_CU,
            IN9_CU,
            IN10_CU,
            IN11_CU,
            IN12_CU,
            IN13_CU,
            IN14_CU,
            IN15_CU,
            IN16_CU,
            IN17_CU,
            IN18_CU,
            IN19_CU,
            IN20_CU,
            IN21_CU,
            IN22_CU,
            IN23_CU,
            IN24_CU,
            IN25_CU,
            IN26_CU,
            IN27_CU,
            IN28_CU,
            IN29_CU,
            IN30_CU,
            B_CU,
            B_ADHESIVE,
            F_ADHESIVE,
            B_PASTE,
            F_PASTE,
            B_SILKSCREEN,
            F_SILKSCREEN,
            B_MASK,
            F_MASK,
            USER_DRAWINGS,
            USER_COMMENTS,
            USER_ECO1,
            USER_ECO2,
            EDGE_CU,
            F_COURTYARD,
            B_COURTYARD,
            F_FAB,
            B_FAB,
            USER_1,
            USER_2,
            USER_3,
            USER_4,
            USER_5,
            USER_6,
            USER_7,
            USER_8,
            USER_9,
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
            ELayer layer;
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
            std::string name;            // for printing
            PEPCB::Base::TVertex anchor; // geometrical center for simplified connection binding
            std::multimap<Base::ELayer, Base::TGeometry> geometry_list;
        } TPad;

        typedef struct
        {
            std::string name;  // for printing
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