#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BOOK_FILE "books.txt"
#define STUDENT_BOOK_FILE "student_books.txt"

#define MAX_NAME_LEN 40
#define MAX_AUTHOR_LEN 30
#define MAX_DATE_LEN 20

typedef struct {
    int id;
    char name[MAX_NAME_LEN];
    char author[MAX_AUTHOR_LEN];
    char publishedDate[MAX_DATE_LEN];
} Book;

// Declare a global variable to store the username
char username[100];

// Clear display
void clearDisplay() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Replace the manual username prompt with reading from TEMP_USER_FILE
void readUsername() {
    FILE *file = fopen("temp_user.txt", "r");
    if (file != NULL) {
        fgets(username, sizeof(username), file);
        username[strcspn(username, "\n")] = '\0';  // Remove newline character
        fclose(file);
    } else {
        printf("Error: Unable to retrieve username. Please log in again.\n");
        exit(1);  // Exit if username file is missing
    }
}

// Display book list from books.txt
void displayBookList() {
    FILE *file = fopen(BOOK_FILE, "r");
    Book book;

    if (file == NULL) {
        printf("No books found.\n");
        return;
    }

    int id_width = 5;
    int name_width = MAX_NAME_LEN;
    int author_width = MAX_AUTHOR_LEN;

    // Calculate total table width dynamically
    int table_width = id_width + name_width + author_width + 10; // +10 for separators and padding

    printf("Book List:\n");
    for (int i = 0; i < table_width; i++) printf("-");
    printf("\n");

    // Print the header with centered text
    printf("| %-*s | %-*s | %-*s |\n",
           id_width, "ID",
           name_width, "Name",
           author_width, "Author");

    for (int i = 0; i < table_width; i++) printf("-");
    printf("\n");

    // Print the book data with centered text alignment
    while (fscanf(file, "%d %[^\n] %[^\n] %[^\n]\n", &book.id, book.name, book.author, book.publishedDate) != EOF) {
        printf("| %*d | %-*s | %-*s |\n",
               id_width, book.id,
               name_width, book.name,
               author_width, book.author);
    }

    for (int i = 0; i < table_width; i++) printf("-");
    printf("\n\n");
    fclose(file);
}

// Search for a book by title
void searchBook() {
    int searchChoice;
    int searchID;
    char searchTerm[100];
    Book book;
    int found = 0;

    printf("Search Options:\n");
    printf("1. Search by ID\n");
    printf("2. Search by Name\n");
    printf("3. Search by Author\n");
    printf("Enter your choice (1-3): ");
    if (scanf("%d", &searchChoice) != 1) {
        printf("Invalid input.\n");
        while (getchar() != '\n'); // Clear invalid input
        return;
    }
    getchar();  // Consume newline character

    FILE *file = fopen(BOOK_FILE, "r");
    if (file == NULL) {
        printf("No books found.\n");
        return;
    }

    switch (searchChoice) {
        case 1:
            printf("Enter the Book ID to search: ");
            scanf("%d", &searchID);
            getchar(); // Consume newline character

            while (fscanf(file, "%d %[^\n] %[^\n] %[^\n]\n", &book.id, book.name, book.author, book.publishedDate) != EOF) {
                if (book.id == searchID) {
                    printf("\nFound Book:\n");
                    printf("ID: %d\nName: %s\nAuthor: %s\nPublished Date: %s\n\n", book.id, book.name, book.author, book.publishedDate);
                    found = 1;
                    break;
                }
            }
            break;

        case 2:
            printf("Enter the Book Name to search: ");
            fgets(searchTerm, sizeof(searchTerm), stdin);
            searchTerm[strcspn(searchTerm, "\n")] = '\0'; // Remove newline

            printf("\nSearch Results by Name:\n");
            while (fscanf(file, "%d %[^\n] %[^\n] %[^\n]\n", &book.id, book.name, book.author, book.publishedDate) != EOF) {
                if (strstr(book.name, searchTerm) != NULL) {
                    printf("ID: %d\nName: %s\nAuthor: %s\nPublished Date: %s\n\n", book.id, book.name, book.author, book.publishedDate);
                    found = 1;
                }
            }
            break;

        case 3:
            printf("Enter the Author Name to search: ");
            fgets(searchTerm, sizeof(searchTerm), stdin);
            searchTerm[strcspn(searchTerm, "\n")] = '\0'; // Remove newline

            printf("\nSearch Results by Author:\n");
            while (fscanf(file, "%d %[^\n] %[^\n] %[^\n]\n", &book.id, book.name, book.author, book.publishedDate) != EOF) {
                if (strstr(book.author, searchTerm) != NULL) {
                    printf("ID: %d\nName: %s\nAuthor: %s\nPublished Date: %s\n\n", book.id, book.name, book.author, book.publishedDate);
                    found = 1;
                }
            }
            break;

        default:
            printf("Invalid search choice.\n");
            break;
    }

    if (!found) {
        printf("No matching books found.\n");
    }

    fclose(file);
}

// Take a book by ID
void takeBook() {
    int bookID;
    char line[256];
    Book book;
    FILE *file = fopen(BOOK_FILE, "r");
    FILE *borrowedFile = fopen(STUDENT_BOOK_FILE, "a");

    if (file == NULL || borrowedFile == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter the ID of the book to take: ");
    scanf("%d", &bookID);
    getchar();

    int found = 0;
    while (fscanf(file, "%d %[^\n] %[^\n] %[^\n]\n", &book.id, book.name, book.author, book.publishedDate) != EOF) {
        if (book.id == bookID) {
            fprintf(borrowedFile, "%s - Borrowed: %d %s\n", username, book.id, book.name);
            printf("Book '%s' successfully borrowed.\n\n", book.name);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Book ID not found.\n");
    }

    fclose(file);
    fclose(borrowedFile);
}

// Return a book by ID
void returnBook() {
    int bookID;
    char line[256];
    int found = 0;

    printf("Enter the ID of the book to return: ");
    scanf("%d", &bookID);
    getchar(); // Consume newline character left in the input buffer

    FILE *file = fopen(STUDENT_BOOK_FILE, "r");
    FILE *tempFile = fopen("temp.txt", "w");

    if (file == NULL || tempFile == NULL) {
        printf("Error opening file!\n");
        return;
    }

    // Search through each line to find the specific book borrowed by the user with the given ID
    while (fgets(line, sizeof(line), file) != NULL) {
        // Format to look for: "<username> - Borrowed: <bookID> <bookName>"
        char tempUsername[100];
        int tempID;

        // Parse the line for username and book ID
        sscanf(line, "%s - Borrowed: %d", tempUsername, &tempID);

        // Check if the line matches the current user and the specified book ID
        if (strcmp(tempUsername, username) == 0 && tempID == bookID) {
            found = 1; // Mark as found
            continue; // Skip writing this line to the temp file
        }
        fputs(line, tempFile); // Copy other lines to the temporary file
    }

    fclose(file);
    fclose(tempFile);

    // Replace the original file with the modified file
    remove(STUDENT_BOOK_FILE);
    rename("temp.txt", STUDENT_BOOK_FILE);

    // Notify the user of the result
    if (found) {
        printf("Book with ID %d successfully returned.\n\n", bookID);
    } else {
        printf("Book with ID %d not found in your records.\n\n", bookID);
    }
}

// View borrowing history by name instead of ID
void viewHistory() {
    char line[256];
    FILE *file = fopen(STUDENT_BOOK_FILE, "r");
    int found = 0;

    if (file == NULL) {
        printf("No borrowing history found.\n");
        return;
    }

    printf("\nBorrowing History:\n");
    printf("------------------\n");
    while (fgets(line, sizeof(line), file) != NULL) {
        if (strstr(line, username)) {
            printf("%s", line);
            found = 1;
        }
    }

    if (!found) {
        printf("No borrowing history found for you.\n");
    }

    fclose(file);
    printf("\n");
}

// Main function
int main() {
    int choice;

    // Read the username from the temporary file
    readUsername();

    printf("\n---Library Management System---\n\n");

    while (1) {
        printf("1. Book list\n");
        printf("2. Search Books\n");
        printf("3. Take Book\n");
        printf("4. Return Book\n");
        printf("5. History\n");
        printf("6. Clear Display\n");
        printf("7. Exit\n");
        printf("Enter your choice (1-7): ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            // Clear the invalid input
            while (getchar() != '\n');
            continue;
        }
        getchar(); // Consume the newline character left by scanf

        switch (choice) {
            case 1:
                printf("\n");
                displayBookList();
                break;
            case 2:
                printf("\n");
                searchBook();
                break;
            case 3:
                printf("\n");
                takeBook();
                break;
            case 4:
                printf("\n");
                returnBook();
                break;
            case 5:
                printf("\n");
                viewHistory();
                break;
            case 6:
                clearDisplay();
                printf("\n---Library Management System---\n\n");
                break;
            case 7:
                printf("Exiting the system. Goodbye!\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}
