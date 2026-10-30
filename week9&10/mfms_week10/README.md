# MFMS - Week 10: File Handling (PAP521S)

Extends the Week 9 modular MFMS so that data survives program termination.
Employees, suppliers, assets and budgets are saved to and loaded from files in the `data/` folder.

## Modules

| Module | Role |
|---|---|
| `main.c` | Menu and application flow only (options 1-17) |
| `storage.c / .h` | New. All saving, loading, backup and comparison code |
| `suppliers.c / .h` | New. Supplier records (id, name, phone) |
| `assets.c / .h` | New. Asset records (id, name, value) |
| `employees.c / .h` | Extended with `addEmployeeRecord()` and `clearEmployees()` |
| `budget.c / .h` | Extended with `addBudgetRecord()` and `clearBudgets()` |
| `utilities.c / .h` | Unchanged from Week 9 |
| `reports.c / .h` | Unchanged from Week 9 |
| `config.h` | Shared constants, now including `DATA_DIR` and `PHONE_LEN` |

## Build

```
gcc -std=c99 -Wall -Wextra -pedantic -c main.c
gcc -std=c99 -Wall -Wextra -pedantic -c employees.c
gcc -std=c99 -Wall -Wextra -pedantic -c budget.c
gcc -std=c99 -Wall -Wextra -pedantic -c suppliers.c
gcc -std=c99 -Wall -Wextra -pedantic -c assets.c
gcc -std=c99 -Wall -Wextra -pedantic -c utilities.c
gcc -std=c99 -Wall -Wextra -pedantic -c reports.c
gcc -std=c99 -Wall -Wextra -pedantic -c storage.c
gcc main.o employees.o budget.o suppliers.o assets.o utilities.o reports.o storage.o -o mfms
.\mfms
```

Run it from inside the `mfms_week10` folder. The program looks for the `data` folder next to where it is started.

## Menu

```
 1 Add employee      2 List employees     3 Search employee
 4 Add supplier      5 List suppliers
 6 Add asset         7 List assets
 8 Add budget        9 List budgets
10 Employee report  11 Budget report
12 Save Data (text) 13 Load Data (text)
14 Save employees (binary)  15 Load employees (binary)
16 Backup Data      17 Compare text vs binary file
 0 Exit
```

## Storage design

Text files hold one record per line, with `|` between the fields.

| File | Format |
|---|---|
| `data/employees.txt` | `id\|name\|salary` |
| `data/suppliers.txt` | `id\|name\|phone` |
| `data/assets.txt` | `id\|name\|value` |
| `data/budgets.txt` | `id\|name\|allocated\|spent` |
| `data/employees.dat` | binary: raw `Employee` records written with `fwrite()` |

If a name contains the `|` separator, it would break the format. `readString()` replaces any `|` that is typed with `-`.
Names with spaces work because the loader reads up to the next `|` (`%49[^|]`) instead of using `%s`.

## File safety

- Every `fopen()` is checked for `NULL`, and `perror()` or a clear message explains the failure.
- Saving never opens the real file with `"w"` directly. Data is written to a `.tmp` file first, and the old file is replaced only if the whole write succeeded. A failed save cannot destroy existing records.
- Loading reads one line at a time (`fgets` + `sscanf`) and checks the return value. Malformed lines are reported and skipped.
- Loading uses `add...Record()`, so duplicate IDs, negative values and empty names are rejected.
- A missing file is treated as "nothing saved yet", not as a crash.
- Every save and load prints how many records were saved, loaded or skipped.
- Loading replaces the in-memory list, so loading twice does not create duplicates.
- `fread()` and `fwrite()` return values are checked, and files are always closed.

## Text vs binary

Measured with menu option 17 (2 employees):

| | `employees.txt` | `employees.dat` |
|---|---|---|
| Size | 51 bytes | 128 bytes (2 x 64) |
| Readable in an editor | Yes | No |
| Record size | Varies with the data | Fixed: `sizeof(Employee)` = 64 bytes |
| Parsing | Needed (`sscanf`, separators, bad lines) | None, `fread()` fills the record directly |
| Portability | Good | Depends on type sizes, padding and byte order |
| Best use | Reports, exports, debugging | Fast application storage |

The binary file is larger here because the name array always takes 50 bytes, even for short names.
The text file is 51 bytes on Windows because each line ends with two characters (`\r\n`).

## Test runs

| Run | What it shows | Screenshot |
|---|---|---|
| 1 | Enter records for all 4 types, then Save Data | `test_runs/run1.png` |
| 2 | Restart: list is empty, then Load Data brings the records back; binary save/load; size comparison; backup | `test_runs/run2.png` |
| 3 | Damaged `employees.txt`: a malformed line, a duplicate ID and a negative salary are skipped | `test_runs/run3.png` |
| 4 | Missing data files and a missing `data` folder | `test_runs/run4a.png`, `test_runs/run4b.png` |

## Practical challenge items covered

Backup option (16), search employee (3), record counts after loading, handling files that do not exist, confirmation messages, and tests with missing files, malformed records and duplicate IDs.