/*
 Belegarbeit:
    > Name: Eric Wolf
    > Matr.-Nr.: 53453
    > Bibl.-Nr.: 85876
*/

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct ListNode
{
   struct ListNode *pNext;     // Pointer to the next element
   struct ListNode *pPrevious; // Pointer to the previous element
   void *pData;                // Pointer to data
} tListNode;

typedef struct List // Holds head and tail pointer and is therefore list access.
{
   tListNode *pHead;
   tListNode *pTail;
   void (*freeData)(void *data); // Function to modular free data from nodes
} tList;

/* "safe_malloc_list" is responsible for allocating
   everything that has to do with the list and
   releasing the memory in the event of an error. */
void *safe_malloc_list(size_t tSize, tList *pList);

/* "safe_realloc_list" is responsible for reallocating
memory for the list. If the
reallocation is successful, it returns the new pointer
to the list, otherwise, it releases the memory. */
void *safe_realloc_list(void *ptr, size_t size, tList *pList);

/* "free_list" will free the memory allocated to the linked list pointed to by "pList", 
including all the tArticles in the list. After calling this function, 
the linked list should no longer be used. */
void free_list(tList *pList);

// "create_list" will create a new empty linked list and return a pointer to it.
tList *create_list(void (*freeData)(void*));

/* "create_node" will create a new node in the linked list pointed to by "pList",
 and initialize it with the data pointed to by "pData".
 The function will return a pointer to the new node. */
tListNode *create_node(tList *pList, void *pData);

/* "insert" will insert a new node containing the data pointed to by "pData" 
into the linked list pointed to by "pList" at the specified "index". */
void insert_index(tList *pList, int index, void *pData);

/* "insert_first" will insert a new node containing the data pointed to by 
"pData" at the beginning of the linked list pointed to by "pList". */
void insert_first(tList *pList, void *pData);

/* "insert_last" will insert a new node containing the data pointed to by 
"pData" at the end of the linked list pointed to by "pList". */
void insert_last(tList *pList, void *pData);

/* "remove_first" will remove the first node in the linked list pointed to by
 "pList" and free the memory allocated to that node. The data in the removed node 
 will be lost. If the list is empty, this function will have no effect. */
void remove_first(tList *pList);

/* "remove" will remove the node at the specified "index" from the linked list 
pointed to by "pList" and free the memory allocated to that node. */ 
void remove_index(tList *pList, int index);

/* "swap" will swap the positions of the nodes pointed to by "pNode1" and "pNode2" in a linked list. */
void swap(tListNode *pNode1, tListNode *pNode2);

/* "calculate_length" will calculate the number of nodes in the linked list pointed 
to by "pList" and return the result as an integer. */
int calculate_length(tList *pList);

#endif