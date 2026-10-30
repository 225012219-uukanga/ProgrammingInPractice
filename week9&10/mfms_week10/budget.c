/* budget.c - budget operations (Week 10: adds record API for storage) */
#include <stdio.h>
#include "budget.h"
#include "employees.h"   /* REC_* return codes */
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

int addBudgetRecord(const Budget *b)
{
    if (b->id <= 0 || b->name[0] == '\0' || b->allocated < 0 || b->spent < 0) {
        return REC_INVALID;
    }
    if (budgetCount >= MAX_RECORDS) {
        return REC_FULL;
    }
    if (findBudget(b->id) != -1) {
        return REC_DUPLICATE;
    }
    budgets[budgetCount++] = *b;
    return REC_OK;
}

void clearBudgets(void)
{
    budgetCount = 0;
}

void addBudget(void)
{
    Budget b;
    int result;

    b.id = readInt("Budget ID: ");
    readString("Budget name (e.g. Roads): ", b.name, sizeof b.name);
    b.allocated = readDouble("Amount allocated: ");
    b.spent     = readDouble("Amount spent so far: ");

    result = addBudgetRecord(&b);
    if (result == REC_OK)             printf("Budget '%s' added.\n", b.name);
    else if (result == REC_DUPLICATE) printf("A budget with ID %d already exists.\n", b.id);
    else if (result == REC_FULL)      printf("Budget list is full.\n");
    else                              printf("Invalid data (ID > 0, name required, amounts >= 0).\n");
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