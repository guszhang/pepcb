#include "s_expr.h"

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
}

SENode SEReader::fetch(std::string::iterator &it_ch)
{
    SENode node;
    while (*it_ch != '(' && it_ch < this->se_string.end())
        it_ch++;
    while (*it_ch != ')' && it_ch < this->se_string.end())
    {
        it_ch++;
        std::string str;
        if (*it_ch == '(')
        {
            node.children.push_back(fetch(it_ch));
        }
        else if (*it_ch != ' ' || *it_ch != ')')
        {
            str += *it_ch;
        }
        else
        {
            node.children.push_back(SENode(str));
        }
    }
    it_ch++;
    node.isValue = false;
    node.value = node.children[0].value;
    return node;
}