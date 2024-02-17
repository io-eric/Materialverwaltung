/*
 Belegarbeit:
    > Name: Eric Wolf
    > Matr.-Nr.: 53453
    > Bibl.-Nr.: 85876
*/

#ifndef DATABASE_H
#define DATABASE_H

typedef struct Article
{
    char *uid; // Article number
    char *description;
    unsigned quantity;
} tArticle;

/* "free_tArticle" will free the memory allocated to tArticle */
void free_tArticle(tArticle *tArticle);

/* "partition" will partition a linked list starting from "pStart" and ending at "pEnd", 
and return a pointer to the partition point in the list.*/
tListNode *partition(tListNode *pStart, tListNode *pEnd);

/* "sort_alpha" will sort a linked list in alphabetical order starting from "pStart" and ending at "pEnd". */
void sort_alpha(tListNode *pStart, tListNode *pEnd);

/* "uid_exists" will check if a given "uid" exists in the linked list pointed to by "pList". */
bool uid_exists(tList *pList, const char *uid);

/* "get_tArticle" will search for a tArticle in the linked list pointed to by "pList"
 that has the specified "uid" and return a pointer to that tArticle. 
 If no such tArticle exists in the list, the function will return a null pointer. */
tArticle *get_tArticle(tList *pList, const char *uid);

/* "get_tArticle_index" will search for a tArticle in the linked list pointed to by "pList" 
that has the specified "uid" and return the index of that tArticle in the list. */
int get_tArticle_index(tList *pList,  const char *uid);

/* "read_database" will read data from a CSV file and populate a linked list pointed to by "pList" with tArticles. */
void read_database(tList *pList);

/* "write_database" will write data from a linked list pointed to by "pList" to a CSV file. 
The existing data in the database will be replaced with the data from the linked list.*/
void write_database(tList *pList);

/* "search_tList" will search the linked list pointed to by "pList" for tArticles whose 
titles or uids matches the "query". This function will modify the linked list. */
void search_tList(tList *pList, const char query[100]);

#endif