/* utilities.h - reusable input helpers */
#ifndef UTILITIES_H
#define UTILITIES_H

#include <stddef.h>

int    readInt(const char *prompt);
double readDouble(const char *prompt);
void   readString(const char *prompt, char *buffer, size_t size);
void   pauseScreen(void);

#endif