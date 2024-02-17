/*
 Belegarbeit:
    > Name: Eric Wolf
    > Matr.-Nr.: 53453
    > Bibl.-Nr.: 85876
*/

#ifndef UTILS_H
#define UTILS_H

typedef enum
{
    false,
    true
} bool;

/* "digits_only" checks if a string contains only numeric characters and returns true if it does, and false otherwise. */
bool digits_only(const char *s);

/* "replace_substr" will replace all occurrences of "substr" in the "str" character array with "replacement". */
void replace_substr(char *str, const char *substr, const char *replacement);

/* "url_decode" will decode the URL-encoded string pointed to by "str" and return a pointer to a character array containing the decoded string. */
char *url_decode(const char *str);

#endif