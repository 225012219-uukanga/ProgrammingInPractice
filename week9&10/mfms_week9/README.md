# MFMS - Week 9: Multi-File Programming (PAP521S)

Municipal Financial Management System, split into modules.
Each module has one clear responsibility. The `.h` files are the public interface and the `.c` files hold the code.

## Modules

| Module | Responsibility | Public functions |
|---|---|---|
| `main.c` | Menu and application flow only | `main()` |
| `employees.c / .h` | Employee records | `addEmployee()`, `listEmployees()`, `searchEmployee()`, `calculatePayrollTotal()`, `getEmployeeCount()`, `getEmployee()` |
| `budget.c / .h` | Budget records | `addBudget()`, `listBudgets()`, `calculateBudgetBalance()`, `getBudgetCount()`, `getBudget()` |
| `utilities.c / .h` | Safe input helpers | `readInt()`, `readDouble()`, `readString()`, `pauseScreen()` |
| `reports.c / .h` | Reports, built from the employees and budget modules | `employeeReport()`, `budgetReport()` |
| `config.h` | Shared constants (`MAX_RECORDS`, `NAME_LEN`, `APP_NAME`) | none |

Every header has an include guard (`#ifndef` / `#define` / `#endif`) so it cannot be included twice.

## Build (separate compilation, then link)

```
gcc -std=c99 -Wall -Wextra -pedantic -c main.c
gcc -std=c99 -Wall -Wextra -pedantic -c employees.c
gcc -std=c99 -Wall -Wextra -pedantic -c budget.c
gcc -std=c99 -Wall -Wextra -pedantic -c utilities.c
gcc -std=c99 -Wall -Wextra -pedantic -c reports.c
gcc main.o employees.o budget.o utilities.o reports.o -o mfms
.\mfms
```

## Module dependencies

```
main.c      -> config.h, utilities.h, employees.h, budget.h, reports.h
employees.c -> employees.h (-> config.h), utilities.h
budget.c    -> budget.h (-> config.h), utilities.h
utilities.c -> utilities.h
reports.c   -> reports.h, employees.h, budget.h
```

## Linker error experiment

Leaving `reports.o` out of the link command:

```
gcc main.o employees.o budget.o utilities.o -o mfms_broken
```

gives:

```
undefined reference to `employeeReport'
undefined reference to `budgetReport'
```

Why it happens: `main.c` compiled without problems because `reports.h` promised
that these functions exist (their prototypes). The linker then looks for the real
machine code of those functions. That code is in `reports.o`, which was not given
to it, so linking fails.

## Test runs

| Run | What it tests |
|---|---|
| 1 | Add 2 employees, list them, search by ID |
| 2 | Add 2 budgets, list them, budget report (TOTAL balance 599999.25) |
| 3 | Invalid number, duplicate ID, ID not found, invalid menu option, employee report |

All three runs passed. Screenshots are in the `test_runs` folder.