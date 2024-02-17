/*
 Belegarbeit:
    > Name: Eric ....
    > Matr.-Nr.: .....
    > Bibl.-Nr.: .....
*/

#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

void *safe_malloc_list(size_t size, tList *pList)
{
    // Allocate memory
    void *new_ptr = malloc(size);
    if (!new_ptr)
    {
        // Allocation failed, exit from the program
        fprintf(stderr, "Failed to allocate %ld bytes\n", size);
        // Will only be called if we already have a list
        if (pList)
            free_list(pList);
        exit(EXIT_FAILURE);
    }
    return new_ptr;
}

void *safe_realloc_list(void *ptr, size_t size, tList *pList) {
    void *new_ptr = realloc(ptr, size);
    if (!new_ptr) {
        // Allocation failed, exit from the program
        fprintf(stderr, "Failed to reallocate %ld bytes\n", size);
        // Will only be called if we already have a list
        if (pList)
            free_list(pList);
        exit(EXIT_FAILURE);
    }
    return new_ptr;
}

void free_list(tList *pList)
{
    // Free all nodes and their pData
    while (pList->pHead)
    {
        remove_first(pList);
    }
    // Finally free the list
    free(pList);
}

tList *create_list(void (*freeData)(void*))
{
    // Allocate memory for pList
    tList *pList = safe_malloc_list(sizeof(tList), NULL);
    pList->pHead = NULL;
    pList->pTail = NULL;
    pList->freeData = freeData;
    return pList;
}

tListNode *create_node(tList *pList, void *pData)
{
    // Allocate memory for pNode
    tListNode *pNode = safe_malloc_list(sizeof(tListNode), pList);
    pNode->pData = pData;
    pNode->pNext = NULL;
    pNode->pPrevious = NULL;
    return pNode;
}

int calculate_length(tList *pList)
{
    tListNode *pNode = pList->pHead;
    int length = 0;
    while (pNode)
    {
        pNode = pNode->pNext;
        length++;
    }
    return length;
}

void insert_index(tList *pList, int index, void *pData)
{
    if (index == 0)
    {
        insert_first(pList, pData);
        return;
    }
    int length = calculate_length(pList);
    // If the index is greater than length it can cause a buffer overflow
    // If the index is below zero, no entry can be made at all
    if (index < 0 || length - 1 < index)
    {
        fprintf(stderr, "\nIndex out of range!\n");
        return;
    }

    // Create new pNode
    tListNode *pNode = create_node(pList, pData);
    tListNode *tmp = pList->pHead;

    // Traverse to index node
    while (--index)
    {
        tmp = tmp->pNext;
    }

    pNode->pNext = tmp->pNext;
    pNode->pPrevious = tmp;

    // If tmp is not the last node
    if (tmp->pNext)
    {
        // Assign tmp node's current following node pNewNode as pPrevious
        tmp->pNext->pPrevious = pNode;
    }
    else
    {
        // If tmp is the last node, pNewNode becomes the new last node
        pList->pTail = pNode;
    }

    // Assign tmp node's pNext to newNode
    tmp->pNext = pNode;
}

void insert_first(tList *pList, void *pData)
{
    // Create new pNode
    tListNode *pNode = create_node(pList, pData);

    // If tList is empty
    if (!pList->pHead)
    {
        // pNode becomes pHead and pTail
        pList->pHead = pNode;
        pList->pTail = pNode;
    }
    else
    {
        // pNode becomes the new pHead
        pNode->pNext = pList->pHead;
        pList->pHead->pPrevious = pNode;
        pList->pHead = pNode;
    }
}

void insert_last(tList *pList, void *pData)
{
    // Create new pNode
    tListNode *pNode = create_node(pList, pData);

    // If tList is empty
    if (!pList->pHead)
    {
        // pNode becomes pHead and pTail
        pList->pHead = pNode;
        pList->pTail = pNode;
    }
    else
    {
        // pNode becomes the new pTail
        pNode->pPrevious = pList->pTail;
        pList->pTail->pNext = pNode;
        pList->pTail = pNode;
    }
}

void remove_first(tList *pList)
{
    if (!pList->pHead)
        return;

    tListNode *pNode = pList->pHead->pNext;
    if (pNode)
        pNode->pPrevious = NULL;

    // Free the current pHead and its pData
    pList->freeData(pList->pHead->pData);
    free(pList->pHead);

    pList->pHead = pNode;
    if (!pNode)
        pList->pTail = NULL;
}

void remove_index(tList *pList, int index)
{
    if (index == 0)
    {
        remove_first(pList);
        return;
    }
    int length = calculate_length(pList);
    // If the index is greater than length it can cause a buffer overflow
    // If the index is below zero, no entry can be made at all
    if (index < 0 || length - 1 < index)
    {
        fprintf(stderr, "\nIndex out of range!\n");
        return;
    }

    tListNode *tmp = pList->pHead;

    // Traverse to index node
    while (--index)
    {
        tmp = tmp->pNext;
    }
    tListNode *next = tmp->pNext->pNext;
    tListNode *prev = tmp;

    pList->freeData(tmp->pNext->pData);
    free(tmp->pNext);

    // If "next" does not exist, "prev" becomes the new pTail
    if (!next)
    {
        prev->pNext = NULL;
        pList->pTail = prev;
        return;
    }

    // Unchain tmp from the tList
    prev->pNext = next;
    next->pPrevious = prev;
}

void swap(tListNode *pNode1, tListNode *pNode2){
    void *pTmpData = pNode1->pData;
    pNode1->pData = pNode2->pData;
    pNode2->pData = pTmpData;
}
