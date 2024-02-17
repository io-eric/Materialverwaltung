/*
 Belegarbeit:
    > Name: Eric Wolf
    > Matr.-Nr.: 53453
    > Bibl.-Nr.: 85876
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>

#include "lib/utils/utils.h"
#include "lib/linkedlist/linkedlist.h"
#include "lib/database/database.h"
#include "main.h"

#define INDEX_HTML "public/index.html"

void print_table_search(char query[100])
{
    printf("<input type=\"text\" id=\"table-search\" name=\"search_query\" value=\"%s\" placeholder=\"Bezeichnung,  Artikelnummer, ...\" />", query);
}

void print_table_body(tList *pList, char query[100])
{
    if (pList->pHead)
    {
        tListNode *pNode = pList->pHead;
        while (pNode)
        {
            tArticle *pArticle = pNode->pData;
            printf("<tr>"
                   "<td>%s</td>"
                   "<td>%s</td>"
                   "<td>%u</td>"
                   "<td>"
                   "<form style=\"display: inline-flex;\"action=\"beleg.cgi\" method=\"post\">"
                   "<input type=\"hidden\" name=\"type\" value=\"table_action\"/>"
                   "<input type=\"hidden\" name=\"uid\"  value=\"%s\"/>"
                   "<button type=\"submit\" class=\"edit-button\" name=\"action\" value=\"edit_dialog\">Bearbeiten</button>"
                   "&nbsp;&nbsp;"
                   "<button type=\"submit\" class=\"delete-button\" name=\"action\" value=\"delete\">Löschen</button>"
                   "</form>"
                   "</td>"
                   "</tr>",
                   pArticle->description,
                   pArticle->uid,
                   pArticle->quantity,
                   pArticle->uid);

            pNode = pNode->pNext;
        }
    }
    else
    {
        // "pList" is empty
        if (query)
        {
            printf(
                "<tr>"
                "<td colspan=\"4\" class=\"empty-table-td\">Für die durchgeführte Suche konnten keine Artikel gefunden werden."
                "<form action=\"beleg.cgi\" method=\"post\">"
                "<input type=\"hidden\" name=\"type\" value=\"article_search\" />"
                "<br/>"
                "<button class=\"cancel-button\" style=\"margin: 0; height: 40px\">Alle Artikel anzeigen</button>"
                "</form>"
                "</td>"
                "</tr>");
        }
        else
        {

            printf(
                "<tr>"
                "<td colspan=\"4\" class=\"empty-table-td\">Die Tabelle enthält derzeit keine Artikel. Bitte füge Artikel hinzu, um sie anzuzeigen.</td>"
                "</tr>");
        }
    }
}

void print_edit_dialog(tArticle *pArticle)
{
    printf("<dialog id=\"article-edit-dialog\" class=\"dialog\" open>"
           "<div>"
           "<h3 class=\"form-header\">Artikel bearbeiten</h3>"
           "<form method=\"post\" class=\"article-edit-form\">"
           "<input type=\"hidden\" name=\"type\" value=\"article_edit\"/>"
           "<input type=\"hidden\" name=\"origin\" value=\"%s\"/>"
           "<label>Artikelbezeichnung:</label>"
           "<input type=\"text\" name=\"description\" maxlength=\"75\" value=\"%s\" required/>"
           "<br/>"
           "<br/>"
           "<label>Artikelnummer:</label>"
           "<input type=\"number\" name=\"uid\" max=\"999999999999\" value=\"%s\" required/>"
           "<br/>"
           "<br/>"
           "<label>Lagerbestand:</label>"
           "<input type=\"text\" name=\"quantity\" value=\"%u\" required/>"
           "<br/>"
           "<br/>"
           "<button class=\"submit-button\" type=\"submit\">Speichern</button>"
           "</form>",
           pArticle->uid,
           pArticle->description,
           pArticle->uid,
           pArticle->quantity);
}

void print_error_dialog(tErrorDialog *pErrorDialog)
{
    printf("<dialog id=\"error-dialog\" class=\"dialog\" open>"
           "<div>"
           "<h3 class=\"form-header\">%s</h3>"
           "<p>%s</p>",
           pErrorDialog->title, pErrorDialog->body);
}

void print_create_form(tForm *form)
{
    printf(
        "<form action=\"beleg.cgi\" method=\"post\" id=\"article-create-form\">"
        "<input type=\"hidden\" name=\"type\" value=\"article_add\" />"
        "<h3 class=\"form-header\">Artikel hinzufügen</h3>"
        "<label>Artikelbezeichnung:</label>"
        "<input type=\"text\" name=\"description\" id=\"article-create-form-description\" maxlength=\"75\" value=\"%s\" required />"
        "<br /> <br />"
        "<label>Artikelnummer:</label>"
        "<input type=\"number\" name=\"uid\" id=\"article-create-form-uid\" max=\"999999999999\" value=\"%s\" required/>" // accepts up to 12 digits
        "<br /> <br />"
        "<label>Lagerbestand:</label>"
        "<input type=\"number\" name=\"quantity\" id=\"article-create-form-quantity\"  value=\"%d\" required />"
        "<button type=\"submit\" class=\"submit-button\">Hinzufügen</button>",
        form->description,
        form->uid,
        form->quantity);
}

void print_html(tList *pList, tForm *form, tEditDialog *pEditDialog, tErrorDialog *pErrorDialog, char lastSearchQuery[100])
{
    FILE *pFile = fopen(INDEX_HTML, "r");
    if (pFile)
    {
        char buffer[256];
        while (fgets(buffer, 256, pFile))
        {
            if (strstr(buffer, "id=\"article-tbody\""))
            {
                print_table_body(pList, lastSearchQuery);
                continue;
            }
            else if (strstr(buffer, "id=\"table-search\"") && lastSearchQuery)
            {
                print_table_search(lastSearchQuery);
                continue;
            }
            else if (strstr(buffer, "id=\"article-create-form\""))
            {
                print_create_form(form);
                continue;
            }
            else if (pEditDialog->open && strstr(buffer, "id=\"article-edit-dialog\""))
            {
                tArticle *pArticle = get_tArticle(pList, pEditDialog->uid);
                print_edit_dialog(pArticle);
                continue;
            }
            else if (pErrorDialog->open && strstr(buffer, "id=\"error-dialog\""))
            {
                print_error_dialog(pErrorDialog);
                continue;
            }

            puts(buffer);
        }
        fclose(pFile);
    }
    else
    {
        fprintf(stderr, "File error: %s\n", INDEX_HTML);
    }
}

bool validate_form(tList *pList, tForm *form, tErrorDialog *pErrorDialog)
{
    bool isValid = true;
    if (pList && !uid_exists(pList, form->uid)) // If pList is not given, the uid_exists step is skipped
    {
        isValid = false;
        // Show a error dialog
        pErrorDialog->open = true;
        strcpy(pErrorDialog->title, "Artikelnummer bereits vergeben");
        sprintf(pErrorDialog->body, "Es exestiert bereits ein Artikel mit der Artikelnummer <b>%s</b>.", form->uid);
    }
    else if (!digits_only(form->uid)) // UID only accepts numeric characters
    {
        isValid = false;
        // Show a error dialog
        pErrorDialog->open = true;
        strcpy(pErrorDialog->title, "Ungültige Artikelnummer");
        strcpy(pErrorDialog->body, "Bitte geben Sie eine gültige Artikelnummer an.");
    }
    else if (form->quantity < 0)
    {
        isValid = false;
        // Show a error dialog
        pErrorDialog->open = true;
        strcpy(pErrorDialog->title, "Lagerbestand negativ");
        strcpy(pErrorDialog->body, "Bitte überprüfen Sie Ihre Bestandsangaben.");
    }
    return isValid;
}

void add_article(tList *pList, tForm *form, tErrorDialog *pErrorDialog)
{
    // Checks if "form" is valid
    if (validate_form(pList, form, pErrorDialog))
    {
        // Allocate memory for "pArticle"
        tArticle *pArticle = safe_malloc_list(sizeof(tArticle), pList);
        // Allocate memory for the description buffer
        pArticle->description = safe_malloc_list(strlen(form->description), pList);
        // Allocate memory for the uid buffer
        pArticle->uid = safe_malloc_list(strlen(form->uid), pList);

        strcpy(pArticle->description, form->description);
        strcpy(pArticle->uid, form->uid);
        pArticle->quantity = form->quantity;

         // Reset "form"
         strcpy(form->description, "");
         strcpy(form->uid, "");
         form->quantity = 0;

         insert_first(pList, pArticle);
         write_database(pList);
         sort_alpha(pList->pHead, pList->pTail);
    }
}

int main()
{
    puts("Context-type:text/html\n\n");

    tList *pList = create_list((void (*)(void*)) free_tArticle); // Create a empty linked list
    read_database(pList);                   // Read database into the linked list
    sort_alpha(pList->pHead, pList->pTail); // Sorts the linked list alphabetically

    tForm form;
    form.description = safe_malloc_list(10, pList);
    form.uid = safe_malloc_list(10, pList);
    // Set form to default
    strcpy(form.description, "");
    strcpy(form.uid, "");
    form.quantity = 0;

    // Set dialogs to closed
    tEditDialog editDialog = {.open = false};
    tErrorDialog errorDialog = {.open = false};

    char *pMethod = getenv("REQUEST_METHOD");
    if (strcmp(pMethod, "GET") == 0)
    {
        print_html(pList, &form, &editDialog, &errorDialog, NULL);
    }
    else if (strcmp(pMethod, "POST") == 0)
    {
        char buffer[128];
        char values[8][80]; // Received POST data
        size_t values_len = 80;
        char searchQuery[100] = "";

        fgets(buffer, 128, stdin);
        // Splitting the data
        char *content = strtok(buffer, "&");
        int i = 0;
        while (content)
        {
            char *value = strstr(content, "=") + 1;
            replace_substr(value, "+", " ");
            char *decoded = url_decode(value);
            if (!decoded)
            {
                fprintf(stderr, "Error decoding string: \"%s\"\n", value);
                continue;
            }
            strcpy(values[i], decoded);
            content = strtok(NULL, "&");
            i++;
        }

        char *type = values[0];               // "values[0]" is reserved for the type
        if (strcmp(type, "article_add") == 0) // Add a new "tArticle"
        {
            // Reallocated memory for the description buffer
            form.description = safe_realloc_list(form.description, values_len + 1, pList); // +1 for the null terminator
            strcpy(form.description, values[1]);

            // Reallocated memory for the uid buffer
            form.uid = safe_realloc_list(form.uid, values_len + 1, pList);
            strcpy(form.uid, values[2]);

            form.quantity = atoi(values[3]);

            add_article(pList, &form, &errorDialog);
        }
        else if (strcmp(type, "table_action") == 0) // Actions like edit or delete
        {
            char *action = values[2];
            if (strcmp(action, "edit_dialog") == 0) // Open the edit dialog
            {
                editDialog.open = true;
                // Allocate memory for the uid buffer
                editDialog.uid = safe_malloc_list(values_len + 1, pList); // +1 for the null terminator
                strcpy(editDialog.uid, values[1]);
            }
            else if (strcmp(action, "delete") == 0) // Remove the "tArticle" by uid
            {
                int index = get_tArticle_index(pList, values[1]);
                remove_index(pList, index); // Remove a node from the linked list by index
                write_database(pList);      // Write linked list into database
            }
        }
        else if (strcmp(type, "article_edit") == 0) // Edit/Update "tArticle" data
        {

            // Reallocated memory for the description buffer
            form.description = safe_realloc_list(form.description, values_len + 1, pList); // +1 for the null terminator
            strcpy(form.description, values[2]);
            // Reallocated memory for the uid buffer
            form.uid = safe_realloc_list(form.uid, values_len + 1, pList); // +1 for the null terminator
            strcpy(form.uid, values[3]);
            form.quantity = atoi(values[4]);

            char *origin = safe_malloc_list(values_len + 1, pList); // Original uid
            strcpy(origin, values[1]);

            if (validate_form(strcmp(origin, form.uid) == 0 ? NULL : pList, &form, &errorDialog)) // If origin and form.uid are the same the uid_exists check is skipped
            {
                int index = get_tArticle_index(pList, origin);

                remove_index(pList, index);              // Remove old article
                add_article(pList, &form, &errorDialog); // Add updated article
            }
            free(origin);
        }
        else if (strcmp(type, "article_search") == 0)
        {
            strncpy(searchQuery, values[1], 100);
            search_tList(pList, searchQuery);
        }
        print_html(pList, &form, &editDialog, &errorDialog, searchQuery);

        // Free allocated memory
        free(editDialog.uid);
        free(form.description);
        free(form.uid);
    }

    free_list(pList);
    return 0;
}