/* suppliers.c - supplier operations */
#include <stdio.h>
#include "suppliers.h"
#include "employees.h"   /* REC_* return codes */
#include "utilities.h"

static Supplier suppliers[MAX_RECORDS];
static int supplierCount = 0;

static int findSupplier(int id)
{
    int i;
    for (i = 0; i < supplierCount; i++) {
        if (suppliers[i].id == id) {
            return i;
        }
    }
    return -1;
}

int addSupplierRecord(const Supplier *s)
{
    if (s->id <= 0 || s->name[0] == '\0' || s->phone[0] == '\0') {
        return REC_INVALID;
    }
    if (supplierCount >= MAX_RECORDS) {
        return REC_FULL;
    }
    if (findSupplier(s->id) != -1) {
        return REC_DUPLICATE;
    }
    suppliers[supplierCount++] = *s;
    return REC_OK;
}

void clearSuppliers(void)
{
    supplierCount = 0;
}

void addSupplier(void)
{
    Supplier s;
    int result;

    s.id = readInt("Supplier ID: ");
    readString("Supplier name: ", s.name, sizeof s.name);
    readString("Phone: ", s.phone, sizeof s.phone);

    result = addSupplierRecord(&s);
    if (result == REC_OK)             printf("Supplier '%s' added.\n", s.name);
    else if (result == REC_DUPLICATE) printf("A supplier with ID %d already exists.\n", s.id);
    else if (result == REC_FULL)      printf("Supplier list is full.\n");
    else                              printf("Invalid data (ID > 0, name and phone required).\n");
}

void listSuppliers(void)
{
    int i;

    if (supplierCount == 0) {
        printf("No suppliers recorded.\n");
        return;
    }
    printf("\n%-6s %-25s %-20s\n", "ID", "Name", "Phone");
    for (i = 0; i < supplierCount; i++) {
        printf("%-6d %-25s %-20s\n",
               suppliers[i].id, suppliers[i].name, suppliers[i].phone);
    }
}

int getSupplierCount(void)
{
    return supplierCount;
}

const Supplier *getSupplier(int index)
{
    if (index < 0 || index >= supplierCount) {
        return NULL;
    }
    return &suppliers[index];
}