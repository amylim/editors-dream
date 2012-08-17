#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <string>

using namespace std;

struct Node
{
    Node();
    int count;
    string data;
    int par;
    int line;

    Node(const Node &other);
    Node& operator=(const Node &other);
    void copy(const Node &other);

    friend ostream& operator<<(ostream &out, Node &x);
    friend bool operator<(const Node &x, const Node &y);
    friend bool operator>(const Node &x, const Node &y);

};

#endif // NODE_H
