/* main.c - coordinates the application: menu and flow only */
#include <stdio.h>
#include "config.h"
#include "utilities.h"
#include "employees.h"
#include "budget.h"
#include "reports.h"

static void displayMainMenu(void)
{
    printf("\n==== %s ====\n", APP_NAME);
    printf(" 1. Add employee\n");
    printf(" 2. List employees\n");
    printf(" 3. Search employee\n");
    printf(" 4. Add budget\n");
    printf(" 5. List budgets\n");
    printf(" 6. Employee report\n");
    printf(" 7. Budget report\n");
    printf(" 0. Exit\n");
}

int main(void)
{
    int choice;

    do {
        displayMainMenu();
        choice = readInt("Choose an option: ");

        switch (choice) {
        case 1: addEmployee();     break;
        case 2: listEmployees();   break;
        case 3: searchEmployee();  break;
        case 4: addBudget();       break;
        case 5: listBudgets();     break;
        case 6: employeeReport();  break;
        case 7: budgetReport();    break;
        case 0: printf("Goodbye.\n"); break;
        default: printf("Invalid option.\n");
        }
        if (choice != 0) {
            pauseScreen();
        }
    } while (choice != 0);

    return 0;
}