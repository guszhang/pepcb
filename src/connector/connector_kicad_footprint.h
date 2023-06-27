#pragma once

#ifndef CONNECTOR_KICAD_FOOTPRINT_H
#define CONNECTOR_KICAD_FOOTPRINT_H

#include "../core/core.h"

#include <cstring>
#include <iostream>

namespace PEPCB
{
    namespace Connector
    {
        class KicadFootprintLoader
        {
        public:
            KicadFootprintLoader(std::string _kicad_footprint_directory);
            ~KicadFootprintLoader();

            std::vector<PEPCB::Base::ELayer> enabled_copper_layers;
            void enableCopperLayer(PEPCB::Base::ELayer layer);
            PEPCB::Base::TFootprint fetchFootprint(std::string _library_directory, std::string _footprint_name);

        private:
            std::string kicad_footprint_directory;
            void addPad(std::string _pad_name, PEPCB::Base::TCopper);
        };
    }
}

#endif