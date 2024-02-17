## Project: Material Management - CGI Web Application

### Description
This project implements a CGI web application for managing materials. The application allows users to add, edit, search, and delete records, where each record includes the article name, article number, and stock level.

[PREVIEW](https://github.com/EricSource-io/Materialverwaltung/blob/main/preview.png)

## Compilation:

#### Command Line:
```bash
gcc lib/utils/utils.c lib/linkedlist/linkedlist.c lib/database/database.c main.c -o beleg.cgi
```
## Functionalities:
- **Add Article:** Users can add new articles with article name, article number, and stock level.
- **Edit Article:** Ability to edit existing records.
- **Search Article:** Search records based on article name, article number, or stock level.
- **Delete Article:** Delete records from the material management system.

## Data Storage:
- The data is saved when exiting the program if the data has changed during program execution.

## Stock Level Modification:
- Stock levels can be modified through a separate menu item (e.g., "Inbound"/"Outbound"/"Goods Receipt").

## Task Statement:
The complete task statement for this CGI web application is available on the [university's website](https://www2.htw-dresden.de/~beck/PSPI/Belegaufgaben/PspCB2.html).

## Technologies:
- The application is implemented in C++.
- Use of CGI technology for the web application.

## Procedure:
1. Implement functionalities for adding, deleting, searching, and displaying records.
2. Integrate stock level modifications through the separate menu item.

## Used Resources:
- [University's Website - Assignment Details](https://www2.htw-dresden.de/~beck/PSPI/Belegaufgaben/PspCB2.html)

