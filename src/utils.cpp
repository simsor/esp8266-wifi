#include <FS.h>

#include "utils.h"

bool readSpiffsFile(const char* filename, char **out)
{
    if (!SPIFFS.exists(filename)) {
        Serial.printf("File %s does not exist\n", filename);
        return false;
    }

    File f = SPIFFS.open(filename, "r");

    if (!f || !f.size()) {
        Serial.printf("An error occured when opening %s\n", filename);
        return false;
    }

    *out = (char*) malloc(sizeof(char) * f.size() + 1);
    if (*out == NULL) {
        Serial.println("utils.cpp: could not allocate buf");
        return false;
    }
    f.readBytes(*out, f.size());

    (*out)[f.size()] = '\0';

    f.close();
    return true;
}

bool writeToSpiffsFile(const char* filename, const char* toWrite)
{
    if (!SPIFFS.exists(filename)) {
        Serial.printf("File %s does not exist\n", filename);
        return false;
    }

    File f = SPIFFS.open(filename, "w");
    
    if (!f) {
        Serial.printf("An error occured when opening %s\n", filename);
        return false;
    }

    f.print(toWrite);

    f.close();
    return true;
}

bool setNextService(const char *serviceName)
{
    return writeToSpiffsFile("/service.txt", serviceName);
}

int countOccur(char *haystack, char needle)
{
    int count = 0;
    char c;
    for (int i=0; i < strlen(haystack); i++) {
        c = haystack[i];
        if (c == needle) {
            count++;
        }
    }
    return count;
}

void trim(char *str)
{
    int end = strlen(str) - 1;
    char c;
    for (int i=end; i > 0; i--) {
        c = str[i];
        if (c != '\n' && c != ' ' && c != '\r') {
            break;
        }

        str[i] = '\0';
    }
}