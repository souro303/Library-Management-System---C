#include <stdio.h>
#include <stdlib.h> // Required for system() function

int main() {
    int choice;

    printf("Library Management System\n");
    printf("=========================\n");

    while (1) {
        printf("1. Student\n");
        printf("2. Library Manager\n");
        printf("3. Exit\n");
        printf("Enter your choice (1, 2 or 3): ");
        scanf("%d", &choice);

        if (choice == 1) {
            system("cls");
            system("gcc 2_Student.c -o student && student.exe");
            break;
            printf("\nReturning to main menu...\n\n");
        } else if (choice == 2) {
            system("cls");
            system("gcc 3_Manager.c -o manager && manager.exe");
            break;
        } else if (choice == 3) {
            printf("Exiting the program...\n");
            break;
        } else {
            printf("Invalid choice. Please enter 1, 2, or 3.\n\n");
        }
    }

    return 0;
}
