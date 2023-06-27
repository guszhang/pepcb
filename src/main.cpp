#include "connector/connector_kicad_footprint.h"
#include "ui/ui.h"

int main(int argc, char **argv)
{
    PEPCB::Base::TFootprint fp1;
    PEPCB::Connector::KicadFootprintLoader loader(".");
    // fp1 = loader.fetchFootprint("res/kicad/dummy_project/Library.pretty", "dummy_footprint1.kicad_mod");
    // fp1 = loader.fetchFootprint("res", "R_0603_1608Metric.kicad_mod");
    fp1 = loader.fetchFootprint("res", "HDMI_A_Amphenol_10029449-x01xLF_Horizontal.kicad_mod");

    PEPCB::UI::addFootprint(fp1);
    PEPCB::UI::loadUILayers();
    PEPCB::UI::startUI(600, 600);
    return 0;
}