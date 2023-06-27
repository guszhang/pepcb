// PEPCB core.h
// Date: 03 May 2022 - 31 May 2023
// University of Manchester, Gus Zhang <cheng.zhang@manchester.ac.uk>

#pragma once

#include <vector>
#include <map>
#include <string>

#define CIRCLE_EDGES 32

namespace PEPCB
{
    namespace Base
    {
        // Length representation: in int64_t with a minimum resolution of 1 nm;
        typedef int64_t TDim;

        // Angle representation: in int32_t with a minimum resolution of 0.1 degree. Thus only using range 0-3599. Using uint32 because of memory alignment.
        typedef int32_t TAngle;

        // Layer used in vias representation:
        typedef uint32_t TLayer;

        // Net representation: in uint64_t
        typedef uint64_t TNet;

        // Weightings representation: in double
        typedef double TWeighting;

        // Enumerate Layer
        enum ELayer : TLayer
        {
            F_CU = 0,
            IN1_CU = 1,
            IN2_CU = 2,
            IN3_CU = 3,
            IN4_CU = 4,
            IN5_CU = 5,
            IN6_CU = 6,
            IN7_CU = 7,
            IN8_CU = 8,
            IN9_CU = 9,
            IN10_CU = 10,
            IN11_CU = 11,
            IN12_CU = 12,
            IN13_CU = 13,
            IN14_CU = 14,
            IN15_CU = 15,
            IN16_CU = 16,
            IN17_CU = 17,
            IN18_CU = 18,
            IN19_CU = 19,
            IN20_CU = 20,
            IN21_CU = 21,
            IN22_CU = 22,
            IN23_CU = 23,
            IN24_CU = 24,
            IN25_CU = 25,
            IN26_CU = 26,
            IN27_CU = 27,
            IN28_CU = 28,
            IN29_CU = 29,
            IN30_CU = 30,

            // layers below won't need assigned values
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
            EDGE_CUTS,
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
            // extra defined layers
            VIA,
            HOLE,
            UNRECOGNISED,
            // bottom copper is always the last layer
            B_CU = UINT32_MAX,
        };

        // Enumerate Placement Layer
        enum EPLayer
        {
            TOP,
            BOTTOM,
        };

        // Enumerate Geometry Type
        enum EGeometryType
        {
            LINE,
            LINE_STRIP,
            LOOP,
            CIRCLE,
            POLYGON,
        };

        // Type Color
        typedef struct
        {
            uint8_t r;
            uint8_t g;
            uint8_t b;
            uint8_t a;
        } TColor;

        // Type Vertex
        typedef struct
        {
            TDim X;
            TDim Y;
        } TVertex; // Vertex positions are stored in integers, with resolution of 1 nm.

        TVertex rotate(TVertex _v, double _angle);

        // Types of geometries
        class TGeometry // by default, any geometry is a polygon (with holes)
        {
        public:
            std::vector<TVertex> outer_vertex_list;
            std::vector<std::vector<TVertex>> inner_vertex_list_list;
            void log(std::string name);
        };

        class TRect : public TGeometry
        {
        public:
            TRect(TVertex _centre, TDim _sizeX, TDim _sizeY, TAngle _angle);
        };

        class TRectRound : public TGeometry
        {
        public:
            TRectRound(TVertex _centre, TDim _sizeX, TDim _sizeY, TAngle _angle, double _round_ratio);
        };

        class TLine : public TGeometry
        {
        public:
            TLine(TVertex _a, TVertex _b, TDim _width);
        };

        class TLineStrip : public TGeometry
        {
        public:
            std::vector<TVertex> vertex_list;
            TDim width;
        };

        class TLoop : public TGeometry
        {
        public:
            std::vector<TVertex> vertex_list;
            TDim width;
        };

        class TCircleNonfill : public TGeometry
        {
        public:
            TVertex centre;
            TDim radius, width;
        };

        class TCircle : public TGeometry
        {
        public:
            TVertex centre;
            TDim radius;
        };

        class TArc : public TGeometry
        {
        public:
            TVertex a, b;
            TDim width;
            TAngle angle_start, angle_end;
        };

        class TDrill
        {
        public:
            TVertex centre;
            TDim drill_diameter;
        };

        class TVia
        {
        public:
            TVertex centre;
            TDim drill_diameter;
            ELayer layer_start, layer_end; // layer start < layer end; top layer always=0, bottom layer always=UINT8_MAX
        };

        // Type Copper (copper piece or copper continuum) must be a single continuum, however, it cannot be restrained from the data structure, thus has to be checked though additional methods
        class TCopper
        {
        public:
            std::map<ELayer, std::vector<TGeometry>> polygon_list;
            std::vector<TVia> via_list;

            TDim clearance;
            TNet net;
            TWeighting weighting_I, weighting_dI, weighting_V, weighting_dV;

            bool isValid;
            void validate(void);
            void addGeometry(ELayer _layer, TGeometry _polygon);
        };

        class TFootprint
        {
        public:
            std::string name;
            std::map<ELayer, std::vector<TGeometry>> layer_list;
            std::vector<TVia> via_list;
            std::vector<TDrill> drill_list;
            std::map<std::string, TCopper> pad_list;
            void insertGeometry(ELayer _layer, TGeometry _geometry);
        };

        class TComponent
        {
        public:
            TVertex position;
            TAngle angle;
            EPLayer placement_layer;
            TFootprint footprint;
            std::string ref;
        };

        class TAggregation
        {
        public:
            std::map<std::string, TComponent> component_list; // a list of components that has been placed down
            std::vector<TGeometry> top_courtyard_list, bottom_courtyard_list;
            std::vector<TCopper> copper_list;

            bool isValid;
            void validate(void);
        };
    }

    namespace CoreCircuit
    {
        class CircuitDefinition
        {
        public:
            std::vector<Base::TComponent> component_list;
            std::vector<std::string> net_list;

            std::vector<Base::TAggregation> generate_aggregation_list();
        };

    }

    namespace GA
    {
        class GACandidate
        {
        public:
            std::vector<Base::TAggregation> aggregation_list;
        };
    }

    namespace FS
    {
        std::string readFromFile(std::string _Filename);
    }
}