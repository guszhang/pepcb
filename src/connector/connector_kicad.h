#pragma once

#include <map>
#include "core.h"

namespace Pepcb
{

    typedef struct TPCBElement
    {
        std::map<std::string, TPCBElement*> values;
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
        
    };
}