#pragma once

#include "core.h"

namespace Pepcb
{
    class ConnectorKicadImporter
    {
    public:
        ConnectorKicadImporter(std::string filename);
        CoreCircuit::CircuitDetails ImportCircuit();
    };
}