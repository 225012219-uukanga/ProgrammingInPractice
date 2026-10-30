/* assets.h - municipal asset operations */
#ifndef ASSETS_H
#define ASSETS_H

#include "config.h"

typedef struct {
    int    id;
    char   name[NAME_LEN];
    double value;
} Asset;

void         addAsset(void);
void         listAssets(void);

int          addAssetRecord(const Asset *a);   /* returns REC_* */
void         clearAssets(void);
int          getAssetCount(void);
const Asset *getAsset(int index);

#endif