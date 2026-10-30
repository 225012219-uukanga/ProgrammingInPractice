/* storage.c - persistence for the MFMS.
 *
 * Text format (one record per line, '|' separates fields):
 *   employees.txt : id|name|salary
 *   suppliers.txt : id|name|phone
 *   assets.txt    : id|name|value
 *   budgets.txt   : id|name|allocated|spent
 *
 * Safe-saving pattern: data is written to "<file>.tmp" first. Only when
 * the whole write succeeded is the old file replaced, so a failed save
 * cannot destroy the previous data.
 */
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "storage.h"
#include "config.h"
#include "employees.h"
#include "suppliers.h"
#include "assets.h"
#include "budget.h"

#define EMP_TXT  DATA_DIR "/employees.txt"
#define SUP_TXT  DATA_DIR "/suppliers.txt"
#define AST_TXT  DATA_DIR "/assets.txt"
#define BUD_TXT  DATA_DIR "/budgets.txt"
#define EMP_DAT  DATA_DIR "/employees.dat"

#define PATH_MAX_LEN 128
#define LINE_LEN     256

/* ------------------------------------------------------------------ */
/* helpers                                                            */
/* ------------------------------------------------------------------ */

/* Explains why a file could not be opened. A missing file is not an
   error the first time the program runs, so it gets a friendly message. */
static void reportOpenFailure(const char *path, int err)
{
    if (err == ENOENT) {
        printf("  %s not found - nothing to load yet "
               "(or the '%s' folder is missing).\n", path, DATA_DIR);
    } else {
        errno = err;
        perror(path);
    }
}

static FILE *openTemp(const char *path, char *tmp, size_t size, const char *mode)
{
    FILE *fp;

    snprintf(tmp, size, "%s.tmp", path);
    fp = fopen(tmp, mode);
    if (fp == NULL) {
        perror(tmp);
        printf("  Make sure the '%s' folder exists.\n", DATA_DIR);
    }
    return fp;
}

/* Closes the temp file and, if everything went well, replaces the real one. */
static int commitTemp(FILE *fp, const char *tmp, const char *path, int ok)
{
    if (fclose(fp) != 0) {
        ok = 0;
    }
    if (!ok) {
        remove(tmp);
        printf("  Write to %s failed - previous file left untouched.\n", path);
        return 0;
    }
    remove(path);                 /* ignore result: file may not exist yet */
    if (rename(tmp, path) != 0) {
        perror(path);
        return 0;
    }
    return 1;
}

static const char *whyRejected(int code)
{
    if (code == REC_DUPLICATE) return "duplicate ID";
    if (code == REC_FULL)      return "list full";
    return "invalid values";
}

/* ------------------------------------------------------------------ */
/* text save                                                          */
/* ------------------------------------------------------------------ */

static void saveEmployeesText(void)
{
    char tmp[PATH_MAX_LEN];
    FILE *fp = openTemp(EMP_TXT, tmp, sizeof tmp, "w");
    int i, ok = 1, n = getEmployeeCount();

    if (fp == NULL) return;
    for (i = 0; i < n; i++) {
        const Employee *e = getEmployee(i);
        if (fprintf(fp, "%d|%s|%.2f\n", e->id, e->name, e->salary) < 0) ok = 0;
    }
    if (commitTemp(fp, tmp, EMP_TXT, ok)) printf("  Saved %d employee record(s) to %s\n", n, EMP_TXT);
}

static void saveSuppliersText(void)
{
    char tmp[PATH_MAX_LEN];
    FILE *fp = openTemp(SUP_TXT, tmp, sizeof tmp, "w");
    int i, ok = 1, n = getSupplierCount();

    if (fp == NULL) return;
    for (i = 0; i < n; i++) {
        const Supplier *s = getSupplier(i);
        if (fprintf(fp, "%d|%s|%s\n", s->id, s->name, s->phone) < 0) ok = 0;
    }
    if (commitTemp(fp, tmp, SUP_TXT, ok)) printf("  Saved %d supplier record(s) to %s\n", n, SUP_TXT);
}

static void saveAssetsText(void)
{
    char tmp[PATH_MAX_LEN];
    FILE *fp = openTemp(AST_TXT, tmp, sizeof tmp, "w");
    int i, ok = 1, n = getAssetCount();

    if (fp == NULL) return;
    for (i = 0; i < n; i++) {
        const Asset *a = getAsset(i);
        if (fprintf(fp, "%d|%s|%.2f\n", a->id, a->name, a->value) < 0) ok = 0;
    }
    if (commitTemp(fp, tmp, AST_TXT, ok)) printf("  Saved %d asset record(s) to %s\n", n, AST_TXT);
}

static void saveBudgetsText(void)
{
    char tmp[PATH_MAX_LEN];
    FILE *fp = openTemp(BUD_TXT, tmp, sizeof tmp, "w");
    int i, ok = 1, n = getBudgetCount();

    if (fp == NULL) return;
    for (i = 0; i < n; i++) {
        const Budget *b = getBudget(i);
        if (fprintf(fp, "%d|%s|%.2f|%.2f\n", b->id, b->name, b->allocated, b->spent) < 0) ok = 0;
    }
    if (commitTemp(fp, tmp, BUD_TXT, ok)) printf("  Saved %d budget record(s) to %s\n", n, BUD_TXT);
}

void saveAllText(void)
{
    printf("Saving data (text files)...\n");
    saveEmployeesText();
    saveSuppliersText();
    saveAssetsText();
    saveBudgetsText();
}

/* ------------------------------------------------------------------ */
/* text load  (fgets + sscanf: one line at a time, malformed lines    */
/* are reported and skipped instead of breaking the whole load)       */
/* ------------------------------------------------------------------ */

static void loadEmployeesText(void)
{
    FILE *fp = fopen(EMP_TXT, "r");
    char line[LINE_LEN];
    int lineNo = 0, loaded = 0, skipped = 0;

    if (fp == NULL) { reportOpenFailure(EMP_TXT, errno); return; }
    clearEmployees();
    while (fgets(line, sizeof line, fp) != NULL) {
        Employee e;
        int r;
        lineNo++;
        if (line[0] == '\n') continue;
        if (sscanf(line, "%d|%49[^|]|%lf", &e.id, e.name, &e.salary) != 3) {
            printf("  %s line %d malformed - skipped\n", EMP_TXT, lineNo);
            skipped++;
            continue;
        }
        r = addEmployeeRecord(&e);
        if (r == REC_OK) loaded++;
        else { printf("  %s line %d rejected (%s)\n", EMP_TXT, lineNo, whyRejected(r)); skipped++; }
    }
    if (ferror(fp)) perror(EMP_TXT);
    fclose(fp);
    printf("  Loaded %d employee record(s) (%d skipped)\n", loaded, skipped);
}

static void loadSuppliersText(void)
{
    FILE *fp = fopen(SUP_TXT, "r");
    char line[LINE_LEN];
    int lineNo = 0, loaded = 0, skipped = 0;

    if (fp == NULL) { reportOpenFailure(SUP_TXT, errno); return; }
    clearSuppliers();
    while (fgets(line, sizeof line, fp) != NULL) {
        Supplier s;
        int r;
        lineNo++;
        if (line[0] == '\n') continue;
        if (sscanf(line, "%d|%49[^|]|%19[^\n|]", &s.id, s.name, s.phone) != 3) {
            printf("  %s line %d malformed - skipped\n", SUP_TXT, lineNo);
            skipped++;
            continue;
        }
        r = addSupplierRecord(&s);
        if (r == REC_OK) loaded++;
        else { printf("  %s line %d rejected (%s)\n", SUP_TXT, lineNo, whyRejected(r)); skipped++; }
    }
    if (ferror(fp)) perror(SUP_TXT);
    fclose(fp);
    printf("  Loaded %d supplier record(s) (%d skipped)\n", loaded, skipped);
}

static void loadAssetsText(void)
{
    FILE *fp = fopen(AST_TXT, "r");
    char line[LINE_LEN];
    int lineNo = 0, loaded = 0, skipped = 0;

    if (fp == NULL) { reportOpenFailure(AST_TXT, errno); return; }
    clearAssets();
    while (fgets(line, sizeof line, fp) != NULL) {
        Asset a;
        int r;
        lineNo++;
        if (line[0] == '\n') continue;
        if (sscanf(line, "%d|%49[^|]|%lf", &a.id, a.name, &a.value) != 3) {
            printf("  %s line %d malformed - skipped\n", AST_TXT, lineNo);
            skipped++;
            continue;
        }
        r = addAssetRecord(&a);
        if (r == REC_OK) loaded++;
        else { printf("  %s line %d rejected (%s)\n", AST_TXT, lineNo, whyRejected(r)); skipped++; }
    }
    if (ferror(fp)) perror(AST_TXT);
    fclose(fp);
    printf("  Loaded %d asset record(s) (%d skipped)\n", loaded, skipped);
}

static void loadBudgetsText(void)
{
    FILE *fp = fopen(BUD_TXT, "r");
    char line[LINE_LEN];
    int lineNo = 0, loaded = 0, skipped = 0;

    if (fp == NULL) { reportOpenFailure(BUD_TXT, errno); return; }
    clearBudgets();
    while (fgets(line, sizeof line, fp) != NULL) {
        Budget b;
        int r;
        lineNo++;
        if (line[0] == '\n') continue;
        if (sscanf(line, "%d|%49[^|]|%lf|%lf", &b.id, b.name, &b.allocated, &b.spent) != 4) {
            printf("  %s line %d malformed - skipped\n", BUD_TXT, lineNo);
            skipped++;
            continue;
        }
        r = addBudgetRecord(&b);
        if (r == REC_OK) loaded++;
        else { printf("  %s line %d rejected (%s)\n", BUD_TXT, lineNo, whyRejected(r)); skipped++; }
    }
    if (ferror(fp)) perror(BUD_TXT);
    fclose(fp);
    printf("  Loaded %d budget record(s) (%d skipped)\n", loaded, skipped);
}

void loadAllText(void)
{
    printf("Loading data (text files)...\n");
    loadEmployeesText();
    loadSuppliersText();
    loadAssetsText();
    loadBudgetsText();
}

/* ------------------------------------------------------------------ */
/* binary employees (fwrite / fread)                                  */
/* ------------------------------------------------------------------ */

void saveEmployeesBinary(void)
{
    char tmp[PATH_MAX_LEN];
    FILE *fp = openTemp(EMP_DAT, tmp, sizeof tmp, "wb");
    int i, ok = 1, n = getEmployeeCount();

    if (fp == NULL) return;
    for (i = 0; i < n; i++) {
        const Employee *e = getEmployee(i);
        if (fwrite(e, sizeof *e, 1, fp) != 1) {
            perror("Error writing employee");
            ok = 0;
            break;
        }
    }
    if (commitTemp(fp, tmp, EMP_DAT, ok)) printf("Saved %d employee record(s) to %s (binary)\n", n, EMP_DAT);
}

void loadEmployeesBinary(void)
{
    FILE *fp = fopen(EMP_DAT, "rb");
    Employee e;
    int loaded = 0, skipped = 0;

    if (fp == NULL) { reportOpenFailure(EMP_DAT, errno); return; }
    clearEmployees();
    while (fread(&e, sizeof e, 1, fp) == 1) {
        int r;
        e.name[NAME_LEN - 1] = '\0';          /* never trust stored text */
        r = addEmployeeRecord(&e);
        if (r == REC_OK) loaded++;
        else skipped++;
    }
    if (ferror(fp)) perror(EMP_DAT);
    fclose(fp);
    printf("Loaded %d employee record(s) from %s (%d skipped)\n", loaded, EMP_DAT, skipped);
}

/* ------------------------------------------------------------------ */
/* backup and comparison                                              */
/* ------------------------------------------------------------------ */

/* Copies src to src + ".bak". Returns 1 on success, 0 if src is missing
   or anything fails. */
static int backupFile(const char *src)
{
    char dst[PATH_MAX_LEN];
    char buffer[1024];
    size_t got;
    int ok = 1;
    FILE *in = fopen(src, "rb");
    FILE *out;

    if (in == NULL) {
        printf("  %s: not found - skipped\n", src);
        return 0;
    }
    snprintf(dst, sizeof dst, "%s.bak", src);
    out = fopen(dst, "wb");
    if (out == NULL) {
        perror(dst);
        fclose(in);
        return 0;
    }
    while ((got = fread(buffer, 1, sizeof buffer, in)) > 0) {
        if (fwrite(buffer, 1, got, out) != got) { ok = 0; break; }
    }
    if (ferror(in)) ok = 0;
    fclose(in);
    if (fclose(out) != 0) ok = 0;
    if (ok) printf("  %s -> %s\n", src, dst);
    else    printf("  Backup of %s failed\n", src);
    return ok;
}

void backupData(void)
{
    int done = 0;

    printf("Backing up data files...\n");
    done += backupFile(EMP_TXT);
    done += backupFile(SUP_TXT);
    done += backupFile(AST_TXT);
    done += backupFile(BUD_TXT);
    done += backupFile(EMP_DAT);
    printf("Backup finished: %d file(s) copied.\n", done);
}

/* Size of a file in bytes, or -1 if it cannot be opened. */
static long fileSize(const char *path)
{
    long size;
    FILE *fp = fopen(path, "rb");

    if (fp == NULL) return -1;
    if (fseek(fp, 0, SEEK_END) != 0) { fclose(fp); return -1; }
    size = ftell(fp);
    fclose(fp);
    return size;
}

void compareEmployeeFiles(void)
{
    long t = fileSize(EMP_TXT);
    long b = fileSize(EMP_DAT);

    printf("\nText vs binary (employees)\n");
    if (t < 0) printf("  %-22s : not saved yet\n", EMP_TXT);
    else       printf("  %-22s : %ld bytes (human readable)\n", EMP_TXT, t);
    if (b < 0) printf("  %-22s : not saved yet\n", EMP_DAT);
    else       printf("  %-22s : %ld bytes (%lu bytes per record, not readable)\n",
                      EMP_DAT, b, (unsigned long)sizeof(Employee));
    if (t >= 0 && b >= 0)
        printf("  Binary records have a fixed size (name array is always %d bytes);\n"
               "  text size depends on how long the values are.\n", NAME_LEN);
}