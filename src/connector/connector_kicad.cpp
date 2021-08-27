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
    //std::cout << this->_input_file_buffer << std::endl;
    TPCBElement *root = StringToTree(this->_input_file_buffer, 0);
    PrintTree(root, 0);
    netlist_file.close();
}

TPCBElement *ConnectorKicadImporter::StringToTree(std::string str, int pos)
{
    TPCBElement *root = new TPCBElement;
    TPCBElement *node_pointer = root;
    TPCBElement *new_node;
    std::stack<TPCBElement *> node_stack;
    int string_length = str.length();
    std::string attribute = "";
    while (pos < string_length)
    {
        switch (str[pos])
        {
        case ')':
            if (attribute.length() > 0)
            {
                node_pointer->values.push_back(std::pair<std::string, TPCBElement *>(attribute, nullptr));
                attribute = "";
            }
            node_pointer = node_stack.top();
            node_stack.pop();
            while(str[pos+1]==' ')pos++;
            break;
        case '(':
            while(str[pos+1]==' ')pos++;
            new_node = new TPCBElement;
            attribute = "";
            while (str[++pos] != ' ')
                attribute += str[pos];
            node_pointer->values.push_back(std::pair<std::string, TPCBElement *>(attribute, new_node));
            attribute = "";
            node_stack.push(node_pointer);
            node_pointer = new_node;
            break;
        case ' ':
            if (attribute.length() > 0)
            {
                node_pointer->values.push_back(std::pair<std::string, TPCBElement *>(attribute, nullptr));
                attribute = "";
            }
            while(str[pos+1]==' ')pos++;
            break;
        case '\"':
            while(str[++pos]!='\"') attribute+=str[pos];
            break;
        case '\n':
        case '\r':
            break;
        default:
            attribute += str[pos];
            break;
        }
        pos++;
    }
    return root;
}

void ConnectorKicadImporter::PrintTree(TPCBElement *node, int level)
{
    int i;
    std::vector<std::pair<std::string, TPCBElement *>>::iterator it;
    //std::cout << "@Elements=" << node->values.size() << std::endl;
    for (it = node->values.begin(); it != node->values.end(); ++it)
    {
        for (i = 0; i < level; i++)
            std::cout << "  ";
        std::cout << it->first << std::endl;
        if (it->second != nullptr)
        {
            PrintTree(it->second, level+1);
        }
    }
    return;
}