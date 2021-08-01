#pragma once

#ifndef BBLINKEDLIST_H
#define BBLINKEDLIST_H


class BBLinkedList
{
public:
    BBLinkedList();

    int getCount();

    template<typename T>
    T* next() { return (T*)m_pNext; }

    void pushBack(BBLinkedList *pNode);
    void insertAfter(BBLinkedList *pNode);

    template<typename T>
    T* removeSelf()
    {
        T *ptr = (T*)m_pNext;
        m_pNext = nullptr;
        return ptr;
    }

    void remove(BBLinkedList *pNode);
    bool isEnd();

    void clear();

    BBLinkedList *m_pNext;
};

#endif // BBLINKEDLIST_H
