#include <vector>
#include <iostream>
#include "connector_kicad.h"
#include "core.h"

using namespace Pepcb;

int main()
{
    ConnectorKicadImporter circuit("res/ver1_pri.net");
    //circuit.PrintTree(circuit.root, 0);

    //CoreCircuit::CircuitDetails *circuit1 = circuit.ImportCircuit();

    circuit.PrintTree(circuit.LoadFootprint("Resistor_SMD:R_0402_1005Metric"), 0);
    // for (auto it = circuit1->net_list.begin(); it < circuit1->net_list.end(); it++)
    // {
    //     std::cout << *it << std::endl;
    // }

    // for (auto it = circuit1->part_ref.begin(); it != circuit1->part_ref.end(); it++)
    // {
    //     std::cout << it->second << ":" << it->first << std::endl;
    // }

    return 0;
}