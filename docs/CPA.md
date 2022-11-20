# The CPA data structure

## Introduction

The **Copper Polygon Area** data structure are dedicated for describing, storing, and tweaking the copper areas for power electronics PCB designs. It contains a similar scope of information as any normal PCB layout files in other CAD software, but describing the copper traces in a uniform way &mdash; thus efficient for programmes to read/write and tweak/evaluate.

### Authorship

Author: Gus Zhang &lt;cheng.zhang@manchester.ac.uk&gt;
Date: 18 Nov 2022

### License

MIT LICENSE

Copyright 2022 Gus Zhang

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

## Scope of this document

This document is the technical specifications of one implementation, in C++, of the CPA concept. The following properties are explicitly defined.

- Definitions/enumerations constants such as layer names and specific functionalities
- Definitions of structure names and hierarchies
- Definitions of basic methods

## Definitions of constants

### Layers

<u> Declaration:</u> **enum** _ELayer_

Layers constants are defined as enum type in C++ and automatically assigned values. The linked functionalities are bind to corresponding evaluation and tweaking methods. The layers definitions are adopting the [KiCAD Version 6 conventions](https://dev-docs.kicad.org/en/file-formats/sexpr-intro/#:~:text=will%20be%20truncated.-,Layers,-All%20drawable%20board), including (copied from the webpage):

- 60 total layers.
- 32 copper layers.
- 8 paired technical layers for silk screen, solder mask, solder paste, and adhesive.
- 4 user pre-defined layers for drawings, engineering change order (ECO), and comments.
- 1 layer to define the board outline.
- 1 layer to define the board margins.
- 9 optional user definable layers.

| Name          | Description                           | Linked Functionality           |
| ------------- | ------------------------------------- | ------------------------------ |
| F_CU          | Front copper layer                    | Copper DRC rules               |
| IN1_CU        | Inner copper layer 1                  | Copper DRC rules               |
| IN2_CU        | Inner copper layer 2                  | Copper DRC rules               |
| IN3_CU        | Inner copper layer 3                  | Copper DRC rules               |
| IN4_CU        | Inner copper layer 4                  | Copper DRC rules               |
| IN5_CU        | Inner copper layer 5                  | Copper DRC rules               |
| IN6_CU        | Inner copper layer 6                  | Copper DRC rules               |
| IN7_CU        | Inner copper layer 7                  | Copper DRC rules               |
| IN8_CU        | Inner copper layer 8                  | Copper DRC rules               |
| IN9_CU        | Inner copper layer 9                  | Copper DRC rules               |
| IN10_CU       | Inner copper layer 10                 | Copper DRC rules               |
| IN11_CU       | Inner copper layer 11                 | Copper DRC rules               |
| IN12_CU       | Inner copper layer 12                 | Copper DRC rules               |
| IN13_CU       | Inner copper layer 13                 | Copper DRC rules               |
| IN14_CU       | Inner copper layer 14                 | Copper DRC rules               |
| IN15_CU       | Inner copper layer 15                 | Copper DRC rules               |
| IN16_CU       | Inner copper layer 16                 | Copper DRC rules               |
| IN17_CU       | Inner copper layer 17                 | Copper DRC rules               |
| IN18_CU       | Inner copper layer 18                 | Copper DRC rules               |
| IN19_CU       | Inner copper layer 19                 | Copper DRC rules               |
| IN20_CU       | Inner copper layer 20                 | Copper DRC rules               |
| IN21_CU       | Inner copper layer 21                 | Copper DRC rules               |
| IN22_CU       | Inner copper layer 22                 | Copper DRC rules               |
| IN23_CU       | Inner copper layer 23                 | Copper DRC rules               |
| IN24_CU       | Inner copper layer 24                 | Copper DRC rules               |
| IN25_CU       | Inner copper layer 25                 | Copper DRC rules               |
| IN26_CU       | Inner copper layer 26                 | Copper DRC rules               |
| IN27_CU       | Inner copper layer 27                 | Copper DRC rules               |
| IN28_CU       | Inner copper layer 28                 | Copper DRC rules               |
| IN29_CU       | Inner copper layer 29                 | Copper DRC rules               |
| IN30_CU       | Inner copper layer 30                 | Copper DRC rules               |
| B_CU          | Back copper layer                     | Copper DRC rules               |
| B_ADHESIVE    | Back adhesive layer                   | Not used                       |
| F_ADHESIVE    | Front adhesive layer                  | Not used                       |
| B_PASTE       | Back solder paste layer               | Not used                       |
| F_PASTE       | Front solder paste layer              | Not used                       |
| B_SILKSCREEN  | Back silk screen layer                | Not used                       |
| F_SILKSCREEN  | Front silk screen layer               | Not used                       |
| B_MASK        | Back solder mask layer                | Not used                       |
| F_MASK        | Front solder mask layer               | Not used                       |
| USER_DRAWINGS | User drawing layer                    | Not used                       |
| USER_COMMENTS | User comment layer                    | Not used                       |
| USER_ECO1     | User engineering change order layer 1 | Not used                       |
| USER_ECO2     | User engineering change order layer 2 | Not used                       |
| EDGE_CU       | Board outline layer                   | Placement and copper DRC rules |
| F_COURTYARD   | Footprint front courtyard layer       | Placement DRC rules            |
| B_COURTYARD   | Footprint back courtyard layer        | Placement DRC rules            |
| F_FAB         | Footprint front fabrication layer     | Not used                       |
| B_FAB         | Footprint back fabrication layer      | Not used                       |
| USER_1        | User definable layer 1                | Not used                       |
| USER_2        | User definable layer 2                | Not used                       |
| USER_3        | User definable layer 3                | Not used                       |
| USER_4        | User definable layer 4                | Not used                       |
| USER_5        | User definable layer 5                | Not used                       |
| USER_6        | User definable layer 6                | Not used                       |
| USER_7        | User definable layer 7                | Not used                       |
| USER_8        | User definable layer 8                | Not used                       |
| USER_9        | User definable layer 9                | Not used                       |

### Placement Layers

<u> Declaration:</u> **enum** _EPLayer_

Placement Layers constants are defined for specifying which side the component is placed. All components are expected to be placed on either the top or the bottom layer (that is to say, cannot be embedded within layers).

| Name   | Description  |
| ------ | ------------ |
| TOP    | Top layer    |
| BOTTOM | Bottom layer |

### Graphics Types

<u> Declaration:</u> **enum** _EGraphicsType_

The graphics types are for display purposes.

| Name        | Description                                           |
| ----------- | ----------------------------------------------------- |
| LINE        | Single segment line                                   |
| LINE_STRIP  | Multi-segment lines, open-ended                       |
| LOOP        | Multi-segment lines, closed                           |
| CIRCLE      | Circle, not filled                                    |
| CIRCLE_FILL | Circle, filled                                        |
| ARC         | Arc                                                   |
| POLYGON     | Polygon, filled, different from polygon for computing |
| TEXT        | Texts                                                 |

## Definition of Types and Structures

The numerical system uses int64_t for coordinates with a resolution of 1 nm. KiCAD is using 32-bit integer with the same resolution, results in a maximum range of +/-2.14 m. The computation performance on modern x86_64 and arm64 architectures for 32-bit and 64-bit integers are almost the same.

### Vertex

<u> Declaration:</u> **struct** _TVertex_

| Property | Type    | Description            |
| -------- | ------- | ---------------------- |
| X        | int64_t | x coordinate, step 1nm |
| Y        | int64_t | y coordinate, step 1nm |

### Part

<u> Declaration:</u> **struct** _TPart_

| Property  | Type                       | Description                                                                                       |
| --------- | -------------------------- | ------------------------------------------------------------------------------------------------- |
| ref       | std::string                | Designator reference                                                                              |
| value     | std::string                | Value                                                                                             |
| pad_list  | std::vector&lt;TPad&gt;    | Pad list                                                                                          |


### Copper Boundary (Polygon)

<u> Declaration:</u> **struct** _TPolygon_

| Property | Type | Description |
| -------- | ---- | ----------- |

|

### Colours

<u> Declaration:</u> **struct** _TColor_

Colour properties for rendering in the viewer.

| Property | Type    | Description |
| -------- | ------- | ----------- |
| r        | uint8_t | red         |
| g        | uint8_t | green       |
| b        | uint8_t | blue        |
| a        | uint8_t | alpha       |

### Part Placement

<u> Declaration:</u> **struct** _TPartPlacement_

| Property        | Type    | Description                   |
| --------------- | ------- | ----------------------------- |
| position        | TVertex | position of the component     |
| angle           | float   | rotate angle (CCW) in radiant |
| placement_layer | EPLayer | placement layer               |
