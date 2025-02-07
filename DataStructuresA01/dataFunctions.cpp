/*
* FILE			: dataFunctions.cpp
* PROJECT		: Data Structures Assignment 1
* PROGRAMMERS	: NICHOLAS REILLY, RODRIGO GOMES, BRYAN LEONOR, RYAN KIPKAZI
* FIRST VERSION	: 2025-02-01
* DESCRIPTION	: The C++ file that will hold the source code for all of the functions used in the program.
* REFERENCING   : Deitel, P., & Deitel, H. (2016). How to Program in C and C++ (8th ed.). Deitel & Associates Inc.
*/

//Referecing the custom library made in the project.
#include "dataFunctions.h"

//
// FUNCTION   :       displayMenu
// DESCRIPTION:       This function displays all options to the user for the book system.
// PARAMETERS :       none
// RETURNS    :       none.
//
void displayMenu(void) {
    printf("----------MAIN MENU----------\n");
    printf("1.Add a book into system.\n");
    printf("2.View all books in the system.\n");
    printf("3.Update a book in the system.\n");
    printf("4.Delete a book from the system.\n");
    printf("5.Search for a book in the system.\n");
    printf("Enter the number for the option you desire:\n");
}


//
// FUNCTION   :       mainMenu
// DESCRIPTION:       This function acts as main function and holds options and calls for the book system.
// PARAMETERS :       none.
// RETURNS    :       none.
//
void mainMenu(void) {

    //Declaring all local variables used in the program.
    BookNode* bookList = NULL;
    int choice;
    bool loopRunning = true;

    //While loopRunning is true, always run. Madhan mentioned no exit call in rubric so it runs forever?
    while (loopRunning) {
        displayMenu();
        //Validates choice is acceptable and removes the newline character that scanf_s adds
        if (scanf_s("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number between 1 and 5.\n");
            while (getchar() != '\n');
            continue;
        }

        //Clear the buffer when returning back to the main menu from another function. 
        getchar();

        //Switch that manages choices the user selects
        switch (choice) {
        case 1:
            printf("Adding book into system...\n");
            addABook(&bookList);
            break;
        case 2:
            printf("Viewing all books in system...\n");
            break;
        case 3:
            printf("Updating book in system...\n");
            break;
        case 4:
            printf("Deleting book in system...\n");
            break;
        case 5:
            printf("Searching for a book in system...\n");
            searchABook(bookList);
            break;
        default:
            printf("Invalid choice. Please select a number between 1 and 5.\n");
        }

    }
}

//
// FUNCTION   :       addABook
// DESCRIPTION:       This function creates instances of the Book struct type and places them in memory for referncing
// PARAMETERS :       Booknode** head, the pointer of a pointer that references the beginning of the list and allows for modification
// RETURNS    :       none.
//
void addABook(BookNode** head) {
    char choice;

    //Create instances of Book struct type in memory as long as the function is called.
    do {
        BookNode* newBook = (BookNode*)malloc(sizeof(BookNode));
        if (newBook == NULL) {
            printf("Memory allocation failed!\n");
            return;
        }


        //Logic that confirms the book id does not already exist in the system.
        int validID = 0;
        while (!validID) {
            printf("Enter Book ID: ");
            scanf_s("%d", &newBook->data.bookID);

            if (isDuplicateID(*head, newBook->data.bookID)) {
                printf("Error: Book ID already exists. Try again.\n");
            }
            else {
                validID = 1;
            }
        }

        //Getting the details of the book from the user.
        printf("Enter Book Title: ");
        //Consume the newline character left behind from entering function.
        getchar();
        fgets(newBook->data.title, TITLE_LENG, stdin);
        newBook->data.title[strcspn(newBook->data.title, "\n")] = '\0'; // Remove newline

        printf("Enter Author Name: ");
        fgets(newBook->data.author, AUTH_LENG, stdin);
        newBook->data.author[strcspn(newBook->data.author, "\n")] = '\0';

        printf("Enter Publication Year: ");
        scanf_s("%d", &newBook->data.publicationYear);

        newBook->next = NULL;

        //Add this to the end of the list.
        if (*head == NULL) {
            *head = newBook;
        }
        else {
            BookNode* temp = *head;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = newBook;
        }

        printf("Book added successfully!\n");

        //Ask the user if they want to add another book into the system.
        printf("Do you want to add another book? (y/n): ");
        //Consume the newline before getting choice to confirm the choice is picked up only.
        getchar();
        scanf_s("%c", &choice);

    } while (choice == 'y' || choice == 'Y');
}

//
// FUNCTION   :       searchABook
// DESCRIPTION:       This function takes a full or partial word and comapres it again titles of all book instances and displays results.
// PARAMETERS :       Booknode* head, a pointer to the head of the singly linked list so it knows where to start the search.
// RETURNS    :       none.
//
void searchABook(BookNode* head) {
    char searchTitle[TITLE_LENG];
    char choice;

    //Logic that gathers the title of the book until a result is found. 
    do {
        printf("\nEnter the book title to search (partial or full): ");
        fgets(searchTitle, TITLE_LENG, stdin);
        searchTitle[strcspn(searchTitle, "\n")] = '\0';

        int found = 0;
        BookNode* current = head;

        printf("\nSearch Results:\n");
        printf("-------------------------------------------------\n");

        while (current != NULL) {
            if (titleContains(current->data.title, searchTitle)) {
                printf("Book ID: %d\n", current->data.bookID);
                printf("Title: %s\n", current->data.title);
                printf("Author: %s\n", current->data.author);
                printf("Publication Year: %d\n", current->data.publicationYear);
                printf("-------------------------------------------------\n");
                found = 1;
            }
            current = current->next;
        }

        //exception if the entry made by the user does not yield any results.
        if (!found) {
            printf("No books found matching \"%s\".\n", searchTitle);
        }

        //Ask user if they want to search again
        printf("\nWould you like to search for another book? (Y/N): ");
        scanf_s(" %c", &choice, 1);
        //Clear the buffer so the choice is the only thing evaluated.
        while (getchar() != '\n');

    } while (choice == 'Y' || choice == 'y');

    printf("\nReturning to main menu...\n");
}


//
// FUNCTION   :       isDuplicateID
// DESCRIPTION:       This function takes a book ID and confirms that no other entries of it exist in the list. 
// PARAMETERS :       Booknode* head, a pointer to the head of the singly linked list so it knows where to start the search.
//                    int id, which is the reference to the .bookID value of the Book struct to check for.
// RETURNS    :       0 if it is a not a duplicate entry.
//                    -1 if it is a duplicate.
//
int isDuplicateID(BookNode* head, int id) {

    //The current data type is pointing to the beginning of the list. As long as it is not NULL (not at the end), keep going and comapre the ID.
    BookNode* current = head;
    while (current != NULL) {
        if (current->data.bookID == id) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

//
// FUNCTION   :       toLowerCase
// DESCRIPTION:       This function takes a char array and evaluates each index entry and applies the tolower function from ctype.h
// PARAMETERS :       char* str, a pointer to the word entered in to refernce.
// RETURNS    :       none.
//
void toLowerCase(char* str) {
    while (*str) {
        *str = (char)tolower((unsigned char)*str);
        str++;
    }
}

//
// FUNCTION   :       titleContains
// DESCRIPTION:       This function checks if the provided title (either full or partial) 
//                    is contained within any of the book titles in the linked list, 
//                    performing a case-insensitive comparison. 
// PARAMETERS :       BookNode* head,  a pointer to the head of the singly linked list where the search starts.
//                    const char* title, a poiinter to the title (or part of it) the user wants to search for.
// RETURNS    :       true, if it is found.
//                    false if no results were yielded.
//
int titleContains(const char* title, const char* query) {

    //Set the arrays to match the length ceilings.
    char lowerTitle[TITLE_LENG], lowerQuery[TITLE_LENG];

    //Copy all of the contents, the size of the array, and the source of data that is going into the new arrays.
    strcpy_s(lowerTitle, sizeof(lowerTitle), title);
    strcpy_s(lowerQuery, sizeof(lowerQuery), query);

    //Convert all to lowercase so that way case sensitivity is not an issue.
    toLowerCase(lowerTitle);
    toLowerCase(lowerQuery);

    //Check if the query is in the title
    return strstr(lowerTitle, lowerQuery) != NULL;
}

//
// FUNCTION   :       viewBooks
// DESCRIPTION:       This function runs through and displays all books within the linked list
// PARAMETERS :       BookNode* head, a pointer to the start of the linked list
// RETURNS    :       Nothing
//
static void viewBooks(BookNode* head)
{
    BookNode* i = head;
    while (i != NULL)
    {
        printf("ID: %d | Title: %s | Author: %s | Publication Year: %d", i->data.bookID, i->data.title, i->data.author, i->data.publicationYear);
        i = i->next;
    }

    if (i == NULL)
    {
        printf("There are no books in the catalogue"); 
    }
}