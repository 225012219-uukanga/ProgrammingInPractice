/* budget.c - budget operations */
#include <stdio.h>
#include "budget.h"
#include "utilities.h"

static Budget budgets[MAX_RECORDS];
static int budgetCount = 0;

static int findBudget(int id)
{
    int i;
    for (i = 0; i < budgetCount; i++) {
        if (budgets[i].id == id) {
            return i;
        }
    }
    return -1;
}

void addBudget(void)
{
    Budget b;

    if (budgetCount >= MAX_RECORDS) {
        printf("Budget list is full.\n");
        return;
    }
    b.id = readInt("Budget ID: ");
    if (b.id <= 0) {
        printf("ID must be a positive number.\n");
        return;
    }
    if (findBudget(b.id) != -1) {
        printf("A budget with ID %d already exists.\n", b.id);
        return;
    }
    readString("Budget name (e.g. Roads): ", b.name, sizeof b.name);
    if (b.name[0] == '\0') {
        printf("Name cannot be empty.\n");
        return;
    }
    b.allocated = readDouble("Amount allocated: ");
    b.spent     = readDouble("Amount spent so far: ");
    if (b.allocated < 0 || b.spent < 0) {
        printf("Amounts cannot be negative.\n");
        return;
    }
    budgets[budgetCount++] = b;
    printf("Budget '%s' added.\n", b.name);
}

void listBudgets(void)
{
    int i;

    if (budgetCount == 0) {
        printf("No budgets recorded.\n");
        return;
    }
    printf("\n%-6s %-20s %12s %12s\n", "ID", "Name", "Allocated", "Spent");
    for (i = 0; i < budgetCount; i++) {
        printf("%-6d %-20s %12.2f %12.2f\n",
               budgets[i].id, budgets[i].name,
               budgets[i].allocated, budgets[i].spent);
    }
}

double calculateBudgetBalance(void)
{
    double balance = 0.0;
    int i;
    for (i = 0; i < budgetCount; i++) {
        balance += budgets[i].allocated - budgets[i].spent;
    }
    return balance;
}

int getBudgetCount(void)
{
    return budgetCount;
}

const Budget *getBudget(int index)
{
    if (index < 0 || index >= budgetCount) {
        return NULL;
    }
    return &budgets[index];
}