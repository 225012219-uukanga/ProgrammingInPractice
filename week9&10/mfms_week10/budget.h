/* budget.h - budget operations */
#ifndef BUDGET_H
#define BUDGET_H

#include "config.h"

typedef struct {
    int    id;
    char   name[NAME_LEN];
    double allocated;
    double spent;
} Budget;

void          addBudget(void);
void          listBudgets(void);
double        calculateBudgetBalance(void);

int           addBudgetRecord(const Budget *b);   /* returns REC_* (see employees.h) */
void          clearBudgets(void);
int           getBudgetCount(void);
const Budget *getBudget(int index);

#endif