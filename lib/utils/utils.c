/*
 Belegarbeit:
    > Name: Eric Wolf
    > Matr.-Nr.: 53453
    > Bibl.-Nr.: 85876
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "utils.h"

bool digits_only(const char *s)
{
    while (*s) {
        if (isdigit(*s++) == 0) return false;
    }

    return true;
}

void replace_substr(char *str, const char *substr, const char *replacement)
{
    char *pSubstr = str;

    while ((pSubstr = strstr(pSubstr, substr)) != NULL)
    {
        // Create a new string with enough space to hold the modified string
        char modifiedStr[128];
        strncpy(modifiedStr, str, pSubstr - str); // Copy the part of the string before the substring
        modifiedStr[pSubstr - str] = '\0';        // Add a null terminator to the end of the modified string

        // Concatenate the "modifiedStr" and the "modifiedStr"
        strcat(modifiedStr, replacement);
        strcat(modifiedStr, pSubstr + strlen(substr));

        // Copy the modified string back into the original string
        strcpy(str, modifiedStr);

        // Move the pointer to the end of the "replacement" string
        pSubstr += strlen(replacement);
    }
}

char *url_decode(const char *str)
{
    // Check for NULL input
    if (!str)
        return NULL;

    // Allocate a buffer for the decoded string
    size_t len = strlen(str);
    char *decoded = malloc(len + 1);
    if (!decoded)
        return NULL;

    // Decode the string
    char *p = decoded;
    while (*str)
    {
        // Check if "str" has the right format
        // isxdigit is used to check if the character is a valid hexadecimal digit
        if (*str == '%' && isxdigit(str[1]) && isxdigit(str[2]))
        {
            // Decode %XX sequence
            char hex[3] = {str[1], str[2], '\0'};
            *p++ = (char)strtol(hex, NULL, 16);
            str += 3;
        }
        else
        {
            // Copy regular character
            *p++ = *str++;
        }
    }

    // Terminate the decoded string
    *p = '\0';

    // Return the decoded string
    return decoded;
}