/* storage.h - saving and loading MFMS data (text and binary files) */
#ifndef STORAGE_H
#define STORAGE_H

/* Text files: data/employees.txt, suppliers.txt, assets.txt, budgets.txt */
void saveAllText(void);
void loadAllText(void);

/* Binary file for employees: data/employees.dat */
void saveEmployeesBinary(void);
void loadEmployeesBinary(void);

/* Extras */
void backupData(void);              /* copies each data file to <name>.bak */
void compareEmployeeFiles(void);    /* text vs binary file size            */

#endif