#include "connector_kicad.h"

int main()
{
    Pepcb::ConnectorKicadImporter circuit("res/ver1_pri.net");
    return 0;
}