#pragma once

#ifndef BBLINKEDLIST_H
#define BBLINKEDLIST_H


class BBLinkedList
{
public:
    BBLinkedList();

    template<typename T>
    T* next() { return (T*)m_pNext; }

    void pushBack(BBLinkedList *pNode);
    void insertAfter(BBLinkedList *pNode);
    void remove(BBLinkedList *pNode);
    bool isEnd();

    BBLinkedList *m_pNext;
};

#endif // BBLINKEDLIST_H
