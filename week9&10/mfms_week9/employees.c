/* employees.c - employee operations */
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

void addEmployee(void)
{
    Employee e;

    if (employeeCount >= MAX_RECORDS) {
        printf("Employee list is full.\n");
        return;
    }
    e.id = readInt("Employee ID: ");
    if (e.id <= 0) {
        printf("ID must be a positive number.\n");
        return;
    }
    if (findEmployee(e.id) != -1) {
        printf("An employee with ID %d already exists.\n", e.id);
        return;
    }
    readString("Name: ", e.name, sizeof e.name);
    if (e.name[0] == '\0') {
        printf("Name cannot be empty.\n");
        return;
    }
    e.salary = readDouble("Salary: ");
    if (e.salary < 0) {
        printf("Salary cannot be negative.\n");
        return;
    }
    employees[employeeCount++] = e;
    printf("Employee '%s' added.\n", e.name);
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