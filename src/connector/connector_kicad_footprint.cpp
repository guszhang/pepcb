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

    footprint.name = reader.root.children[1].getValueStr();

    for (auto it = reader.root.children.begin(); it < reader.root.children.end(); it++)
    {
        if (it->value == "fp_line")
        {
            PEPCB::Base::TLine line;
            PEPCB::Base::ELayer layer;
            line.type = PEPCB::Base::LINE;
            for (auto it_a = it->children.begin(); it_a < it->children.end(); it_a++)
            {
                if (it_a->value == "start")
                {
                    line.a.X = it_a->children[1].getValueDim();
                    line.a.Y = it_a->children[2].getValueDim();
                }
                else if (it_a->value == "end")
                {
                    line.b.X = it_a->children[1].getValueDim();
                    line.b.Y = it_a->children[2].getValueDim();
                }
                else
                    // since KiCAD 7
                    if (it_a->value == "stroke")
                    {
                        line.width = it_a->children[1].children[1].getValueDim();
                    }
                    else
                        // KiCAD 6
                        if (it_a->value == "width")
                        {
                            line.width = it_a->children[1].getValueDim();
                        }
                        else if (it_a->value == "layer")
                        {
                            layer = it_a->children[1].getValueELayer();
                        }
            }
            footprint.insertGeometry(layer, line);
        } else 
        if (it->value=="pad") {
            std::string pad_name=it->children[1].getValueStr();
            if(it->children[2].value=="smd"){
                PEPCB::Base::TPolygon polygon;
                PEPCB::Base::TVertex centre;
                PEPCB::Base::TAngle angle=0;
                PEPCB::Base::TDim sizeX, sizeY;
                std::vector<PEPCB::Base::ELayer> layers;
                if(it->children[3].value=="roundrect" || it->children[3].value=="rect"){
                    for(auto it_a=it->children.begin();it_a<it->children.end();it_a++){
                        if(it_a->value=="at"){
                            centre.X=it_a->children[1].getValueDim();
                            centre.Y=it_a->children[2].getValueDim();
                            if(it_a->children.size()>3){
                                angle=it_a->children[3].getValueAngle();
                            }
                        } else
                        if(it_a->value=="size"){
                            sizeX=it_a->children[1].getValueDim();
                            sizeY=it_a->children[2].getValueDim();
                        } else
                        if(it_a->value=="layers"){
                            for(auto it_b=it_a->children.begin()+1; it_b<it_a->children.end();it_b++){
                            layers.push_back(it_b->getValueELayer());
                            }
                        }
                    }
                    
                }
            }
        }

    }

    return footprint;
}
