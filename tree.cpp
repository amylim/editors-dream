#include "tree.h"

NodeT::NodeT()
{
    height = 0;
    heightDiff = 0;
    count = 0;
    data = "\0";
    location.resize(1);
}

NodeT::NodeT(const NodeT &other)
{
    copy(other);
}

NodeT& NodeT::operator=(const NodeT &other)
{
    if(this != &other)
        copy(other);
    return *this;
}

void NodeT::copy(const NodeT &other)
{
    height = other.height;
    heightDiff = other.heightDiff;
    count = other.count;
    data = other.data;
    location = other.location;
}

void NodeT::addLocation(int p, int l)
{
    location[count].par = p;
    location[count].line = l;
    sortLocationAscending();
    count++;
    location.resize(count+5);
}

void NodeT::clear()
{
    heightDiff = 0;
    count = 0;
    data = "\0";
    location.resize(1);
    location[0].par = 1;
    location[0].line = 1;
}

void NodeT::sortLocationAscending()
{
    int i = count;
    while(location[i] < location[i-1] && i > 0)
    {
        WordCoord temp = location[i-1];
        location[i-1] = location[i];
        location[i] = temp;
        i--;

    }
}

ostream& operator<<(ostream &out, NodeT &t)
{
    out << setw(5) << t.count << " " << t.data << endl;
    for(int i = 0; i < t.count; i++)
        out << t.location[i] << endl;
    return out;
}

bool operator<(const NodeT &x, const NodeT &y)
{
    return x.data < y.data;
}

bool operator>(const NodeT &x, const NodeT &y)
{
    return x.data > y.data;
}

//----------word coordinates------------

WordCoord::WordCoord()
{
    par = 0;
    line = 0;
}

WordCoord::WordCoord(const WordCoord &other)
{
    copy(other);
}

WordCoord& WordCoord::operator=(const WordCoord &other)
{
    if(this != &other)
        copy(other);
    return *this;
}

void WordCoord::copy(const WordCoord &other)
{
    par = other.par;
    line = other.line;
}

ostream& operator<<(ostream &out, WordCoord &t)
{
    out << t.par << ", " << t.line;
    return out;
}

bool operator<(const WordCoord &x, const WordCoord &y)
{
    if(x.par == y.par)
        return x.line < y.line;
    else
        return x.par < y.par;
}

bool operator>(const WordCoord &x, const WordCoord &y)
{
    if(x.par == y.par)
        return x.line > y.line;
    else
        return x.par > y.par;
}

Tree::Tree()
{
    size = 2;
    myTree.resize(size);
    myTree[0].height = 1;
    qty = 0;
}

Tree::~Tree()
{

}

Tree::Tree(Tree &other)
{
    copy(other);
}

Tree& Tree::operator=(Tree &other)
{
    if(this != &other)
        copy(other);
    return *this;
}

void Tree::copy(Tree &other)
{
    for(int i = 0; i < other.size; i++)
        myTree[i] = other.myTree[i];
    size = other.size;
    qty = other.qty;
}

void Tree::insert(string x, int p, int l)
{
    int i = find(x);
    while(i >= size/2-1)
    {
        resizeUp();
    }
    if(x != myTree[i].data)
    {
        myTree[i].data = x;
        qty++;
        myTree[i].addLocation(p, l);
        updateHeightInfoInsert(i);
    }
    else
        myTree[i].addLocation(p, l);
}

void Tree::print(int i)
{
    if(myTree[leftChild(i)].data != "\0")
        print(leftChild(i));
    cout << myTree[i] << endl;
    if(myTree[rightChild(i)].data != "\0")
        print(rightChild(i));
}

void Tree::updateHeightInfoInsert(int newItemIndex)
{
    int i = newItemIndex;
    while(i != 0)
    {
        if(isLeftChild(i))
        {
            int start = farRight(parent(i), newItemIndex);
            int diff = pow(2, myTree[newItemIndex].height - myTree[parent(i)].height - 1);
            int check = 0;
            bool empty = true;
            while(empty && check < diff)
            {
                if(myTree[start].data != "\0")
                    empty = false;
                check++;
                start--;
            }
            if(--myTree[parent(i)].heightDiff == -2)
            {
                balance(parent(i), i);
                break;
            }
            if(!empty)
                break;
        }
        else
        {

            int start = farLeft(parent(i), newItemIndex);
            int diff = pow(2, myTree[newItemIndex].height - myTree[parent(i)].height - 1);
            int check = 0;
            bool empty = true;
            while(empty && check < diff)
            {
                if(myTree[start].data != "\0")
                    empty = false;
                check++;
                start++;
            }
            if(++myTree[parent(i)].heightDiff == 2)
            {
                balance(parent(i), i);
                break;
            }
            if(!empty)
                break;
        }
        i = parent(i);
    }
}

void Tree::balance(int root, int problemChild)
{
    if(myTree[root].heightDiff == -2)
    {
        //right case
        if(myTree[problemChild].heightDiff == -1)
            rightRotation(problemChild);
        //left-right case
        else
        {
            leftRotation(rightChild(problemChild));
            rightRotation(problemChild);
        }
    }
    else
    {
        //left case
        if(myTree[problemChild].heightDiff == 1)
            leftRotation(problemChild);
        //right-left case
        else
        {
            rightRotation(leftChild(problemChild));
            leftRotation(problemChild);
        }
    }
    myTree[root].heightDiff = 0;
    myTree[leftChild(root)].heightDiff = getHeightDiff(leftChild(root));
    myTree[rightChild(root)].heightDiff = getHeightDiff(rightChild(root));
}

int Tree::getHeightDiff(int root)
{
    int left, right;
    if(myTree[leftChild(root)].data != "\0")
        left = myTree[getLastChild(leftChild(root))].height;
    else
        left = myTree[root].height;
    if(myTree[rightChild(root)].data != "\0")
        right = myTree[getLastChild(rightChild(root))].height;
    else
        right = myTree[root].height;
    return right - left;
}

int Tree::getLastChild(int start)
{
    int *storage = new int[qty];
    int count = 0;
    obtainElementsPOST(storage, count, start);
    sortDescending(storage, count);
    return storage[0];
}

void Tree::rightRotation(int pivot)
{
    int *storage = new int[qty];
    int count = 0;
    if(myTree[sibling(pivot)].data != "\0")
    {
        obtainElementsPOST(storage, count, sibling(pivot));
        sortDescending(storage, count);
        moveRightDown(storage, count, sibling(pivot));
    }
    //move parent down
    myTree[sibling(pivot)].heightDiff = myTree[parent(pivot)].heightDiff;
    myTree[sibling(pivot)].count = myTree[parent(pivot)].count;
    myTree[sibling(pivot)].data = myTree[parent(pivot)].data;
    myTree[sibling(pivot)].location = myTree[parent(pivot)].location;

    if(myTree[rightChild(pivot)].data != "\0")
        moveRightNext(rightChild(pivot), myTree[pivot].height+1);

    count = 0;
    obtainElementsPRE(storage, count, pivot);
    sortAscending(storage, count);
    moveRightUp(storage, count, pivot);
}

void Tree::leftRotation(int pivot)
{
    int *storage = new int[qty];
    int count = 0;
    if(myTree[sibling(pivot)].data != "\0")
    {
        obtainElementsPOST(storage, count, sibling(pivot));
        sortDescending(storage, count);
        moveLeftDown(storage, count, sibling(pivot));
    }
    //move parent down
    myTree[sibling(pivot)].heightDiff = myTree[parent(pivot)].heightDiff;
    myTree[sibling(pivot)].count = myTree[parent(pivot)].count;
    myTree[sibling(pivot)].data = myTree[parent(pivot)].data;
    myTree[sibling(pivot)].location = myTree[parent(pivot)].location;

    if(myTree[leftChild(pivot)].data != "\0")
        moveLeftNext(leftChild(pivot), myTree[pivot].height+1);

    count = 0;
    obtainElementsPRE(storage, count, pivot);
    sortAscending(storage, count);
    moveLeftUp(storage, count, pivot);
}

void Tree::moveRightDown(int *storage, int count, int startRoot)
{
    for(int j = 0; j < count; j++)
    {
        int move = rightChild(storage[j]) + farRight(startRoot, storage[j]) - storage[j];
        myTree[move].heightDiff = myTree[storage[j]].heightDiff;
        myTree[move].count = myTree[storage[j]].count;
        myTree[move].data = myTree[storage[j]].data;
        myTree[move].location = myTree[storage[j]].location;
        myTree[storage[j]].clear();
    }
}

void Tree::moveRightNext(int i, int startHeight)
{
    myTree[i + pow(2, myTree[i].height - startHeight)] = myTree[i];
    myTree[i].clear();
    if(myTree[leftChild(i)].data != "\0")
        moveRightNext(leftChild(i), startHeight);
    if(myTree[rightChild(i)].data != "\0")
        moveRightNext(rightChild(i), startHeight);
}

void Tree::moveRightUp(int *storage, int count, int startRoot)
{
    for(int j = 0; j < count; j++)
    {
        int move = parent(2*storage[j] - farLeft(startRoot, storage[j]));
        myTree[move].heightDiff = myTree[storage[j]].heightDiff;
        myTree[move].count = myTree[storage[j]].count;
        myTree[move].data = myTree[storage[j]].data;
        myTree[move].location = myTree[storage[j]].location;
        myTree[storage[j]].clear();
    }
}

int Tree::farRight(int root, int i)
{
    int diff = myTree[i].height - myTree[root].height;
    int mostRight = root;
    int j = 0;
    while(j < diff)
    {
        mostRight = rightChild(mostRight);
        j++;
    }
    return mostRight;
}

void Tree::moveLeftDown(int *storage, int count, int startRoot)
{
    for(int j = 0; j < count; j++)
    {
        int move = leftChild(storage[j]) + farLeft(startRoot, storage[j]) - storage[j];
        myTree[move].heightDiff = myTree[storage[j]].heightDiff;
        myTree[move].count = myTree[storage[j]].count;
        myTree[move].data = myTree[storage[j]].data;
        myTree[move].location = myTree[storage[j]].location;
        myTree[storage[j]].clear();
    }
}

void Tree::moveLeftNext(int i, int startHeight)
{
    myTree[i - pow(2, myTree[i].height - startHeight)] = myTree[i];
    myTree[i].clear();
    if(myTree[leftChild(i)].data != "\0")
        moveLeftNext(leftChild(i), startHeight);
    if(myTree[rightChild(i)].data != "\0")
        moveLeftNext(rightChild(i), startHeight);
}

void Tree::moveLeftUp(int *storage, int count, int startRoot)
{
    for(int j = 0; j < count; j++)
    {
        int move = parent(2*storage[j] - farRight(startRoot, storage[j]));
        myTree[move].heightDiff = myTree[storage[j]].heightDiff;
        myTree[move].count = myTree[storage[j]].count;
        myTree[move].data = myTree[storage[j]].data;
        myTree[move].location = myTree[storage[j]].location;
        myTree[storage[j]].clear();
    }
}

int Tree::farLeft(int root, int i)
{
    int diff = myTree[i].height - myTree[root].height;
    int mostLeft = root;
    int j = 0;
    while(j < diff)
    {
        mostLeft = leftChild(mostLeft);
        j++;
    }
    return mostLeft;
}

void Tree::obtainElementsPOST(int *storage, int &count, int i)
{
    //for post order
    if(myTree[leftChild(i)].data != "\0")
        obtainElementsPOST(storage, count, leftChild(i));
    if(myTree[rightChild(i)].data != "\0")
        obtainElementsPOST(storage, count, rightChild(i));
    storage[count++] = i;
}

void Tree::obtainElementsPRE(int *storage, int &count, int i)
{
    //for pre order
    storage[count++] = i;
    if(myTree[leftChild(i)].data != "\0")
        obtainElementsPRE(storage, count, leftChild(i));
    if(myTree[rightChild(i)].data != "\0")
        obtainElementsPRE(storage, count, rightChild(i));
}

void Tree::obtainElementsIN(NodeT *storage, int &count, int i)
{
    //for in order
    if(myTree[leftChild(i)].data != "\0")
        obtainElementsIN(storage, count, leftChild(i));
    storage[count++] = myTree[i];
    if(myTree[rightChild(i)].data != "\0")
        obtainElementsIN(storage, count, rightChild(i));
}

void Tree::sortDescending(int *array, int count)
{
    for(int i = 0; i < count; i++)
    {
        for(int j = i + 1; j < count; j++)
        {
            if(array[i] < array[j])
            {
                int temp = array[i];
                array[i] = array[j];
                array[j] = temp;
            }
        }
    }
}

void Tree::sortAscending(int *array, int count)
{
    for(int i = 0; i < count; i++)
    {
        for(int j = i + 1; j < count; j++)
        {
            if(array[i] > array[j])
            {
                int temp = array[i];
                array[i] = array[j];
                array[j] = temp;
            }
        }
    }
}

bool Tree::isSameHeight(int x, int y)
{
    return (myTree[x].height == myTree[y].height);
}

bool Tree::hasSibling(int i)
{
    if(i%2) //if left child
    {
        if(myTree[i+1].data != "\0")
            return true;
    }
    else if(i > 0) //if not root
    {
        if(myTree[i-1].data != "\0")
            return true;
    }
    return false;
}

bool Tree::isRightChild(int i)
{
    if(i != 0 && !(i%2))
        return true;
    else
        return false;
}

bool Tree::isLeftChild(int i)
{
    if(i%2)
        return true;
    else
        return false;
}

int Tree::height(int i)
{
    int h = 0;
    i++;
    while(i > 1)
    {
        i = i/2;
        h++;
    }
    return h + 1;
}

void Tree::remove(int index)
{
    int i = index;
    if(myTree[i].data != "\0")
    {
        //no children
        if(myTree[rightChild(i)].data == "\0" && myTree[leftChild(i)].data == "\0")
        {
            myTree[i].clear();
            updateHeightInfoRemove(i);
        }
        //two children
        else if(myTree[rightChild(i)].data != "\0" && myTree[leftChild(i)].data != "\0")
        {
            int replacement = rightChild(i);
            while(myTree[leftChild(replacement)].data != "\0")
                replacement = leftChild(replacement);
            myTree[i].count = myTree[replacement].count;
            myTree[i].data = myTree[replacement].data;
            myTree[i].location = myTree[replacement].location;
            myTree[replacement].clear();
            updateHeightInfoRemove(replacement);
        }
        //one child
        else
        {
            int *storage = new int[qty];
            int count = 0;
            if(myTree[leftChild(i)].data != "\0")
            {
                obtainElementsPRE(storage, count, leftChild(i));
                moveLeftUp(storage, count, leftChild(i));
            }
            else
            {
                obtainElementsPRE(storage, count, rightChild(i));
                moveRightUp(storage, count, rightChild(i));
            }
            updateHeightInfoRemove(i);
        }
        qty--;
    }
    else
    {

    }
}

void Tree::remove(string x)
{
    int i = find(x);
    if(myTree[i].data != "\0")
    {
        //no children
        if(myTree[rightChild(i)].data == "\0" && myTree[leftChild(i)].data == "\0")
        {
            myTree[i].clear();
            updateHeightInfoRemove(i);
        }
        //two children
        else if(myTree[rightChild(i)].data != "\0" && myTree[leftChild(i)].data != "\0")
        {
            int replacement = rightChild(i);
            while(myTree[leftChild(replacement)].data != "\0")
                replacement = leftChild(replacement);
            myTree[i].count = myTree[replacement].count;
            myTree[i].data = myTree[replacement].data;
            myTree[i].location = myTree[replacement].location;
            myTree[replacement].clear();
            updateHeightInfoRemove(replacement);
        }
        //one child
        else
        {
            int *storage = new int[qty];
            int count = 0;
            if(myTree[leftChild(i)].data != "\0")
            {
                obtainElementsPRE(storage, count, leftChild(i));
                moveLeftUp(storage, count, leftChild(i));
            }
            else
            {
                obtainElementsPRE(storage, count, rightChild(i));
                moveRightUp(storage, count, rightChild(i));
            }
            updateHeightInfoRemove(i);
        }
        qty--;
    }
    else
    {

    }
}

void Tree::updateHeightInfoRemove(int oldItemIndex)
{
    int i = oldItemIndex;
    while(i != 0)
    {
        int newHeightDiff = getHeightDiff(parent(i));
        if(myTree[parent(i)].heightDiff == newHeightDiff)
            break;
        else
            myTree[parent(i)].heightDiff = newHeightDiff;
        if(myTree[parent(i)].heightDiff == 2 || myTree[parent(i)].heightDiff == -2)
            balance(parent(i), sibling(i));
        i = parent(i);
    }
}

int Tree::find(string x)
{
    int i = 0;
    string temp = myTree[0].data;
    while(temp != "\0" && i < size)
    {
        if(x < temp)
            temp = myTree[i = 2*i+1].data;
        else if(x > temp)
            temp = myTree[i = 2*i+2].data;
        else
            return i;
    }
    return i;
}

int Tree::sibling(int i)
{
    if(i%2)
        return i+1;
    else if(i > 0)
        return i-1;
    else
        return i;
}

int Tree::leftChild(int i)
{
    return 2*i+1;
}

int Tree::rightChild(int i)
{
    return 2*i+2;
}

int Tree::parent(int i)
{
    return (i-1)/2;
}

int Tree::numOfElements()
{
    return qty;
}

void Tree::resizeUp()
{
    size = size*2;
    myTree.resize(size);
    int newHeight = height(size - 2);
    for(int i = size/2 - 1; i < size - 1; i++)
        myTree[i].height = newHeight;
}

void Tree::resizeDown()
{
    size = size/2;
    myTree.resize(size);
}

/*ostream& operator<<(ostream &out, Tree &t)
{
    for(int i = 0; i < t.myTree.size(); i++)
        if(t.myTree[i].data != "\0")
            out << "(" << t.myTree[i].height << ")" << setw(3) << i << setw(4) << t.myTree[i] << "            (" << t.myTree[i].heightDiff << ")" << endl;

    return out;
}*/

void Tree::printTo(ostream &out, int i)
{
    if(myTree[leftChild(i)].data != "\0")
        printTo(out, leftChild(i));
    out << myTree[i] << endl;
    if(myTree[rightChild(i)].data != "\0")
        printTo(out, rightChild(i));
}
