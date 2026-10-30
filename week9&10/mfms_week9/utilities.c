/* utilities.c - reusable input helpers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilities.h"

/* Reads one line safely. Exits cleanly if input is closed (EOF). */
static void readLine(char *buffer, size_t size)
{
    size_t len;
    int ch;

    if (fgets(buffer, (int)size, stdin) == NULL) {
        printf("\nInput closed. Exiting.\n");
        exit(EXIT_SUCCESS);
    }
    len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    } else {
        /* line was longer than the buffer: throw away the rest */
        while ((ch = getchar()) != '\n' && ch != EOF) {
        }
    }
}

int readInt(const char *prompt)
{
    char line[64];
    char *end;
    long value;

    for (;;) {
        printf("%s", prompt);
        readLine(line, sizeof line);
        value = strtol(line, &end, 10);
        if (end != line && *end == '\0') {
            return (int)value;
        }
        printf("Invalid number. Please try again.\n");
    }
}

double readDouble(const char *prompt)
{
    char line[64];
    char *end;
    double value;

    for (;;) {
        printf("%s", prompt);
        readLine(line, sizeof line);
        value = strtod(line, &end);
        if (end != line && *end == '\0') {
            return value;
        }
        printf("Invalid amount. Please try again.\n");
    }
}

/* Reads text. The '|' character is replaced because later (Week 10)
   it is used as the field separator in the text data files. */
void readString(const char *prompt, char *buffer, size_t size)
{
    size_t i;

    printf("%s", prompt);
    readLine(buffer, size);
    for (i = 0; buffer[i] != '\0'; i++) {
        if (buffer[i] == '|') {
            buffer[i] = '-';
        }
    }
}

void pauseScreen(void)
{
    char dummy[8];

    printf("\nPress Enter to continue...");
    readLine(dummy, sizeof dummy);
}