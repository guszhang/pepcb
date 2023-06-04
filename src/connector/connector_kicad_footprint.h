#pragma once

#ifndef CONNECTOR_KICAD_FOOTPRINT_H
#define CONNECTOR_KICAD_FOOTPRINT_H

#include "../core/core.h"

#include <cstring>
#include <iostream>

namespace PEPCB {
    namespace Connector {
        class KicadFootprintLoader{
        public:
            KicadFootprintLoader(std::string _kicad_footprint_directory);
            ~KicadFootprintLoader();

            PEPCB::Base::TFootprint fetch_footprint(std::string _library_directory, std::string _footprint_name);
        private:
            std::string kicad_footprint_directory;
        };
    }
}

#endif