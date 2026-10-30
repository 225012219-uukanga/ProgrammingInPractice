/* employees.h - employee operations */
#ifndef EMPLOYEES_H
#define EMPLOYEES_H

#include "config.h"

typedef struct {
    int    id;
    char   name[NAME_LEN];
    double salary;
} Employee;

void            addEmployee(void);
void            listEmployees(void);
void            searchEmployee(void);
double          calculatePayrollTotal(void);

/* read-only access for other modules (e.g. reports) */
int             getEmployeeCount(void);
const Employee *getEmployee(int index);

#endif