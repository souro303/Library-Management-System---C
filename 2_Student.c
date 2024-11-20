#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define STUDENT_FILE "students.txt"
#define TEMP_USER_FILE "temp_user.txt"

// Check if a username already exists
int isUsernameTaken(const char *username) {
    char fileUsername[50];
    FILE *file = fopen(STUDENT_FILE, "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 0;
    }

    while (fgets(fileUsername, sizeof(fileUsername), file)) {
        // Remove trailing newline character
        fileUsername[strcspn(fileUsername, "\n")] = '\0';

        // Compare the username
        if (strcmp(username, fileUsername) == 0) {
            fclose(file);
            return 1; // Username is taken
        }

        // Skip the rest of the user record (4 more lines and a separator, '---')
        for (int i = 0; i < 5; i++) {
            if (fgets(fileUsername, sizeof(fileUsername), file) == NULL) break;
        }
    }

    fclose(file);
    return 0; // Username is not taken
}


// Register a new user
void registerUser() {
    char username[50], password[50], name[100], studentID[20], department[50];
    FILE *file;

    printf("Register a New Student\n");
    printf("----------------------\n");

    while (1) {
        printf("Enter a new Username: ");
        scanf("%s", username);
        if (!isUsernameTaken(username)) break;
        printf("Username already exists! Please choose a different one.\n");
    }

    printf("Enter Full Name: ");
    getchar();  // Clear newline character from buffer
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';

    printf("Enter Student ID: ");
    scanf("%s", studentID);

    printf("Enter Department: ");
    getchar();  // Clear newline character from buffer
    fgets(department, sizeof(department), stdin);
    department[strcspn(department, "\n")] = '\0';

    printf("Enter a new Password: ");
    scanf("%s", password);

    file = fopen(STUDENT_FILE, "a");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }
    fprintf(file, "%s\n%s\n%s\n%s\n%s\n---\n", username, name, studentID, department, password);
    fclose(file);

    printf("Registration successful! You will now be redirected to login.\n\n");
}

// Login function
int login() {
    char inputUsername[50], inputPassword[50];
    char fileUsername[50], filePassword[50];
    char name[100], studentID[20], department[50];
    FILE *file = fopen(STUDENT_FILE, "r");

    if (file == NULL) {
        printf("No registered students found. Please register first.\n");
        return 0;
    }

    printf("Student Login\n");
    printf("-------------\n");

    printf("Enter Username: ");
    scanf("%s", inputUsername);
    printf("Enter Password: ");
    scanf("%s", inputPassword);

    // Reading and verifying user credentials
    while (fscanf(file, "%s\n%[^\n]\n%s\n%[^\n]\n%s\n---\n", fileUsername, name, studentID, department, filePassword) != EOF) {
        if (strcmp(inputUsername, fileUsername) == 0 && strcmp(inputPassword, filePassword) == 0) {
            fclose(file);

            // Save username to a temporary file for `2_StudentLogin.c`
            FILE *tempFile = fopen(TEMP_USER_FILE, "w");
            if (tempFile != NULL) {
                fprintf(tempFile, "%s\n", inputUsername);
                fclose(tempFile);
            }

            system("cls");

            printf("\nWelcome, %s!\n", name);
            printf("Student ID: %s\n", studentID);
            printf("Department: %s\n", department);
            return 1;  // Successful login
        }
    }

    fclose(file);
    printf("Incorrect username or password.\n");
    return 0;  // Login failed
}

int main() {
    int choice;

    printf("---Student Management System---\n");
    printf("\n");

    while (1) {
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Dashboard\n");
        printf("4. Exit\n");
        printf("Enter your choice (1, 2, 3 or 4): ");
        scanf("%d", &choice);

        if (choice == 1) {
            system("cls");
            registerUser();
            system("cls");
            if (login()) {
                printf("\nLogin successful! Redirecting to student login...\n");
                system("gcc 2_StudentLogin.c -o studentlogin && studentlogin.exe");
                break;
            }
        } else if (choice == 2) {
            system("cls");
            if (login()) {
                printf("\nLogin successful! Redirecting to student login...\n");
                system("gcc 2_StudentLogin.c -o studentlogin && studentlogin.exe");
                break;
            }
        } else if (choice == 3) {
            system("cls");
            system("gcc 1_DashBoard.c -o dashboard && dashboard.exe");
            break;
        } else if (choice == 4) {
            printf("Exiting the program.\n");
            break;
        } else {
            printf("Invalid choice. Please enter 1, 2, 3, or 4.\n\n");
        }
    }

    return 0;
}
