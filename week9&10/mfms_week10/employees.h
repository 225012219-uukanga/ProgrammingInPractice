/* employees.h - employee operations */
#ifndef EMPLOYEES_H
#define EMPLOYEES_H

#include "config.h"

typedef struct {
    int    id;
    char   name[NAME_LEN];
    double salary;
} Employee;

/* return codes of the add...Record() functions */
#define REC_OK         0
#define REC_FULL      -1
#define REC_DUPLICATE -2
#define REC_INVALID   -3

void            addEmployee(void);
void            listEmployees(void);
void            searchEmployee(void);
double          calculatePayrollTotal(void);

int             addEmployeeRecord(const Employee *e);  /* used by load */
void            clearEmployees(void);
int             getEmployeeCount(void);
const Employee *getEmployee(int index);

#endif