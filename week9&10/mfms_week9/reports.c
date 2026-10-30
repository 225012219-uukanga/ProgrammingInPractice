/* reports.c - reports built from the employees and budget modules */
#include <stdio.h>
#include "reports.h"
#include "employees.h"
#include "budget.h"

void employeeReport(void)
{
    int i;
    int count = getEmployeeCount();
    double total = calculatePayrollTotal();

    printf("\n===== EMPLOYEE REPORT =====\n");
    if (count == 0) {
        printf("No employees recorded.\n");
        return;
    }
    for (i = 0; i < count; i++) {
        const Employee *e = getEmployee(i);
        printf("%-6d %-25s %12.2f\n", e->id, e->name, e->salary);
    }
    printf("---------------------------------------------\n");
    printf("Employees      : %d\n", count);
    printf("Total payroll  : %.2f\n", total);
    printf("Average salary : %.2f\n", total / count);
}

void budgetReport(void)
{
    int i;
    int count = getBudgetCount();
    double allocated = 0.0, spent = 0.0;

    printf("\n===== BUDGET REPORT =====\n");
    if (count == 0) {
        printf("No budgets recorded.\n");
        return;
    }
    printf("%-6s %-20s %12s %12s %12s\n",
           "ID", "Name", "Allocated", "Spent", "Balance");
    for (i = 0; i < count; i++) {
        const Budget *b = getBudget(i);
        printf("%-6d %-20s %12.2f %12.2f %12.2f\n",
               b->id, b->name, b->allocated, b->spent,
               b->allocated - b->spent);
        allocated += b->allocated;
        spent     += b->spent;
    }
    printf("---------------------------------------------------------------\n");
    printf("%-27s %12.2f %12.2f %12.2f\n", "TOTAL", allocated, spent,
           calculateBudgetBalance());
}