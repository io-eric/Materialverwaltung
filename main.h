/*
 Belegarbeit:
    > Name: Eric Wolf
    > Matr.-Nr.: 53453
    > Bibl.-Nr.: 85876
*/

typedef struct
{
    bool open;
    char *uid;
} tEditDialog;

typedef struct
{
    bool open;
    char title[80];
    char body[128];
} tErrorDialog;

typedef struct
{
    char *uid; // Article number
    char *description;
    int quantity;
} tForm;

/* "print_table_search" will print the search field of an HTML table containing the pervious search query. */
void print_table_search(char query[100]);

/* "print_table_body" will print the body of an HTML table containing the tArticle
 data in the linked list pointed to by "pList". */
void print_table_body(tList *pList, char query[100]);

/* "print_edit_dialog" will print an HTML dialog box containing a form for editing
he data of the tArticle pointed to by "pArticle".
The fields of the form will be populated with the current data of the tArticle. */
void print_edit_dialog(tArticle *pArticle);

/* "print_error_dialog" will print an HTML dialog box containing an error message and
additional information about the error stored in the "pErrorDialog" struct. */
void print_error_dialog(tErrorDialog *pErrorDialog);

/* "print_create_form" will print an HTML form for creating a new tArticle.
The fields of the form will be populated with default data stored in the "form" struct */
void print_create_form(tForm *form);

/* "print_html" will print an HTML page */
void print_html(tList *pList, tForm *form, tEditDialog *pEditDialog, tErrorDialog *pErrorDialog, char lastSearchQuery[100]);

/* "validate_form" will validate user input from the "form" struct to create or edit a tArticle.
It checks that the input is valid and does not conflict with existing articles in the linked list
pointed to by "pList". */
bool validate_form(tList *pList, tForm *form, tErrorDialog *pErrorDialog);

/* "add_article" will add a new tArticle to the linked list pointed to by "pList" using the data from the "form" struct. */
void add_article(tList *pList, tForm *form, tErrorDialog *pErrorDialog);