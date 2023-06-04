#include "connector/connector_kicad_footprint.h"


int main(int argc, char** argv){
    PEPCB::Connector::KicadFootprintLoader loader(".");
    loader.fetch_footprint("res","R_0603_1608Metric.kicad_mod");

    return 0;
}