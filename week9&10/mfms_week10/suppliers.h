/* suppliers.h - supplier operations */
#ifndef SUPPLIERS_H
#define SUPPLIERS_H

#include "config.h"

typedef struct {
    int  id;
    char name[NAME_LEN];
    char phone[PHONE_LEN];
} Supplier;

void            addSupplier(void);
void            listSuppliers(void);

int             addSupplierRecord(const Supplier *s);   /* returns REC_* */
void            clearSuppliers(void);
int             getSupplierCount(void);
const Supplier *getSupplier(int index);

#endif