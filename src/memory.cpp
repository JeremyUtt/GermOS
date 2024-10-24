// All of this could be much more optimized with hashtables
// oh well... womp womp
#include "memory.hpp"
#define HEAP_SIZE 32768
#define TABLE_SIZE 1000
#include <error.hpp>
#include <libSerial.hpp>
#include <utils.hpp>

struct allocEntry {
    bool realEntry;
    bool inUse;
    uint8_t* start;
    uint8_t* end;
};

uint8_t heap[HEAP_SIZE] = {0};
allocEntry table[TABLE_SIZE];

void initMem() {
    for (uint16_t i = 1; i < TABLE_SIZE; i++) {
        table[i].realEntry = false;
        table[i].inUse = false;
        table[i].start = nullptr;
        table[i].end = nullptr;
    }

    table[0].realEntry = true;
    table[0].inUse = false;
    table[0].start = (uint8_t*)&heap;
    table[0].end = (uint8_t*)&heap + HEAP_SIZE;
}

void* malloc(uint16_t bytes) {
    // printMem();
    for (uint16_t i = 0; i < TABLE_SIZE; i++) {
        uint16_t size = table[i].end - table[i].start;
        if (table[i].realEntry && !table[i].inUse && size >= bytes) {
            // if size already correct, dont shrink, just return
            if (size == bytes) {
                table[i].inUse = true;
                return (void*)table[i].start;
            }
            // else, split entry into t parts and return first;

            // find empty table slot for new free segment
            bool innerFound = false;
            for (uint16_t j = 0; j < TABLE_SIZE; j++) {
                if (!table[j].realEntry) {
                    // Define new entry
                    table[j].realEntry = true;
                    table[j].start = table[i].start + bytes + 1;
                    table[j].end = table[i].end;
                    innerFound = true;
                    break;
                }
            }

            if (!innerFound) {
                error("Failed to allocate memory", true);
            }

            table[i].end = table[i].start + bytes;
            table[i].inUse = true;
            return (void*)table[i].start;
        }
    }

    // this part will only run if the above loop doesn't return early
    // --> could not find any memory

    error("Failed to allocate memory", true);
    return nullptr;  // Wont ever reach here
}

void mergeEntries(allocEntry& a) {
    // int count = 0;
    for (uint16_t j = 0; j < TABLE_SIZE; j++) {
        if (table[j].end + 1 == a.start) {
            if (table[j].realEntry && !table[j].inUse) {
                a.end = table[j].end;

                // clean up old entries
                table[j].inUse = false;
                table[j].realEntry = false;
                table[j].start = nullptr;
                table[j].end = nullptr;
            }
            break;
        }
    }

    for (uint16_t j = 0; j < TABLE_SIZE; j++) {
        if (table[j].start - 1 == a.end) {
            if (table[j].realEntry && !table[j].inUse) {
                a.end = table[j].end;
                // clean up old entries
                table[j].inUse = false;
                table[j].realEntry = false;
                table[j].start = nullptr;
                table[j].end = nullptr;
            }
            break;
        }
    }
}

void free(void* mem) {
    for (uint16_t i = 0; i < TABLE_SIZE; i++) {
        if (table[i].inUse && table[i].start == (uint8_t*)mem) {
            table[i].inUse = false;

            // clear memory
            for (uint16_t j = 0; j < table[i].end - table[i].start; j++) {
                table[i].start[j] = 0;
            }

            mergeEntries(table[i]);
            return;
        }
    }
}

void printMem() {
    for (uint16_t i = 0; i < TABLE_SIZE; i++) {
        if (table[i].realEntry) {
            if (table[i].inUse) {
                // printf("Entry: Start: %d End: %d, in use\r\n", table[i].start, table[i].end);
            } else {
                // printf("Entry: Start: %d End: %d, free\r\n", table[i].start, table[i].end);
            }
        }
    }
}