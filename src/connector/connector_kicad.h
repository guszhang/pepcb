#pragma once

#include <map>
#include <vector>
#include "core.h"

namespace Pepcb
{

    typedef struct TPCBElement
    {
        std::vector<std::pair<std::string, TPCBElement *>> values;
    } TPCBElement;

    class ConnectorKicadImporter
    {
    public:
        ConnectorKicadImporter(std::string filename);
        CoreCircuit::CircuitDetails ImportCircuit(void);

    private:
        std::string _input_file_buffer;
        TPCBElement _root;
        TPCBElement* StringToTree(std::string, int);
        void PrintTree(TPCBElement* node, int level);
        
    };
}