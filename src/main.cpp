#include "connector/connector_kicad_footprint.h"
#include "ui/ui.h"

int main(int argc, char **argv)
{
    PEPCB::Connector::KicadFootprintLoader loader(".");
    loader.fetchFootprint("res/kicad/dummy_project/Library.pretty", "dummy_footprint1.kicad_mod");
    PEPCB::UI::loadUILayers();
    PEPCB::UI::startUI(1600, 900);
    return 0;
}