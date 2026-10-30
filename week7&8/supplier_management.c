#include <stdio.h>
#include <string.h>

int main() {
    char supplierName[100];
    char email[100];
    char phone[30];
    char town[50];
    char backupName[100];
    char description[200];
    char searchName[100];

    int choice;
    int hasSupplier = 0; // tracks whether a supplier has been added yet

    do {
        printf("\n================================\n");
        printf(" MUNICIPAL FINANCIAL MANAGEMENT\n");
        printf("================================\n");
        printf("1. Add Supplier\n");
        printf("2. Display Supplier\n");
        printf("3. Search Supplier\n");
        printf("4. Show Name Length\n");
        printf("5. Copy Supplier Name (strcpy demo)\n");
        printf("6. Build Supplier Description (strcat demo)\n");
        printf("7. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar(); // clears the leftover newline left by scanf

        switch (choice) {
            case 1:
                printf("Enter supplier name: ");
                fgets(supplierName, sizeof(supplierName), stdin);
                supplierName[strcspn(supplierName, "\n")] = '\0';

                printf("Enter email: ");
                fgets(email, sizeof(email), stdin);
                email[strcspn(email, "\n")] = '\0';

                printf("Enter phone: ");
                fgets(phone, sizeof(phone), stdin);
                phone[strcspn(phone, "\n")] = '\0';

                printf("Enter town: ");
                fgets(town, sizeof(town), stdin);
                town[strcspn(town, "\n")] = '\0';

                hasSupplier = 1;
                printf("\nSupplier added successfully.\n");
                break;

            case 2:
                if (!hasSupplier) {
                    printf("\nNo supplier has been added yet.\n");
                } else {
                    printf("\n--- SUPPLIER DETAILS ---\n");
                    printf("Name : %s\n", supplierName);
                    printf("Email: %s\n", email);
                    printf("Phone: %s\n", phone);
                    printf("Town : %s\n", town);
                }
                break;

            case 3:
                if (!hasSupplier) {
                    printf("\nNo supplier has been added yet.\n");
                } else {
                    printf("Enter supplier name to search: ");
                    fgets(searchName, sizeof(searchName), stdin);
                    searchName[strcspn(searchName, "\n")] = '\0';

                    if (strcmp(supplierName, searchName) == 0) {
                        printf("Supplier found.\n");
                    } else {
                        printf("Supplier not found.\n");
                    }
                }
                break;

            case 4:
                if (!hasSupplier) {
                    printf("\nNo supplier has been added yet.\n");
                } else {
                    printf("\nSupplier name length: %zu\n", strlen(supplierName));
                    printf("Email length: %zu\n", strlen(email));
                    printf("Town length: %zu\n", strlen(town));
                }
                break;

            case 5:
                if (!hasSupplier) {
                    printf("\nNo supplier has been added yet.\n");
                } else {
                    strcpy(backupName, supplierName);
                    printf("\nOriginal name: %s\n", supplierName);
                    printf("Backup copy  : %s\n", backupName);
                }
                break;

            case 6:
                if (!hasSupplier) {
                    printf("\nNo supplier has been added yet.\n");
                } else {
                    strcpy(description, supplierName);
                    strcat(description, " operates in ");
                    strcat(description, town);
                    strcat(description, ".");
                    printf("\n%s\n", description);
                }
                break;

            case 7:
                printf("\nGoodbye.\n");
                break;

            default:
                printf("\nInvalid choice. Please try again.\n");
        }

    } while (choice != 7);

    return 0;
}