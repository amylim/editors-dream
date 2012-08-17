#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <math.h>

using namespace std;

struct WordCoord
{
    WordCoord();
    int par;
    int line;

    WordCoord(const WordCoord &other);
    WordCoord& operator=(const WordCoord &other);
    void copy(const WordCoord &other);

    friend ostream& operator<<(ostream &out, WordCoord &x);
    friend bool operator<(const WordCoord &x, const WordCoord &y);
    friend bool operator>(const WordCoord &x, const WordCoord &y);
};

struct NodeT
{
    NodeT();
    int height;
    int heightDiff;
    int count;
    string data;
    vector<WordCoord> location;

    NodeT(const NodeT &other);
    NodeT& operator=(const NodeT &other);
    void copy(const NodeT &other);
    void sortLocationAscending();
    void addLocation(int p, int l);
    void clear();

    friend ostream& operator<<(ostream &out, NodeT &x);
    friend bool operator<(const NodeT &x, const NodeT &y);
    friend bool operator>(const NodeT &x, const NodeT &y);
};

//avl tree
class Tree //: public NodeT
{
public:
    Tree();
    ~Tree();
    Tree(Tree &other);
    Tree& operator=(Tree &other);

    void insert(string x, int p, int l);
    void updateHeightInfoInsert(int newItemIndex);
    void balance(int root, int problemChild);
    int getHeightDiff(int root);
    int getLastChild(int start);

    void rightRotation(int pivot);
    void leftRotation(int pivot);
    void moveRightDown(int *storage, int count, int startRoot);
    void moveRightNext(int i, int startHeight);
    void moveRightUp(int *storage, int count, int startRoot);
    int farRight(int root, int i);
    void moveLeftDown(int *storage, int count, int startRoot);
    void moveLeftNext(int i, int startHeight);
    void moveLeftUp(int *storage, int count, int startRoot);
    int farLeft(int root, int i);
    void obtainElementsPOST(int *storage, int &count, int i);
    void obtainElementsPRE(int *storage, int &count, int i);
    void obtainElementsIN(NodeT *storage, int &count, int i);
    void sortDescending(int *array, int count);
    void sortAscending(int *array, int count);


    bool isSameHeight(int x, int y);
    bool hasSibling(int i);
    bool isRightChild(int i);
    bool isLeftChild(int i);
    int height(int i);
    void remove(int index);
    void remove(string x);
    void updateHeightInfoRemove(int oldItemIndex);

    int sibling(int i);
    int leftChild(int i);
    int rightChild(int i);
    int parent(int i);

    int numOfElements();
    int find(string x);
    void resizeUp();
    void resizeDown();

    void print(int i);
    void printTo(ostream &out, int i);
    friend ostream& operator<<(ostream &out, Tree &t);

private:
    vector<NodeT> myTree;
    int size;
    int qty;
    void copy(Tree &other);
};

#endif // TREE_H
