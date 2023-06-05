#include "s_expr.h"

#include <cstdio>

const char *ANSI_RESET = "\x1b[24m";
const char *ANSI_HIGHLIGHT = "\x1b[4m";

void printTree(SENode node, int level)
{
    for (int i = 0; i < level; i++)
    {
        printf("  ");
    }
    if (node.isValue)
    {
        if (node.value != "")
            printf("%s --D> %lld --L> %d\r\n", node.value.c_str(), node.getValueDim(), node.getValueELayer());
        else
            printf("{empty string}\r\n");
    }
    else
    {
        printf("%s%s%s\r\n", ANSI_HIGHLIGHT, node.value.c_str(), ANSI_RESET);
        for (auto it = node.children.begin(); it < node.children.end(); it++)
        {
            printTree(*it, level + 1);
        }
    }
}

SENode::SENode()
{
    this->isValue = true;
    this->value = "";
}

SENode::SENode(std::string _value)
{
    this->isValue = true;
    this->value = _value;
}

SENode::~SENode()
{
    // do nothing
}

SEReader::SEReader(std::string _se_string)
{
    this->se_string = _se_string;
    std::string::iterator it_ch = this->se_string.begin();
    this->root = fetch(it_ch);
    // printTree(this->root, 0);
}

SENode SEReader::fetch(std::string::iterator &it_ch)
{
    SENode node;
    std::string str = "";
    while (*it_ch != '(' && it_ch < this->se_string.end())
        it_ch++;
    while (*it_ch != ')' && it_ch < this->se_string.end())
    {
        it_ch++;
        if (*it_ch == '(')
        {
            node.children.push_back(fetch(it_ch));
        }
        if (*it_ch == '\"')
        {
            str += *it_ch++;
            while (*it_ch != '\"')
            {
                str += *it_ch++;
            }
            str += *it_ch;
        }
        else if (*it_ch != ' ' && *it_ch != ')')
        {
            if (*it_ch != '\r' && *it_ch != '\n' && *it_ch != '\t')
                str += *it_ch;
        }
        else
        {
            if (str != "")
                node.children.push_back(SENode(str));
            str = "";
        }
    }
    it_ch++;
    node.isValue = false;
    node.value = node.children[0].value;
    return node;
}

std::string SENode::getValueStr()
{
    std::string ret_str;
    if (this->value.front() == '"' && (this->value.back()) == '"')
    {
        ret_str = this->value.substr(1, this->value.length() - 2);
    }
    else
    {
        ret_str = this->value;
    }
    return ret_str;
}

PEPCB::Base::TDim SENode::getValueDim()
{
    PEPCB::Base::TDim dimension = 0;
    bool before_decimal = true;
    int64_t mul_after_decimal = 100000000;
    int sign = 1;
    for (auto it = this->value.begin(); it < this->value.end(); it++)
    {
        if (*it == '-')
        {
            sign = sign * -1;
        }
        else if (*it >= '0' && *it <= '9')
        {
            if (before_decimal)
            {

                dimension += dimension * 10 + (*it - '0') * 1000000000;
            }
            else
            {
                dimension += (*it - '0') * mul_after_decimal;
                mul_after_decimal /= 10;
            }
        }
        else if (*it == '.')
        {
            before_decimal = false;
        }
    }
    dimension *= sign;
    return dimension;
}

PEPCB::Base::TAngle SENode::getValueAngle()
{
    PEPCB::Base::TAngle angle = this->getValueDim() / 100000000;
    return angle;
}

PEPCB::Base::ELayer SENode::getValueELayer()
{
    PEPCB::Base::ELayer layer = PEPCB::Base::UNRECOGNISED;
    if (this->getValueStr() == "F.Cu")
        layer = PEPCB::Base::F_CU;
    if (this->getValueStr() == "In1.Cu")
        layer = PEPCB::Base::IN1_CU;
    if (this->getValueStr() == "In2.Cu")
        layer = PEPCB::Base::IN2_CU;
    if (this->getValueStr() == "In3.Cu")
        layer = PEPCB::Base::IN3_CU;
    if (this->getValueStr() == "In4.Cu")
        layer = PEPCB::Base::IN4_CU;
    if (this->getValueStr() == "In5.Cu")
        layer = PEPCB::Base::IN5_CU;
    if (this->getValueStr() == "In6.Cu")
        layer = PEPCB::Base::IN6_CU;
    if (this->getValueStr() == "In7.Cu")
        layer = PEPCB::Base::IN7_CU;
    if (this->getValueStr() == "In8.Cu")
        layer = PEPCB::Base::IN8_CU;
    if (this->getValueStr() == "In9.Cu")
        layer = PEPCB::Base::IN9_CU;
    if (this->getValueStr() == "In10.Cu")
        layer = PEPCB::Base::IN10_CU;
    if (this->getValueStr() == "In11.Cu")
        layer = PEPCB::Base::IN11_CU;
    if (this->getValueStr() == "In12.Cu")
        layer = PEPCB::Base::IN12_CU;
    if (this->getValueStr() == "In13.Cu")
        layer = PEPCB::Base::IN13_CU;
    if (this->getValueStr() == "In14.Cu")
        layer = PEPCB::Base::IN14_CU;
    if (this->getValueStr() == "In15.Cu")
        layer = PEPCB::Base::IN15_CU;
    if (this->getValueStr() == "In16.Cu")
        layer = PEPCB::Base::IN16_CU;
    if (this->getValueStr() == "In17.Cu")
        layer = PEPCB::Base::IN17_CU;
    if (this->getValueStr() == "In18.Cu")
        layer = PEPCB::Base::IN18_CU;
    if (this->getValueStr() == "In19.Cu")
        layer = PEPCB::Base::IN19_CU;
    if (this->getValueStr() == "In20.Cu")
        layer = PEPCB::Base::IN20_CU;
    if (this->getValueStr() == "In21.Cu")
        layer = PEPCB::Base::IN21_CU;
    if (this->getValueStr() == "In22.Cu")
        layer = PEPCB::Base::IN22_CU;
    if (this->getValueStr() == "In23.Cu")
        layer = PEPCB::Base::IN23_CU;
    if (this->getValueStr() == "In24.Cu")
        layer = PEPCB::Base::IN24_CU;
    if (this->getValueStr() == "In25.Cu")
        layer = PEPCB::Base::IN25_CU;
    if (this->getValueStr() == "In26.Cu")
        layer = PEPCB::Base::IN26_CU;
    if (this->getValueStr() == "In27.Cu")
        layer = PEPCB::Base::IN27_CU;
    if (this->getValueStr() == "In28.Cu")
        layer = PEPCB::Base::IN28_CU;
    if (this->getValueStr() == "In29.Cu")
        layer = PEPCB::Base::IN29_CU;
    if (this->getValueStr() == "In30.Cu")
        layer = PEPCB::Base::IN30_CU;
    if (this->getValueStr() == "B.Cu")
        layer = PEPCB::Base::B_CU;

    if (this->getValueStr() == "B.Adhes")
        layer = PEPCB::Base::B_ADHESIVE;
    if (this->getValueStr() == "F.Adhes")
        layer = PEPCB::Base::F_ADHESIVE;
    if (this->getValueStr() == "B.Paste")
        layer = PEPCB::Base::B_PASTE;
    if (this->getValueStr() == "F.Paste")
        layer = PEPCB::Base::F_PASTE;
    if (this->getValueStr() == "B.SilkS")
        layer = PEPCB::Base::B_SILKSCREEN;
    if (this->getValueStr() == "F.SilkS")
        layer = PEPCB::Base::F_SILKSCREEN;
    if (this->getValueStr() == "B.Mask")
        layer = PEPCB::Base::B_MASK;
    if (this->getValueStr() == "F.Mask")
        layer = PEPCB::Base::F_MASK;
    if (this->getValueStr() == "Dwgs.User")
        layer = PEPCB::Base::USER_DRAWINGS;
    if (this->getValueStr() == "Cmts.User")
        layer = PEPCB::Base::USER_COMMENTS;
    if (this->getValueStr() == "Eco1.User")
        layer = PEPCB::Base::USER_ECO1;
    if (this->getValueStr() == "Eco2.User")
        layer = PEPCB::Base::USER_ECO2;
    if (this->getValueStr() == "F.CrtYd")
        layer = PEPCB::Base::F_COURTYARD;
    if (this->getValueStr() == "B.CrtYd")
        layer = PEPCB::Base::B_COURTYARD;
    if (this->getValueStr() == "F.Fab")
        layer = PEPCB::Base::F_FAB;
    if (this->getValueStr() == "B.Fab")
        layer = PEPCB::Base::B_FAB;
    if (this->getValueStr() == "User.1")
        layer = PEPCB::Base::USER_1;
    if (this->getValueStr() == "User.2")
        layer = PEPCB::Base::USER_2;
    if (this->getValueStr() == "User.3")
        layer = PEPCB::Base::USER_3;
    if (this->getValueStr() == "User.4")
        layer = PEPCB::Base::USER_4;
    if (this->getValueStr() == "User.5")
        layer = PEPCB::Base::USER_5;
    if (this->getValueStr() == "User.6")
        layer = PEPCB::Base::USER_6;
    if (this->getValueStr() == "User.7")
        layer = PEPCB::Base::USER_7;
    if (this->getValueStr() == "User.8")
        layer = PEPCB::Base::USER_8;
    if (this->getValueStr() == "User.9")
        layer = PEPCB::Base::USER_9;
    return layer;
}