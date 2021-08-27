#pragma once

#include <map>
#include <vector>
#include "core.h"

namespace Pepcb
{
    class TPCBElement
    {
    public:
        std::vector<std::pair<std::string, TPCBElement *>> values;
    };

    class ConnectorKicadImporter
    {
    public:
        TPCBElement* root;

        ConnectorKicadImporter(std::string filename);
        CoreCircuit::CircuitDetails* ImportCircuit(void);
        void PrintTree(TPCBElement* node, int level);
        std::vector<TPCBElement*> FetchElement(TPCBElement *node, std::string element);

    private:
        std::string _input_file_buffer;
        TPCBElement _root;
        TPCBElement* StringToTree(std::string, int);
        
    };
}