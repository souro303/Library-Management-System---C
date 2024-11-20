#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MANAGES_FILE "manages.txt"

void login() {
    char username[100], password[100], storedUsername[100], storedPassword[100];
    FILE *file = fopen(MANAGES_FILE, "r");
    int found = 0;

    if (file == NULL) {
        printf("Error: Unable to open the manages file.\n");
        return;
    }

    printf("Manager Login\n");
    printf("-------------\n");

    printf("Enter username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';  // Remove the newline character

    printf("Enter password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';  // Remove the newline character

    // Check each line of the file for a matching username and password
    while (fscanf(file, "%s %s", storedUsername, storedPassword) != EOF) {
        if (strcmp(username, storedUsername) == 0 && strcmp(password, storedPassword) == 0) {
            found = 1;
            break;
        }
    }

    fclose(file);

    if (found) {
        printf("Login successful! Welcome, %s.\n", username);
    } else {
        printf("Invalid username or password.\n\n");
        login();
    }
}

int main() {
    int choice;

    while (1) {
        printf("--- Manager Management System ---\n\n");
        printf("1. Login\n");
        printf("2. Dashboard\n");
        printf("3. Exit\n");
        printf("Enter your choice (1-3): ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            // Clear the invalid input
            while (getchar() != '\n');
            continue;
        }

        getchar(); // Consume the newline character left by scanf

        if (choice == 1) {
            system("cls");
            login();
            system("cls");
            system("gcc 3_ManagerLogin.c -o managerlogin && managerlogin.exe");
            break;
        }else if (choice == 2) {
            system("cls");
            system("gcc 1_DashBoard.c -o dashboard && dashboard.exe");
            break;
        } else if (choice == 3) {
            printf("Exiting the system. Goodbye!\n");
            break;
        } else {
            printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
