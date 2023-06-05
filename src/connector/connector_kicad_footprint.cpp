#include "connector_kicad_footprint.h"
#include "s_expr.h"

using namespace PEPCB::Connector;

KicadFootprintLoader::KicadFootprintLoader(std::string _kicad_footprint_directory)
{
    this->kicad_footprint_directory = _kicad_footprint_directory;
}

KicadFootprintLoader::~KicadFootprintLoader()
{
}

PEPCB::Base::TFootprint KicadFootprintLoader::fetch_footprint(std::string _library_directory, std::string _footprint_name)
{
    PEPCB::Base::TFootprint footprint;
    std::string filepath = this->kicad_footprint_directory + "/" + _library_directory + "/" + _footprint_name;
    std::string footprint_str = PEPCB::FS::readFromFile(filepath);
    SEReader reader(footprint_str);
    
    return footprint;
}
