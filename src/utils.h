#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>
#include <string.h>

//bool readSpiffsFile(const char* filename, String &out);
bool readSpiffsFile(const char* filename, char **out);
bool writeToSpiffsFile(const char* filename, const char* toWrite);

bool setNextService(const char* serviceName);

int countOccur(char *haystack, char needle);

void trim(char *str);

#endif