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

-   Definitions/enumerations constants such as layer names and specific functionalities
-   Definitions of structure names and hierarchies
-   Definitions of basic methods

## Definitions of constants

### Layers

<u> Declaration:</u> **enum** _ELayer_

Layers constants are defined as enum type in C++ and automatically assigned values. The linked functionalities are bind to corresponding evaluation and tweaking methods. The layers definitions are adopting the [KiCAD Version 6 conventions](https://dev-docs.kicad.org/en/file-formats/sexpr-intro/#:~:text=will%20be%20truncated.-,Layers,-All%20drawable%20board), including (copied from the webpage):

-   60 total layers.
-   32 copper layers.
-   8 paired technical layers for silk screen, solder mask, solder paste, and adhesive.
-   4 user pre-defined layers for drawings, engineering change order (ECO), and comments.
-   1 layer to define the board outline.
-   1 layer to define the board margins.
-   9 optional user definable layers.

| Name          | Description                           | Linked Functionality           |
| ------------- | ------------------------------------- | ------------------------------ |
| F_Cu          | Front copper layer                    | Copper DRC rules               |
| In1_Cu        | Inner copper layer 1                  | Copper DRC rules               |
| In2_Cu        | Inner copper layer 2                  | Copper DRC rules               |
| In3_Cu        | Inner copper layer 3                  | Copper DRC rules               |
| In4_Cu        | Inner copper layer 4                  | Copper DRC rules               |
| In5_Cu        | Inner copper layer 5                  | Copper DRC rules               |
| In6_Cu        | Inner copper layer 6                  | Copper DRC rules               |
| In7_Cu        | Inner copper layer 7                  | Copper DRC rules               |
| In8_Cu        | Inner copper layer 8                  | Copper DRC rules               |
| In9_Cu        | Inner copper layer 9                  | Copper DRC rules               |
| In10_Cu       | Inner copper layer 10                 | Copper DRC rules               |
| In11_Cu       | Inner copper layer 11                 | Copper DRC rules               |
| In12_Cu       | Inner copper layer 12                 | Copper DRC rules               |
| In13_Cu       | Inner copper layer 13                 | Copper DRC rules               |
| In14_Cu       | Inner copper layer 14                 | Copper DRC rules               |
| In15_Cu       | Inner copper layer 15                 | Copper DRC rules               |
| In16_Cu       | Inner copper layer 16                 | Copper DRC rules               |
| In17_Cu       | Inner copper layer 17                 | Copper DRC rules               |
| In18_Cu       | Inner copper layer 18                 | Copper DRC rules               |
| In19_Cu       | Inner copper layer 19                 | Copper DRC rules               |
| In20_Cu       | Inner copper layer 20                 | Copper DRC rules               |
| In21_Cu       | Inner copper layer 21                 | Copper DRC rules               |
| In22_Cu       | Inner copper layer 22                 | Copper DRC rules               |
| In23_Cu       | Inner copper layer 23                 | Copper DRC rules               |
| In24_Cu       | Inner copper layer 24                 | Copper DRC rules               |
| In25_Cu       | Inner copper layer 25                 | Copper DRC rules               |
| In26_Cu       | Inner copper layer 26                 | Copper DRC rules               |
| In27_Cu       | Inner copper layer 27                 | Copper DRC rules               |
| In28_Cu       | Inner copper layer 28                 | Copper DRC rules               |
| In29_Cu       | Inner copper layer 29                 | Copper DRC rules               |
| In30_Cu       | Inner copper layer 30                 | Copper DRC rules               |
| B_Cu          | Back copper layer                     | Copper DRC rules               |
| B_Adhesive    | Back adhesive layer                   | Not used                       |
| F_Adhesive    | Front adhesive layer                  | Not used                       |
| B_Paste       | Back solder paste layer               | Not used                       |
| F_Paste       | Front solder paste layer              | Not used                       |
| B_Silkscreen  | Back silk screen layer                | Not used                       |
| F_Silkscreen  | Front silk screen layer               | Not used                       |
| B_Mask        | Back solder mask layer                | Not used                       |
| F_Mask        | Front solder mask layer               | Not used                       |
| User_Drawings | User drawing layer                    | Not used                       |
| User_Comments | User comment layer                    | Not used                       |
| User_Eco1     | User engineering change order layer 1 | Not used                       |
| User_Eco2     | User engineering change order layer 2 | Not used                       |
| Edge_Cu       | Board outline layer                   | Placement and copper DRC rules |
| F_Courtyard   | Footprint front courtyard layer       | Placement DRC rules            |
| B_Courtyard   | Footprint back courtyard layer        | Placement DRC rules            |
| F_Fab         | Footprint front fabrication layer     | Not used                       |
| B_Fab         | Footprint back fabrication layer      | Not used                       |
| User_1        | User definable layer 1                | Not used                       |
| User_2        | User definable layer 2                | Not used                       |
| User_3        | User definable layer 3                | Not used                       |
| User_4        | User definable layer 4                | Not used                       |
| User_5        | User definable layer 5                | Not used                       |
| User_6        | User definable layer 6                | Not used                       |
| User_7        | User definable layer 7                | Not used                       |
| User_8        | User definable layer 8                | Not used                       |
| User_9        | User definable layer 9                | Not used                       |

### Placement Layers

<u> Declaration:</u> **enum** _EPLayer_

Placement Layers constants are defined for specifying which side the component is placed. All components are expected to be placed on either the top or the bottom layer (that is to say, cannot be embedded within layers).

| Name | Description  |
| ---- | ------------ |
| F_Cu | Top layer    |
| B_Cu | Bottom layer |

### Colour

<u> Declaration:</u> **struct** _TColor_

Colour properties for rendering in the viewer.

| Property | Type    | Description |
| -------- | ------- | ----------- |
| r        | uint8_t | red         |
| g        | uint8_t | green       |
| b        | uint8_t | blue        |
| a        | uint8_t | alpha       |
