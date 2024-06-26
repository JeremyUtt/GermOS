#include <utils.hpp>
char strBuf[330];
string intToStr(int value, int base) {
    char* rc;
    char* ptr;
    char* low;
    // Check for supported base.
    if (base < 2 || base > 36) {
        *strBuf = '\0';
        return strBuf;
    }
    rc = ptr = strBuf;
    // Set '-' for negative decimals.
    if (value < 0 && base == 10) {
        *ptr++ = '-';
    }
    // Remember where the numbers start.
    low = ptr;
    // The actual conversion.
    do {
        // Modulo is negative for negative value. This trick makes abs()
        // unnecessary.
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnop"
                 "qrstuvwxyz"[35 + value % base];
        value /= base;
    } while (value);
    // Terminating the string.
    *ptr-- = '\0';
    // Invert the numbers.
    while (low < ptr) {
        char tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }

    return string(rc);
}

uint32_t strToInt(string value) {
    uint32_t result = 0;
    for (uint32_t i = 0; i < value.size(); i++) {
        result += (value[i] - '0') * pow(10, value.size() - i - 1);
    }
    return result;
}

uint32_t getStrLen(char String[]) {
    int strLen = 0;
    for (int i = 0; i < 1; i++) {
        if (String[strLen] != 0) {
            i--;
            strLen++;
        }
    }
    return strLen;
}

uint32_t getStrLen(const char String[]) {
    int strLen = 0;
    for (int i = 0; i < 1; i++) {
        if (String[strLen] != 0) {
            i--;
            strLen++;
        }
    }
    return strLen;
}

bool strcmp(char str1[], char str2[]) {
    int len1 = getStrLen(str1);
    int len2 = getStrLen(str2);

    if (len1 != len2) {
        return false;
    }

    for (int i = 0; i < len1; i++) {
        if (str1[i] != str2[i]) {
            return false;
        }
    }
    return true;
}

uint32_t pow(uint32_t a, uint32_t b) {
    uint32_t result = 1;
    for (uint32_t i = 0; i < b; i++) {
        result *= a;
    }
    return result;
}