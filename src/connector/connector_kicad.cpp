#include <fstream>
#include <iostream>
#include <cctype>
#include "connector_kicad.h"

using namespace Pepcb;

ConnectorKicadImporter::ConnectorKicadImporter(std::string filename)
{
    std::ifstream netlist_file(filename);
    std::cout << filename << std::endl;
    char c = netlist_file.get();
    int nl = -1;
    bool ncflag = false;
    while (netlist_file.good())
    {
        switch (c)
        {
        case '(':
            nl++;
            break;
        case ')':
            nl--;
            if (ncflag)
            {
                std::cout << std::endl;
                for (int i = 0; i < nl; i++)
                    std::cout << "  ";
            }
            ncflag = false;
            break;
        case ' ':
            if (ncflag)
            {
                std::cout << std::endl;
                for (int i = 0; i < nl; i++)
                    std::cout << "  ";
            }
            ncflag = false;
            break;
        case '\"':
            std::cout << c;
            c = netlist_file.get();

            while (c != '\"')
            {
                std::cout << c;
                c = netlist_file.get();
            }
            std::cout << c;

            break;
        default:
            if (isprint(c))
            {
                std::cout << c;
                ncflag = true;
            }
        }
        c = netlist_file.get();
    }
    netlist_file.close();
}