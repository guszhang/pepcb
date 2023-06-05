#pragma once

#ifndef S_EXPR_H
#define S_EXPR_H

#include <string>
#include <vector>

#include "../core/core.h"

class SENode
{
public:
    SENode();
    SENode(std::string _value);
    ~SENode();

    bool isValue;
    std::string value;
    std::vector<SENode> children;

    std::string getValueStr();
    PEPCB::Base::TDim getValueDim();
    PEPCB::Base::TAngle getValueAngle();
    PEPCB::Base::ELayer getValueELayer();

    SENode getChild(std::string child_str);
};

class SEReader
{
public:
    SEReader(std::string _se_string);

    std::string se_string;
    SENode root;
    SENode fetch(std::string::iterator &it_ch);
};

#endif