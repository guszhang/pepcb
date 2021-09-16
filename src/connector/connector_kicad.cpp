#include <fstream>
#include <iostream>
#include <cctype>
#include <stack>
#include <algorithm>
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
    netlist_file.close();
    this->_input_file_buffer.erase(std::remove(this->_input_file_buffer.begin(), this->_input_file_buffer.end(), '\n'), this->_input_file_buffer.end());
    this->_input_file_buffer.erase(std::remove(this->_input_file_buffer.begin(), this->_input_file_buffer.end(), '\r'), this->_input_file_buffer.end());
    this->_input_file_buffer.erase(std::remove(this->_input_file_buffer.begin(), this->_input_file_buffer.end(), '\t'), this->_input_file_buffer.end());

    this->root = StringToTree(this->_input_file_buffer, 0);
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
            while (str[pos + 1] == ' ')
                pos++;
            break;
        case '(':
            while (str[pos + 1] == ' ')
                pos++;
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
            while (str[pos + 1] == ' ')
                pos++;
            break;
        case '\"':
            while (str[++pos] != '\"')
                attribute += str[pos];
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
    for (it = node->values.begin(); it != node->values.end(); it++)
    {
        for (i = 0; i < level; i++)
            std::cout << "  ";
        std::cout << it->first << std::endl;
        if (it->second != nullptr)
        {
            PrintTree(it->second, level + 1);
        }
    }
    return;
}

std::vector<TPCBElement *> ConnectorKicadImporter::FetchElement(TPCBElement *node, std::string element)
{
    std::vector<TPCBElement *> node_list, child_list;
    std::vector<std::pair<std::string, TPCBElement *>>::iterator it;
    for (it = node->values.begin(); it != node->values.end(); it++)
    {
        if (it->first == element)
        {
            node_list.push_back(it->second);
        }
        if (it->second != nullptr)
        {
            child_list = FetchElement(it->second, element);
            node_list.insert(node_list.end(), child_list.begin(), child_list.end());
        }
    }
    return node_list;
}

CoreCircuit::CircuitDetails *ConnectorKicadImporter::ImportCircuit(void)
{
    CoreCircuit::CircuitDetails *circuit_details = new CoreCircuit::CircuitDetails;

    auto libpart_list = FetchElement(this->root, "libpart");
    std::unordered_map<std::string, std::map<int, std::string>> libpart_map;
    for (auto it = libpart_list.begin(); it < libpart_list.end(); it++)
    {
        std::vector<std::string> lib_part_tuples = {};
        lib_part_tuples.push_back((*it)->values[0].second->values[0].first + ":" + (*it)->values[1].second->values[0].first);
        auto libpart_alias_list = FetchElement(*it, "alias");
        for (auto it_alias = libpart_alias_list.begin(); it_alias < libpart_alias_list.end(); it_alias++)
        {
            lib_part_tuples.push_back((*it)->values[0].second->values[0].first + ":" + (*it_alias)->values[0].first);
        }
        std::map<int, std::string> lib_part_pin_map = {};
        for (auto it_attr = (*it)->values.begin(); it_attr < (*it)->values.end(); it_attr++)
        {
            if (it_attr->first == "pins")
            {
                for (auto it_pin = it_attr->second->values.begin(); it_pin < it_attr->second->values.end(); it_pin++)
                {
                    // std::cout << it_pin->second->values[0].first << std::endl;
                    lib_part_pin_map.insert(std::pair<int, std::string>(std::stoi(it_pin->second->values[0].second->values[0].first), it_pin->second->values[1].second->values[0].first));
                }
                break;
            }
        }
        for (auto it_libpart_tuple = lib_part_tuples.begin(); it_libpart_tuple < lib_part_tuples.end(); it_libpart_tuple++)
        {
            libpart_map.insert(std::pair<std::string, std::map<int, std::string>>(*it_libpart_tuple, lib_part_pin_map));
            std::cout << *it_libpart_tuple << std::endl;
        }
    }

    // for (auto it = libpart_map.begin(); it != libpart_map.end(); it++)
    // {
    //     std::cout << "Part_model: " << it->first << std::endl;
    //     for (auto it_pin = it->second.begin(); it_pin != it->second.end(); it_pin++)
    //     {
    //         std::cout << "  Pin: " << it_pin->first << " Pad: " << it_pin->second << std::endl;
    //     }
    // }

    std::unordered_map<std::string, std::map<int, int>> pin_to_pad_map;
    auto part_list = FetchElement(this->root, "comp");
    for (auto it = part_list.begin(); it < part_list.end(); it++)
    {
        std::string lib_part_tuple;
        for (auto it_attr = (*it)->values.begin(); it_attr < (*it)->values.end(); it_attr++)
        {
            if (it_attr->first == "libsource")
            {
                lib_part_tuple = it_attr->second->values[0].second->values[0].first + ":" + it_attr->second->values[1].second->values[0].first;
                break;
            }
        }

        CoreCircuit::TParts part = {
            .name = (*it)->values[0].second->values[0].first,
            .value = (*it)->values[1].second->values[0].first,
        };
        auto pin_map = libpart_map[lib_part_tuple];
        std::map<int, int> part_pin_to_pad_map;
        int pin_index = 0;
        for (auto it_pin = pin_map.begin(); it_pin != pin_map.end(); it_pin++)
        {
            part_pin_to_pad_map.insert(std::pair<int, int>(it_pin->first, pin_index));
            pin_index++;
            // part.pad_list.push_back(CoreCircuit::TPad(

            // ))
        }
        circuit_details->part_list.push_back(part);
        circuit_details->part_ref.insert(std::pair<std::string, int>(part.name, it - part_list.begin()));
        pin_to_pad_map.insert(std::pair<std::string, std::map<int, int>>(part.name, part_pin_to_pad_map));
    }

    for (auto it = pin_to_pad_map.begin(); it != pin_to_pad_map.end(); it++)
    {
        std::cout << "Part_ref: " << it->first << " Part_id: " << circuit_details->part_ref[it->first] << std::endl;
        for (auto it_pin = it->second.begin(); it_pin != it->second.end(); it_pin++)
        {
            std::cout << "  Pin: " << it_pin->first << " Pad: " << it_pin->second << std::endl;
        }
    }
    std::vector<std::pair<int, int>> node_list;
    auto net_list = FetchElement(this->root, "net");
    for (auto it = net_list.begin(); it < net_list.end(); it++)
    {
        node_list.clear();
        circuit_details->net_list.push_back((*it)->values[1].second->values[0].first);
        std::cout << "Net: " << (*it)->values[1].second->values[0].first << std::endl;
        for (auto it_node = (*it)->values.begin() + 2; it_node < (*it)->values.end(); it_node++)
        {
            std::string part_ref = it_node->second->values[0].second->values[0].first;
            int pin_ref = stoi(it_node->second->values[1].second->values[0].first);
            //std::cout << part_ref << " " << pin_ref << std::endl;
            auto pin_ref_map = pin_to_pad_map[part_ref];
            node_list.push_back(std::pair<int, int>(circuit_details->part_ref[part_ref], pin_ref_map[pin_ref]));
            std::cout << circuit_details->part_ref[part_ref] << " " << pin_ref_map[pin_ref] << std::endl;
        }
        std::cout << std::endl;
    }
    return circuit_details;
}