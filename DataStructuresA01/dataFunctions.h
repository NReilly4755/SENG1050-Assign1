/*
* FILE			: dataFunctions.h
* PROJECT		: Data Structures Assignment 1
* PROGRAMMERS	: NICHOLAS REILLY, RODRIGO GOMES, BRYAN LEONOR, RYAN KIPKAZI
* FIRST VERSION	: 2025-01-27
* DESCRIPTION	: The header file that will hold defintions, declarations and function protoypes.
* REFERENCING   : Deitel, P., & Deitel, H. (2016). How to Program in C and C++ (8th ed.). Deitel & Associates Inc.
*/

#pragma once

//Referencing libraries used to porcess functions.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//Defining maximum array length definitions.
#define TITLE_LENG 50
#define AUTH_LENG 50

//Defining the Book struct type.
typedef struct {
	int bookID;
	char title[TITLE_LENG];
	char author[AUTH_LENG];
	int publicationYear;
}Book;

//Defining the BookNode struct type. I implemented this to traverse and create pointers for the books.
typedef struct BookNode {
	//The actual Book data entry.
	Book data;
	//The pointer to the next book.
	struct BookNode* next;
} BookNode;


//Function prototypes.
void displayMenu(void);
void mainMenu(void);
void addABook(BookNode** head);
void searchABook(BookNode* head);
int isDuplicateID(BookNode* head, int id);
int titleContains(const char* title, const char* query);
void toLowerCase(char* str);
void deleteABook(BookNode** head);