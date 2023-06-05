#include "connector/connector_kicad_footprint.h"


int main(int argc, char** argv){
    PEPCB::Connector::KicadFootprintLoader loader(".");
    loader.fetch_footprint("res/kicad/dummy_project/Library.pretty","dummy_footprint1.kicad_mod");

    return 0;
}