#include <vector>
#include <iostream>
#include "connector_kicad.h"

using namespace Pepcb;

int main()
{
    ConnectorKicadImporter circuit("res/ver1_pri.net");
    //circuit.PrintTree(circuit.root, 0);

    auto node_list = circuit.FetchElement(circuit.root, "comp");
    for (auto it = node_list.begin(); it < node_list.end(); it++)
    {
        std::cout << "Component " << it-node_list.begin() << " : " << (*it)->values[2].second->values[0].first <<std::endl;
    }

    return 0;
}