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

TPCBElement* ConnectorKicadImporter::StringToTree(std::string str, int pos)
{
    TPCBElement* new_node=new TPCBElement;
    int string_length = str.length();
    std::string attribute;
    while(pos<string_length){
        switch(str[pos]){
            case ')':
                new_node->values.insert(std::pair<std::string, TPCBElement*>(attribute, nullptr));
                return new_node;
                break;
            case '(':
                new_node->values.insert(std::pair<std::string, TPCBElement*>(attribute, StringToTree(str, pos++)));
                break;
            case ' ':
                new_node->values.insert(std::pair<std::string, TPCBElement*>(attribute, nullptr));
                break;
            default:
                attribute+=str[pos];
                break;
        }
        pos++;
    }
    return new_node;
}