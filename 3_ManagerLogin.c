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

typedef struct {
    char studentName[100];
    int bookCount;
    char books[100][100];
} StudentRecord;

void displayStudentBorrowings() {
    FILE *file = fopen(STUDENT_BOOK_FILE, "r");
    char line[256];
    StudentRecord students[100];  // Assuming a maximum of 100 students for simplicity
    int studentCount = 0;

    if (file == NULL) {
        printf("No borrowing history found.\n");
        return;
    }

    // Read each line in the file
    while (fgets(line, sizeof(line), file) != NULL) {
        char username[100];
        int bookID;
        char bookName[100];

        // Parse the line assuming format "<username> - Borrowed: <bookID> <bookName>"
        sscanf(line, "%s - Borrowed: %d %[^\n]", username, &bookID, bookName);

        // Check if this student already exists in our records
        int found = 0;
        for (int i = 0; i < studentCount; i++) {
            if (strcmp(students[i].studentName, username) == 0) {
                // Add this book to their list
                strcpy(students[i].books[students[i].bookCount], bookName);
                students[i].bookCount++;
                found = 1;
                break;
            }
        }

        // If the student is not found, add a new record
        if (!found) {
            strcpy(students[studentCount].studentName, username);
            strcpy(students[studentCount].books[0], bookName);
            students[studentCount].bookCount = 1;
            studentCount++;
        }
    }

    fclose(file);

    // Display the results
    printf("Student Taken (Borrowing) Summary:\n");
    printf("--------------------------\n");
    for (int i = 0; i < studentCount; i++) {
        printf("Student: %s\n", students[i].studentName);
        printf("Total Books Borrowed: %d\n", students[i].bookCount);
        printf("Books:\n");
        for (int j = 0; j < students[i].bookCount; j++) {
            printf("  - %s\n", students[i].books[j]);
        }
        printf("\n");
    }
}

// Function to get the next available ID
int getNextID() {
    FILE *file = fopen(BOOK_FILE, "r");
    Book book;
    int lastID = 1000;

    if (file == NULL) {
        return 1001; // Start with 1001 if file does not exist
    }

    while (fscanf(file, "%d %[^\n] %[^\n] %[^\n]\n", &book.id, book.name, book.author, book.publishedDate) != EOF) {
        lastID = book.id;
    }

    fclose(file);
    return lastID + 1;
}

// Function to display all books
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
    printf("\n");
    fclose(file);
}
// Function to search for a book by name
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

            while (fscanf(file, "%d %[^\n] %[^\n] %[^\n]\n\n", &book.id, book.name, book.author, book.publishedDate) != EOF) {
                if (book.id == searchID) {
                    printf("\nFound Book:\n");
                    printf("ID: %d\nName: %s\nAuthor: %s\nPublished Date: %s\n", book.id, book.name, book.author, book.publishedDate);
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

// Function to add a new book
void addBook() {
    Book book;
    FILE *file = fopen(BOOK_FILE, "a");

    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    // Generate the next ID
    book.id = getNextID();

    printf("Enter Book Name: ");
    fgets(book.name, sizeof(book.name), stdin);
    book.name[strcspn(book.name, "\n")] = '\0';

    printf("Enter Author: ");
    fgets(book.author, sizeof(book.author), stdin);
    book.author[strcspn(book.author, "\n")] = '\0';

    printf("Enter Published Date: ");
    fgets(book.publishedDate, sizeof(book.publishedDate), stdin);
    book.publishedDate[strcspn(book.publishedDate, "\n")] = '\0';

    fprintf(file, "%d\n%s\n%s\n%s\n", book.id, book.name, book.author, book.publishedDate);
    fclose(file);

    printf("Book '%s' successfully added with ID %d.\n\n", book.name, book.id);
}

// Function to delete a book by ID and adjust IDs
void deleteBook() {
    int deleteID;
    Book book;
    int found = 0, currentID = 1001;

    printf("Enter the ID of the book to delete: ");
    scanf("%d", &deleteID);
    getchar();  // Consume newline character

    FILE *file = fopen(BOOK_FILE, "r");
    FILE *tempFile = fopen("temp_books.txt", "w");

    if (file == NULL || tempFile == NULL) {
        printf("Error opening file!\n");
        return;
    }

    // Copy all books except the one to delete, adjusting IDs sequentially
    while (fscanf(file, "%d %[^\n] %[^\n] %[^\n]\n", &book.id, book.name, book.author, book.publishedDate) != EOF) {
        if (book.id == deleteID) {
            found = 1;
            continue;  // Skip this book
        }
        book.id = currentID++; // Reassign IDs sequentially
        fprintf(tempFile, "%d\n%s\n%s\n%s\n", book.id, book.name, book.author, book.publishedDate);
    }

    fclose(file);
    fclose(tempFile);

    remove(BOOK_FILE);
    rename("temp_books.txt", BOOK_FILE);

    if (found) {
        printf("Book with ID %d successfully deleted and IDs adjusted.\n\n", deleteID);
    } else {
        printf("Book with ID %d not found.\n\n", deleteID);
    }
}

// Function to update a book by ID
void updateBookByID() {
    int updateID;
    Book book;
    int found = 0;

    printf("Enter the ID of the book to update: ");
    scanf("%d", &updateID);
    getchar();  // Consume newline character

    FILE *file = fopen(BOOK_FILE, "r");
    FILE *tempFile = fopen("temp_books.txt", "w");

    if (file == NULL || tempFile == NULL) {
        printf("Error opening file!\n");
        return;
    }

    // Search for the book with the given ID and update its details
    while (fscanf(file, "%d %[^\n] %[^\n] %[^\n]\n", &book.id, book.name, book.author, book.publishedDate) != EOF) {
        if (book.id == updateID) {
            found = 1;
            printf("Updating Book with ID %d\n", updateID);

            printf("Enter New Book Name (current: %s): ", book.name);
            fgets(book.name, sizeof(book.name), stdin);
            book.name[strcspn(book.name, "\n")] = '\0';

            printf("Enter New Author (current: %s): ", book.author);
            fgets(book.author, sizeof(book.author), stdin);
            book.author[strcspn(book.author, "\n")] = '\0';

            printf("Enter New Published Date (current: %s): ", book.publishedDate);
            fgets(book.publishedDate, sizeof(book.publishedDate), stdin);
            book.publishedDate[strcspn(book.publishedDate, "\n")] = '\0';
        }
        fprintf(tempFile, "%d\n%s\n%s\n%s\n", book.id, book.name, book.author, book.publishedDate);
    }

    fclose(file);
    fclose(tempFile);

    remove(BOOK_FILE);
    rename("temp_books.txt", BOOK_FILE);

    if (found) {
        printf("Book with ID %d successfully updated.\n\n", updateID);
    } else {
        printf("Book with ID %d not found.\n\n", updateID);
    }
}

// Function to clear the display
void clearDisplay() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Main function for the manager's book management menu
int main() {
    int choice;

    printf("\n--- Manager Book Management ---\n");

    while (1) {
        printf("\n1. Display Book List\n");
        printf("2. Search Book\n");
        printf("3. Add New Book\n");
        printf("4. Delete Book by ID\n");
        printf("5. Update Book by ID\n");
        printf("6. Students taken books history\n");
        printf("7. Clear Display\n");
        printf("8. Exit\n");
        printf("Enter your choice (1-8): ");


        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');  // Clear the invalid input
            continue;
        }
        getchar();  // Consume the newline character

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
                addBook();
                break;
            case 4:
                printf("\n");
                deleteBook();
                break;
            case 5:
                printf("\n");
                updateBookByID();
                break;
                case 6:
                printf("\n");
                displayStudentBorrowings();
                break;
            case 7:
                clearDisplay();
                printf("\n--- Manager Book Management ---\n");
                break;
            case 8:
                printf("Exiting the manager system. Goodbye!\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
