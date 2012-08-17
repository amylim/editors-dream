#ifndef HEAP_H
#define HEAP_H

#include <iostream>
#include <iomanip>
#include <vector>
#include "node.h"

using namespace std;

//minimum heap
class Heap
{
public:
    Heap(int c = 1);
    ~Heap();
    Heap(Heap &other);
    Heap& operator=(Heap &other);

    void insert(string x, int p, int l);
    void swap(Node &a, Node &b);
    Node removeMin();
    int leftChild(int i);
    int rightChild(int i);
    int parent(int i);

    int numOfElements();
    int indexOfMin(int x, int y);
    void resizeUp();
    void resizeDown();

    friend ostream& operator<<(ostream &out, Heap &t);

private:
    vector<Node> myHeap;
    int size;
    int qty;
    void copy(Heap &other);
};

#endif // HEAP_H
