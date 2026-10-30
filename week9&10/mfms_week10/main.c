/* main.c - coordinates the application: menu and flow only */
#include <stdio.h>
#include "config.h"
#include "utilities.h"
#include "employees.h"
#include "suppliers.h"
#include "assets.h"
#include "budget.h"
#include "reports.h"
#include "storage.h"

static void displayMainMenu(void)
{
    printf("\n==== %s ====\n", APP_NAME);
    printf("  1. Add employee         2. List employees      3. Search employee\n");
    printf("  4. Add supplier         5. List suppliers\n");
    printf("  6. Add asset            7. List assets\n");
    printf("  8. Add budget           9. List budgets\n");
    printf(" 10. Employee report     11. Budget report\n");
    printf(" 12. Save Data (text)    13. Load Data (text)\n");
    printf(" 14. Save employees (binary)   15. Load employees (binary)\n");
    printf(" 16. Backup Data         17. Compare text vs binary file\n");
    printf("  0. Exit\n");
}

int main(void)
{
    int choice;

    do {
        displayMainMenu();
        choice = readInt("Choose an option: ");

        switch (choice) {
        case 1:  addEmployee();            break;
        case 2:  listEmployees();          break;
        case 3:  searchEmployee();         break;
        case 4:  addSupplier();            break;
        case 5:  listSuppliers();          break;
        case 6:  addAsset();               break;
        case 7:  listAssets();             break;
        case 8:  addBudget();              break;
        case 9:  listBudgets();            break;
        case 10: employeeReport();         break;
        case 11: budgetReport();           break;
        case 12: saveAllText();            break;
        case 13: loadAllText();            break;
        case 14: saveEmployeesBinary();    break;
        case 15: loadEmployeesBinary();    break;
        case 16: backupData();             break;
        case 17: compareEmployeeFiles();   break;
        case 0:  printf("Goodbye. (Unsaved data is lost - use Save Data first!)\n"); break;
        default: printf("Invalid option.\n");
        }
        if (choice != 0) {
            pauseScreen();
        }
    } while (choice != 0);

    return 0;
}