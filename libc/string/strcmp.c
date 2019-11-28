#include <stdio.h>
#include <string.h>
int strcmp(const char* str1, const char* str2) {
    int retVal = 0;
    const char *crStr1 = str1, *crStr2 = str2;

    while (1) {
        if (*crStr1 > *crStr2) {
            retVal = 1;
            break;
        } else if (*crStr1 < *crStr2) {
            retVal = -1;
            break;
        } else if (*crStr1 == 0) {
            retVal = 0;
            break;
        }
        crStr1++;
        crStr2++;
    }

    return retVal;
}