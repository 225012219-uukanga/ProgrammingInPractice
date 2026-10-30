/* employees.c - employee operations (Week 10: adds record API for storage) */
#include <stdio.h>
#include "employees.h"
#include "utilities.h"

static Employee employees[MAX_RECORDS];
static int employeeCount = 0;

static int findEmployee(int id)
{
    int i;
    for (i = 0; i < employeeCount; i++) {
        if (employees[i].id == id) {
            return i;
        }
    }
    return -1;
}

/* Validates and stores one record. Returns a REC_* code. */
int addEmployeeRecord(const Employee *e)
{
    if (e->id <= 0 || e->name[0] == '\0' || e->salary < 0) {
        return REC_INVALID;
    }
    if (employeeCount >= MAX_RECORDS) {
        return REC_FULL;
    }
    if (findEmployee(e->id) != -1) {
        return REC_DUPLICATE;
    }
    employees[employeeCount++] = *e;
    return REC_OK;
}

void clearEmployees(void)
{
    employeeCount = 0;
}

void addEmployee(void)
{
    Employee e;
    int result;

    e.id = readInt("Employee ID: ");
    readString("Name: ", e.name, sizeof e.name);
    e.salary = readDouble("Salary: ");

    result = addEmployeeRecord(&e);
    if (result == REC_OK)             printf("Employee '%s' added.\n", e.name);
    else if (result == REC_DUPLICATE) printf("An employee with ID %d already exists.\n", e.id);
    else if (result == REC_FULL)      printf("Employee list is full.\n");
    else                              printf("Invalid data (ID > 0, name required, salary >= 0).\n");
}

void listEmployees(void)
{
    int i;

    if (employeeCount == 0) {
        printf("No employees recorded.\n");
        return;
    }
    printf("\n%-6s %-25s %12s\n", "ID", "Name", "Salary");
    for (i = 0; i < employeeCount; i++) {
        printf("%-6d %-25s %12.2f\n",
               employees[i].id, employees[i].name, employees[i].salary);
    }
}

void searchEmployee(void)
{
    int id = readInt("Enter employee ID to search: ");
    int idx = findEmployee(id);

    if (idx == -1) {
        printf("No employee with ID %d.\n", id);
    } else {
        printf("Found: %d | %s | %.2f\n",
               employees[idx].id, employees[idx].name, employees[idx].salary);
    }
}

double calculatePayrollTotal(void)
{
    double total = 0.0;
    int i;
    for (i = 0; i < employeeCount; i++) {
        total += employees[i].salary;
    }
    return total;
}

int getEmployeeCount(void)
{
    return employeeCount;
}

const Employee *getEmployee(int index)
{
    if (index < 0 || index >= employeeCount) {
        return NULL;
    }
    return &employees[index];
}