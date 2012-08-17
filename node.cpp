#include "node.h"

Node::Node()
{
    count = 0;
    data = "\0";
    par = 0;
    line = 0;
}

Node::Node(const Node &other)
{
    copy(other);
}

Node& Node::operator=(const Node &other)
{
    if(this != &other)
        copy(other);
    return *this;
}

void Node::copy(const Node &other)
{
    count = other.count;
    data = other.data;
    par = other.par;
    line = other.line;
}

ostream& operator<<(ostream &out, Node &t)
{
    out << t.data << " " << t.par << ", " << t.line;
    return out;
}

bool operator<(const Node &x, const Node &y)
{
    if(x.data == y.data)
    {
        if(x.par == y.par)
            return x.line < y.line;
        else
            return x.par < y.par;
    }
    else
        return x.data < y.data;
}

bool operator>(const Node &x, const Node &y)
{
    if(x.data == y.data)
    {
        if(x.par == y.par)
            return x.line > y.line;
        else
            return x.par > y.par;
    }
    else
        return x.data > y.data;
}
