#include "connector_kicad.h"

int main()
{
    Pepcb::ConnectorKicadImporter circuit("res/kicad/ver1.net");
    return 0;
}