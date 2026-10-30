/* assets.c - municipal asset operations */
#include <stdio.h>
#include "assets.h"
#include "employees.h"   /* REC_* return codes */
#include "utilities.h"

static Asset assets[MAX_RECORDS];
static int assetCount = 0;

static int findAsset(int id)
{
    int i;
    for (i = 0; i < assetCount; i++) {
        if (assets[i].id == id) {
            return i;
        }
    }
    return -1;
}

int addAssetRecord(const Asset *a)
{
    if (a->id <= 0 || a->name[0] == '\0' || a->value < 0) {
        return REC_INVALID;
    }
    if (assetCount >= MAX_RECORDS) {
        return REC_FULL;
    }
    if (findAsset(a->id) != -1) {
        return REC_DUPLICATE;
    }
    assets[assetCount++] = *a;
    return REC_OK;
}

void clearAssets(void)
{
    assetCount = 0;
}

void addAsset(void)
{
    Asset a;
    int result;

    a.id = readInt("Asset ID: ");
    readString("Asset name (e.g. Grader): ", a.name, sizeof a.name);
    a.value = readDouble("Value: ");

    result = addAssetRecord(&a);
    if (result == REC_OK)             printf("Asset '%s' added.\n", a.name);
    else if (result == REC_DUPLICATE) printf("An asset with ID %d already exists.\n", a.id);
    else if (result == REC_FULL)      printf("Asset list is full.\n");
    else                              printf("Invalid data (ID > 0, name required, value >= 0).\n");
}

void listAssets(void)
{
    int i;

    if (assetCount == 0) {
        printf("No assets recorded.\n");
        return;
    }
    printf("\n%-6s %-25s %14s\n", "ID", "Name", "Value");
    for (i = 0; i < assetCount; i++) {
        printf("%-6d %-25s %14.2f\n", assets[i].id, assets[i].name, assets[i].value);
    }
}

int getAssetCount(void)
{
    return assetCount;
}

const Asset *getAsset(int index)
{
    if (index < 0 || index >= assetCount) {
        return NULL;
    }
    return &assets[index];
}