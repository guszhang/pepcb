#include <fstream>
#include <iostream>
#include <cctype>
#include <stack>
#include "connector_kicad.h"

using namespace Pepcb;

ConnectorKicadImporter::ConnectorKicadImporter(std::string filename)
{
    std::ifstream netlist_file(filename);
    std::cout << filename << std::endl;
    std::string line_buffer;
    this->_input_file_buffer = "";
    if (netlist_file.is_open())
    {
        while (getline(netlist_file, line_buffer))
        {
            this->_input_file_buffer.append(line_buffer);
        }
    }
    std::cout << this->_input_file_buffer << std::endl;
    netlist_file.close();
}

TPCBElement ConnectorKicadImporter::StringToTree(std::string str, int pos)
{
    TPCBElement new_node;
    while(str[pos]!=')'){
        
    }
}