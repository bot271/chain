//
//  chain.h
//  chain
//
//  Created by Mingze Lee on 2024/10/9.
//

#ifndef __CHAIN_H__
#define __CHAIN_H__
#include <iostream>

template <typename T>
struct chainNode
{
    T element;
    chainNode<T> * next;
    
    chainNode() { };
    chainNode(const T & element) : element(element) { };
    chainNode(const T & element, chainNode<T> * next) : element(element), next(next) { };
};

template <typename T>
class chain
{
protected:
    void checkIndex(int) const;
    
    chainNode<T> * firstNode;
    int listSize;
public:
    chain(int initialCapacity = 10);
    chain(const chain<T> &);
    ~chain();
    
    bool empty() const { return listSize == 0; };
    int size() const { return listSize; };
    T & get(int index) const;
    int indexOf(const T & theElement) const;
    void erase(int index);
    void insert(int index, const T & theElement);
    void output(std::ostream & os) const;
    friend std::ostream & operator<<(std::ostream &, const chain<T> &);
    void clear();
    void setSize(int);
    void binSort(int);
    
    class iterator
    {
    protected:
        chainNode<T> * node;
    private:
        iterator(chainNode<T> * theNode = nullptr) : node(theNode) { };
        T & operator*() { return node -> element; };
        T * operator->() { return &(node -> element); };
        iterator& operator++() { node = node -> next; return *this; };
        iterator& operator++(int)
        {
            iterator olditer = *this;
            node = node -> next;
            return olditer;
        };
        bool operator==(const iterator & iter) { return node == iter.node; };
        bool operator!=(const iterator & iter) { return node != iter.node; };
    };
};

template <typename T>
void chain<T>::checkIndex(int index) const
{
    if (index >= listSize || index < 0)
        throw "bad index";
}

template <typename T>
chain<T>::chain(int initialCapacity)
{
    if (initialCapacity < 1)
    {
        std::cout << "Initial capacity = " << initialCapacity << " must be positive.";
        throw "Bad input";
    }
    firstNode = nullptr;
    listSize = 0;
}

template <typename T>
chain<T>::chain(const chain & prevList)
{
    listSize = prevList.listSize;
    if (listSize == 0)
        firstNode = nullptr;
    chainNode<T> * sourceNode = prevList.firstNode;
    firstNode = new chainNode<T>(sourceNode -> element);
    chainNode<T> * targetNode = firstNode;
    sourceNode = sourceNode -> next;
    while (sourceNode != nullptr)
    {
        targetNode -> next = new chainNode(sourceNode -> element);
        targetNode = targetNode -> next;
        sourceNode = sourceNode->next;
    }
    targetNode->next = nullptr;
}

template <typename T>
chain<T>::~chain()
{
    while (firstNode != nullptr)
    {
        chainNode<T> * nextNode = firstNode -> next;
        delete firstNode;
        firstNode = nextNode;
    }
}
    
template <typename T>
T & chain<T>::get(int index) const
{
    checkIndex(index);
    chainNode<T> * currentNode = firstNode;
    for (int i = 0; i < index; ++i)
        currentNode = currentNode -> next;
    return currentNode -> element;
}

template <typename T>
int chain<T>::indexOf(const T & theElement) const
{
    chainNode<T> * currentNode = firstNode;
    int index = 0;
    while (currentNode -> element != theElement)
    {
        if (currentNode == nullptr)
            return -1;
        currentNode = currentNode -> next;
        ++index;
    }
    return index;
}

template <typename T>
void chain<T>::erase(int index)
{
    checkIndex(index);
    chainNode<T> * deleteNode;
    if (index == 0)
    {
        deleteNode = firstNode;
        firstNode = firstNode -> next;
    }
    else
    {
        chainNode<T> * pcn = firstNode;
        for (int i = 0; i < index - 1; ++i)
            pcn = pcn -> next;
        deleteNode = pcn -> next;
        pcn ->next = deleteNode -> next;
    }
    delete deleteNode;
    --listSize;
}

template <typename T>
void chain<T>::insert(int index, const T & theElement)
{
    if (index < 0 || index > listSize)
        throw "bad index";
    if (index == 0)
        firstNode = new chainNode<T> (theElement, firstNode);
    else
    {
        chainNode<T> * pcn = firstNode;
        for (int i = 0; i < index - 1; ++i)
            pcn = pcn -> next;
        pcn -> next = new chainNode<T>(theElement, pcn -> next);
    }
    ++listSize;
}

template <typename T>
void chain<T>::output(std::ostream & os) const
{
    auto currentNode = firstNode;
    while (currentNode != nullptr)
    {
        os << currentNode -> element << ' ';
        currentNode = currentNode -> next;
    }
}

template <typename T>
void chain<T>::clear()
{
    while (firstNode != nullptr)
    {
        chainNode<T> * nextNode = firstNode -> next;
        delete firstNode;
        firstNode = nextNode;
    }
    listSize = 0;
}

template <typename T>
void chain<T>::setSize(int newSize)
{
    if (newSize < 0)
        throw "bad size input";
    if (newSize >= listSize)
        return;
    chainNode<T> * pcn = firstNode;
    if (newSize == 0)
        clear();
    for (int i = 0; i < newSize - 1; ++i)
        pcn = pcn -> next;
    chainNode<T> * nextNode = pcn -> next;
    chainNode<T> * tempNode = nextNode;
    while (nextNode != nullptr)
    {
        nextNode = nextNode -> next;
        delete tempNode;
        tempNode = nextNode;
    }
    pcn -> next = nullptr;
    listSize = newSize;
}

template <typename T>
void chain<T>::binSort(int range)
{
    chainNode<T> ** bottom, ** top;
    bottom = new chainNode<T> * [range + 1];
    top = new chainNode<T> * [range + 1];
    for (int b = 0; b <= range; ++b)
        bottom[b] = nullptr;
    for (;firstNode != nullptr; firstNode = firstNode -> next)
    {
        int binNum = firstNode -> element;
        if (bottom[binNum] == nullptr)
            bottom[binNum] = top[binNum] = firstNode;
        else
        {
            top[binNum] -> next = firstNode;
            top[binNum] = firstNode;
        }
    }
    chainNode<T> * py = nullptr;
    for (int binNum = 0; binNum <= range; ++binNum)
        if (bottom[binNum] != nullptr)
        {
            if (py == nullptr)
                firstNode = bottom[binNum];
            else
                py -> next = bottom[binNum];
            py = top[binNum];
        }
    if (py != nullptr)
        py -> next = nullptr;
    delete [] bottom;
    delete [] top;
}




#endif
