/*
 Belegarbeit:
    > Name: Eric Wolf
    > Matr.-Nr.: 53453
    > Bibl.-Nr.: 85876
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../utils/utils.h"
#include "../linkedlist/linkedlist.h"
#include "database.h"

#define CSV_FILE "database.csv"
#define CSV_DELIMITER ","

void free_tArticle(tArticle *tArticle){
    free(tArticle->description);
    free(tArticle->uid);
    free(tArticle);
}

tListNode *partition(tListNode *pStart, tListNode *pEnd)
{
    tListNode *pPivotPrev = pStart;
    tListNode *pCurrent = pStart;
    size_t size = strlen(((tArticle *)pEnd->pData)->description);
    char *pivot = (char *)malloc(size + 1); // +1 for the null terminator
    if (!pivot)
    {
        // Allocation failed, exit from the program
        fprintf(stderr, "Failed to allocate %ld bytes\n", size);
        exit(EXIT_FAILURE);
    }
    strcpy(pivot, ((tArticle *)pEnd->pData)->description);
    while (pStart != pEnd)
    {
        if (strcasecmp(pivot, ((tArticle *)pStart->pData)->description) >= 0)
        {
            pPivotPrev = pCurrent;
            swap(pCurrent, pStart);
            pCurrent = pCurrent->pNext;
        }
        pStart = pStart->pNext;
    }
    free(pivot);
    swap(pCurrent, pEnd);
    return pPivotPrev;
}

void sort_alpha(tListNode *pStart, tListNode *pEnd)
{
    if (pStart == pEnd)
        return;

    tListNode *pPrev = partition(pStart, pEnd);
    sort_alpha(pStart, pPrev);
    sort_alpha(pPrev->pNext, pEnd);
}

bool uid_exists(tList *pList, const char *uid)
{
    bool valid = true;
    tListNode *pNode = pList->pHead;
    while (pNode)
    {
        if (strcmp(((tArticle *)pNode->pData)->uid, uid) == 0)
        {
            valid = false;
            break;
        }
        pNode = pNode->pNext;
    }
    return valid;
}

tArticle *get_tArticle(tList *pList, const char *uid)
{
    tListNode *pNode = pList->pHead;
    tArticle *pArticle = NULL;
    while (pNode)
    {
        if (strcmp(((tArticle *)pNode->pData)->uid, uid) == 0)
        {
            pArticle = pNode->pData;
            break;
        }
        pNode = pNode->pNext;
    }
    return pArticle;
}

int get_tArticle_index(tList *pList, const char *uid)
{
    tListNode *pNode = pList->pHead;
    int index = 0;
    while (pNode)
    {
        if (strcmp(((tArticle *)pNode->pData)->uid, uid) == 0)
            break;
        index++;
        pNode = pNode->pNext;
    }
    return index;
}

void search_tList(tList *pList, const char query[100])
{
    tListNode *pNode = pList->pHead;
    int index = 0;
    while (pNode)
    {
        tListNode *pNext = pNode->pNext; // Save the next node
        tArticle *pArticle = pNode->pData;

        size_t searchStringSize = strlen(pArticle->description) + strlen(pArticle->uid) + 1; // Calculate the size of the new string
        char *searchString = malloc(searchStringSize);                                       // Allocate a new string
        strcpy(searchString, pArticle->description);                                         // Copy the first string into the new string
        strcat(searchString, pArticle->uid);                                                 // Concatenate the second string onto the new string

        if (!strstr(searchString, query))
        {
            remove_index(pList, index);
        }
        else
        {
            index++;
        }
        free(searchString); // Free the allocated memory when done
        pNode = pNext;
    }
}

void read_database(tList *pList)
{
    if (!pList)
        return;
    FILE *pFile = fopen(CSV_FILE, "r");
    if (!pFile)
    {
        fprintf(stderr, "Can't open file\n");
        return;
    }

    char buffer[128];
    int row = 0;
    int column = 0;

    while (fgets(buffer, 128, pFile))
    {
        column = 0;
        row++;

        // Splitting the data
        char *value = strtok(buffer, CSV_DELIMITER);

        // Allocate memory for "pArticle"
        tArticle *pArticle = safe_malloc_list(sizeof(tArticle), pList);

        // Assign data to pArticle
        while (value)
        {
            // Calculate the length of the value string
            size_t value_len = strlen(value);

            switch (column)
            {
            // Article number
            case 0:
            {
                // Allocate memory for the uid buffer
                pArticle->uid = safe_malloc_list(value_len + 1, pList); // +1 for the null terminator
                // Copy the value string to the uid buffer
                strcpy(pArticle->uid, value);
            }
            break;
            // Article description
            case 1:
            {
                // Allocate memory for the uid buffer
                pArticle->description = safe_malloc_list(value_len + 1, pList); // +1 for the null terminator
                // Copy the value string to the uid buffer
                strcpy(pArticle->description, value);
            }
            break;
            // Article quantity
            case 2:
            {
                pArticle->quantity = atoi(value);
            }
            break;
            }
            value = strtok(NULL, CSV_DELIMITER);
            column++;
        }
        insert_last(pList, pArticle);
    }
    fclose(pFile);
}

void write_database(tList *pList)
{
    if (!pList)
        return;
    FILE *pFile = fopen(CSV_FILE, "w");
    if (!pFile)
    {
        fprintf(stderr, "Can't open file\n");
        return;
    }
    tListNode *pNode = pList->pHead;
    while (pNode)
    {
        tArticle *pArticle = pNode->pData;
        // Replace the reserved delimiter with a space
        replace_substr(pArticle->description, CSV_DELIMITER, " ");
        // Writes data to the CSV file
        fprintf(pFile, "%s%s%s%s%u\n",
                pArticle->uid,
                CSV_DELIMITER,
                pArticle->description,
                CSV_DELIMITER,
                pArticle->quantity);
        pNode = pNode->pNext;
    }
    fclose(pFile);
}