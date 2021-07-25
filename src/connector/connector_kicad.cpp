#include <fstream>
#include <iostream>
#include <cctype>
#include <stack>
#include "connector_kicad.h"

using namespace Pepcb;
using namespace std;



ConnectorKicadImporter::ConnectorKicadImporter(string filename)
{
    ifstream netlist_file(filename);
    cout << filename << endl;
    string line_buffer;
    this->_input_file_buffer = "";
    if (netlist_file.is_open())
    {
        while (getline(netlist_file, line_buffer))
        {
            this->_input_file_buffer.append(line_buffer);
        }
    }
    cout << this->_input_file_buffer << endl;
    netlist_file.close();
}