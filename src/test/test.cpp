#include "connector_kicad.h"

int main()
{
    Pepcb::ConnectorKicadImporter circuit("res/M3_Mounting_Hole.kicad_mod");
    return 0;
}