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
// PARAMETERS :       none.
// RETURNS    :       none.
//
void displayMenu(void) {
	printf("\n----------MAIN MENU----------\n");
	printf("1.Add a book into system.\n");
	printf("2.View all books in the system.\n");
	printf("3.Update a book in the system.\n");
	printf("4.Delete a book from the system.\n");
	printf("5.Search for a book in the system.\n");
	printf("6.Exit Program");
	printf("\n-----------------------------\n");
	printf("Enter the number for the option you desire:");
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
	BookNode* head = NULL;
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
			viewBooks(bookList);
			break;
		case 3:
			printf("Updating book in system...\n");
			updateBook(bookList);
			break;
		case 4:
			printf("Deleting book in system...\n");
			deleteABook(&bookList);
			break;
		case 5:
			printf("Searching for a book in system...\n");
			searchABook(bookList);
			break;
		case 6:
			printf("Exiting Program...\n");
			loopRunning = false;
			break;
		default:
			printf("Invalid choice. Please select a number between 1 and 5.\n");
		}

		//Ends looping menu
		if (loopRunning == false)
		{
			break;
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
		
		//Gets the user input  for the choice
		scanf_s(" %c", &choice, (unsigned)sizeof(choice));

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
//                    const char* title, a pointer to the title (or part of it) the user wants to search for.
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
void viewBooks(BookNode* head)
{
    BookNode* i = head;
	//Loop that will run through the list with books in it
    while (i != NULL)
    {
        printf("ID: %d | Title: %s | Author: %s | Publication Year: %d\n", i->data.bookID, i->data.title, i->data.author, i->data.publicationYear);
		//This sends it to the next book in the list
        i = i->next; 
    }
	//if the list is empty display error message
    if (head == NULL)
    {
        printf("There are no books in the catalogue\n");
    } 
}


/*
 * FUNCTION   : updateBook
 * DESCRIPTION: Prompts the user to enter the ID of the book to update. If found, allows modification of the Title, Author,
 *              and Publication Year. If not found, displays an appropriate message
 * PARAMETERS : head of a pointer to the head of the linked list of BookNode struct.
 * RETURNS    : VOID
 */
void updateBook(BookNode* head) {
	char inputBuffer[128];
	int id;

	//Asks the user for the Book ID to update
	printf("\nEnter the Book ID to update: ");
	if (!fgets(inputBuffer, sizeof(inputBuffer), stdin)) {
		printf("Error reading input\n");
		return;
	}
	//Remove the '\n' and checks if the id is valid!
	inputBuffer[strcspn(inputBuffer, "\n")] = '\0';
	if (sscanf_s(inputBuffer, "%d", &id) != 1) {
		printf("Invalid input, please enter a valid book id!\n");
		return;
	}

	//Search for the book by the ID
	BookNode* current = head;
	while (current != NULL && current->data.bookID != id) {
		current = current->next;
	}
	
	//If the book is not found it breaks
	if (current == NULL) {
		printf("No book with ID %d found in the system!\n", id);
		return;
	}

	printf("Book with ID %d found\n", id);

	//Updating the title
	printf("Current Title: %s\n", current->data.title);
	printf("Enter new Title: ");
	if (!fgets(inputBuffer, sizeof(inputBuffer), stdin)) {
		printf("Error reading title!\n");
		return;
	}
	inputBuffer[strcspn(inputBuffer, "\n")] = '\0';
	if (strlen(inputBuffer) > 0) {
		strcpy_s(current->data.title, TITLE_LENG, inputBuffer);
	}

	//Updating the author
	printf("Current Author: %s\n", current->data.author);
	printf("Enter new Author: ");
	if (!fgets(inputBuffer, sizeof(inputBuffer), stdin)) {
		printf("Error reading author!\n");
		return;
	}
	inputBuffer[strcspn(inputBuffer, "\n")] = '\0';
	if (strlen(inputBuffer) > 0) {
		strcpy_s(current->data.author, AUTH_LENG, inputBuffer);
	}

	//Updating the publication year
	int newYear;
	printf("Current Publication Year: %d\n", current->data.publicationYear);
	printf("Enter new Publication Year: ");
	if (!fgets(inputBuffer, sizeof(inputBuffer), stdin)) {
		printf("Error reading publication year!\n");
		return;
	}
	inputBuffer[strcspn(inputBuffer, "\n")] = '\0';
	if (strlen(inputBuffer) > 0) {
		if (sscanf_s(inputBuffer, "%d", &newYear) != 1) {
			printf("Invalid input for publication year, no update made for year!\n");
		}
		else {
			current->data.publicationYear = newYear;
		}
	}

	printf("Book updated successfully.\n");
}


/*
FUNCTION: deleteABook
DESCRIPTION:
This function deletes a book from the system (singly linked list) by performing a case?insensitive search on the book title.
It displays all matching books and then deletes the one corresponding to the entered Book ID.
After each deletion (or if no match is found), it allows the user to delete another book. The user can exit this function by entering '^' when prompted for a book title.
PARAMETERS: BookNode** head, a pointer to the pointer of the head of the list
RETURNS: none.
*/
void deleteABook(BookNode** head)
{
	char anotherDelete;  /* Stores the user's choice to delete another book */

	do
	{
		/* Check if the list is empty */
		if (*head == NULL)
		{
			printf("There are no books in the system to delete.\n");
			return;
		}

		char searchTitle[TITLE_LENG];

		/*
		Prompting for a Title with Exit Option:
		The user is asked to enter a book title (or part of it) to search for deletion.
		The prompt informs the user to enter '^' to return to the main menu.
		If the user presses Enter without typing anything, they are reprompted.
		*/
		int validTitle = 0;
		while (!validTitle)
		{
			printf("Enter the book title to delete ('^' to go back):");
			if (fgets(searchTitle, TITLE_LENG, stdin) == NULL)
				continue;
			size_t len = strlen(searchTitle);
			if (len > 0 && searchTitle[len - 1] == '\n')
				searchTitle[len - 1] = '\0';  /* Remove the newline */
			if (strlen(searchTitle) == 0)
			{
				printf("Please enter a book title or enter '^' to go back to the main menu.\n");
				continue;
			}
			if (strcmp(searchTitle, "^") == 0)
				return;  /* Exit deletion function */
			validTitle = 1;
		}

		/*
		Case Insensitive Matching:
		Iterate through the linked list. Use the helper function titleContains to check for a match (ignoring case). Display each matching book.
		*/
		int foundCount = 0;
		BookNode* current = *head;
		printf("\nMatching Books:\n");
		printf("\n");/* To add an invisable space to make it easier to read*/
		while (current != NULL)
		{
			if (titleContains((*current).data.title, searchTitle))
			{
				printf("Book ID: %d\n", (*current).data.bookID);
				printf("Title: %s\n", (*current).data.title);
				printf("Author: %s\n", (*current).data.author);
				printf("Publication Year: %d\n", (*current).data.publicationYear);
				printf("\n");
				foundCount++;
			}
			current = (*current).next;

		}

		/*
		If no matching books are found. Inform the user and ask if they would like to search for a different book.
		*/
		if (foundCount == 0)
		{
			printf("The book you are looking for \"%s\" is not in the system.\n", searchTitle);
			char choice;
			int validInput = 0;
			do
			{
				printf("Would you like to look for a different book to delete? (Y/N): ");
				if (scanf_s(" %c", &choice, (unsigned int)sizeof(choice)) != 1)
				{
					printf("Invalid input. Please select Y or N.\n");
					while (getchar() != '\n');
					continue;
				}
				while (getchar() != '\n');  /* Clear the input buffer */
				if (choice == 'Y' || choice == 'y' || choice == 'N' || choice == 'n')
					validInput = 1;
				else
				{
					printf("Please select Y or N.\n");
					validInput = 0;
				}
			} while (!validInput);

			if (choice == 'Y' || choice == 'y')
				continue;  /* Restart deletion loop */
			else
				break;     /* Exit deletion function */
		}

		/*
		Choosing a Book to Delete: Prompt the user to enter the Book ID for the book to delete.
		*/
		int id;
		printf("Enter the Book ID to delete: ");
		if (scanf_s("%d", &id) != 1)
		{
			printf("Invalid input. Please enter a valid Book ID.\n");
			while (getchar() != '\n');  /* Clear input buffer */
			continue;  /* Restart deletion loop */
		}
		while (getchar() != '\n');  /* Clear any leftover newline characters */

		/*
		Locate the Book Node: Traverse the list to find the book with the specified Book ID.
		*/
		current = *head;
		BookNode* previous = NULL;
		while (current != NULL && (*current).data.bookID != id)
		{
			previous = current;
			current = (*current).next;
		}

		if (current == NULL)
		{
			printf("The book with ID %d is not found in the system.\n", id);
			char choice;
			int validInput = 0;
			do
			{
				printf("Would you like to look for a different book to delete? (Y/N): ");
				if (scanf_s(" %c", &choice, (unsigned int)sizeof(choice)) != 1)
				{
					printf("Invalid input. Please select Y or N.\n");
					while (getchar() != '\n');
					continue;
				}
				while (getchar() != '\n');  /* Clear input buffer */
				if (choice == 'Y' || choice == 'y' || choice == 'N' || choice == 'n')
					validInput = 1;
				else
				{
					printf("Please select Y or N.\n");
					validInput = 0;
				}
			} while (!validInput);

			if (choice == 'Y' || choice == 'y')
				continue;
			else
				break;
		}

		/*
		Confirm Deletion: Ask the user to confirm deletion of the selected book and validate that only Y/y or N/n are accepted.
		*/
		char confirm;
		int validInput = 0;
		do
		{
			printf("You have selected the book \"%s\" (ID: %d). Are you sure you would like to delete this book? (Y/N): ", (*current).data.title, (*current).data.bookID);
			if (scanf_s(" %c", &confirm, (unsigned int)sizeof(confirm)) != 1)
			{
				printf("Invalid input. Please select Y or N.\n");
				while (getchar() != '\n');
				continue;
			}
			while (getchar() != '\n');  /* Clear input buffer */
			if (confirm == 'Y' || confirm == 'y' || confirm == 'N' || confirm == 'n')
				validInput = 1;
			else
			{
				printf("Please select Y or N.\n");
				validInput = 0;
			}
		} while (!validInput);

		if (confirm == 'N' || confirm == 'n')
		{
			/*
			If the user cancels deletion:
			- Ask if they want to delete another book.
			*/
			char choice;
			int validChoice = 0;
			do
			{
				printf("Would you like to delete another book? (Y/N): ");
				if (scanf_s(" %c", &choice, (unsigned int)sizeof(choice)) != 1)
				{
					printf("Invalid input. Please select Y or N.\n");
					while (getchar() != '\n');
					continue;
				}
				while (getchar() != '\n');  /* Clear input buffer */
				if (choice == 'Y' || choice == 'y' || choice == 'N' || choice == 'n')
					validChoice = 1;
				else
				{
					printf("Please select Y or N.\n");
					validChoice = 0;
				}
			} while (!validChoice);

			if (choice == 'Y' || choice == 'y')
				continue;
			else
				break;
		}

		/*
		Proceed with Deletion: Save the title for the success message and remove the node from the list (updating the head pointer if necessary) and free its memory.
		*/
		char deletedTitle[TITLE_LENG];
		strcpy_s(deletedTitle, TITLE_LENG, (*current).data.title);
		if (previous == NULL)
			*head = (*current).next;
		else
			(*previous).next = (*current).next;
		free(current);

		printf("The book \"%s\" has been successfully deleted.\n", deletedTitle);

		/*
		Ask if the user would like to delete another book.
		Validate the input to accept only Y/y or N/n.
		*/
		int validInput2 = 0;
		do
		{
			printf("Would you like to delete another book? (Y/N): ");
			if (scanf_s(" %c", &anotherDelete, (unsigned int)sizeof(anotherDelete)) != 1)
			{
				printf("Invalid input. Please select Y or N.\n");
				while (getchar() != '\n');
				continue;
			}
			while (getchar() != '\n');  /* Clear input buffer */
			if (anotherDelete == 'Y' || anotherDelete == 'y' || anotherDelete == 'N' || anotherDelete == 'n')
				validInput2 = 1;
			else
			{
				printf("Please select Y or N.\n");
				validInput2 = 0;
			}
		} while (!validInput2);

	} while (anotherDelete == 'Y' || anotherDelete == 'y');
}

