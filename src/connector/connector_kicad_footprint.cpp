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

PEPCB::Base::TFootprint KicadFootprintLoader::fetchFootprint(std::string _library_directory, std::string _footprint_name)
{
    PEPCB::Base::TFootprint footprint;
    std::string filepath = this->kicad_footprint_directory + "/" + _library_directory + "/" + _footprint_name;
    std::string footprint_str = PEPCB::FS::readFromFile(filepath);
    SEReader reader(footprint_str);

    footprint.name = reader.root.children[1].getValueStr();

    for (auto it = reader.root.children.begin(); it < reader.root.children.end(); it++)
    {
        if (it->value == "fp_line")
        {
            PEPCB::Base::TVertex a, b;
            PEPCB::Base::TDim width;
            PEPCB::Base::ELayer layer;
            for (auto it_a = it->children.begin(); it_a < it->children.end(); it_a++)
            {
                if (it_a->value == "start")
                {
                    a.X = it_a->children[1].getValueDim();
                    a.Y = -it_a->children[2].getValueDim();
                }
                else if (it_a->value == "end")
                {
                    b.X = it_a->children[1].getValueDim();
                    b.Y = -it_a->children[2].getValueDim();
                }
                else
                    // since KiCAD 7
                    if (it_a->value == "stroke")
                    {
                        width = it_a->children[1].children[1].getValueDim();
                    }
                    else
                        // KiCAD 6
                        if (it_a->value == "width")
                        {
                            width = it_a->children[1].getValueDim();
                        }
                        else if (it_a->value == "layer")
                        {
                            layer = it_a->children[1].getValueELayer();
                        }
            }
            PEPCB::Base::TLine line(a, b, width);
            footprint.insertGeometry(layer, line);
        }
        else if (it->value == "pad")
        {
            std::string pad_name = it->children[1].getValueStr();
            if (it->children[2].value == "smd")
            {
                PEPCB::Base::TVertex centre;
                PEPCB::Base::TAngle angle = 0;
                PEPCB::Base::TDim sizeX, sizeY;
                PEPCB::Base::TCopper pad_copper;
                double round_ratio;
                std::vector<PEPCB::Base::ELayer> layers;
                if (it->children[3].value == "rect")
                {
                    for (auto it_a = it->children.begin(); it_a < it->children.end(); it_a++)
                    {
                        if (it_a->value == "at")
                        {
                            centre.X = it_a->children[1].getValueDim();
                            centre.Y = -it_a->children[2].getValueDim();
                            if (it_a->children.size() > 3)
                            {
                                angle = it_a->children[3].getValueAngle();
                                std::cout << "Angle of pad: " << angle << std::endl;
                            }
                        }
                        else if (it_a->value == "size")
                        {
                            sizeX = it_a->children[1].getValueDim();
                            sizeY = it_a->children[2].getValueDim();
                        }
                        else if (it_a->value == "layers")
                        {
                            for (auto it_b = it_a->children.begin() + 1; it_b < it_a->children.end(); it_b++)
                            {
                                layers.push_back(it_b->getValueELayer());
                            }
                        }
                    }
                    PEPCB::Base::TRect rect(centre, sizeX, sizeY, angle);
                    for (auto it_a = layers.begin(); it_a < layers.end(); it_a++)
                    {
                        footprint.insertGeometry(*it_a, rect);
                        pad_copper.addGeometry(*it_a, rect);
                    }
                    footprint.pad_list.insert(std::pair<std::string, PEPCB::Base::TCopper>(pad_name, pad_copper));
                }
                else if (it->children[3].value == "roundrect")
                {
                    for (auto it_a = it->children.begin(); it_a < it->children.end(); it_a++)
                    {
                        if (it_a->value == "at")
                        {
                            centre.X = it_a->children[1].getValueDim();
                            centre.Y = -it_a->children[2].getValueDim();
                            if (it_a->children.size() > 3)
                            {
                                angle = it_a->children[3].getValueAngle();
                                std::cout << "Angle of pad: " << angle << std::endl;
                            }
                        }
                        else if (it_a->value == "size")
                        {
                            sizeX = it_a->children[1].getValueDim();
                            sizeY = it_a->children[2].getValueDim();
                        }
                        else if (it_a->value == "layers")
                        {
                            for (auto it_b = it_a->children.begin() + 1; it_b < it_a->children.end(); it_b++)
                            {
                                layers.push_back(it_b->getValueELayer());
                            }
                        }
                        else if (it_a->value == "roundrect_rratio")
                        {
                            round_ratio = it_a->children[1].getValueDouble();
                        }
                    }
                    PEPCB::Base::TRectRound rect(centre, sizeX, sizeY, angle, round_ratio);
                    for (auto it_a = layers.begin(); it_a < layers.end(); it_a++)
                    {
                        footprint.insertGeometry(*it_a, rect);
                        pad_copper.addGeometry(*it_a, rect);
                    }
                    footprint.pad_list.insert(std::pair<std::string, PEPCB::Base::TCopper>(pad_name, pad_copper));
                }
            }
        }
    }

    return footprint;
}
