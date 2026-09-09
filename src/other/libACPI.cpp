#include <libACPI.hpp>
#include <libSerial.hpp>
#include <utils.hpp>
ACPI::RSDP* rsdpPtr;
ACPI::SDT_header* rsdtPtr;

bool check(char str[], int address) {
    for (uint32_t i = 0; i < getStrLen(str); i++) {
        if (*(char*)(address + i) != str[i]) {
            return false;
        }
    }
    return true;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waddress-of-packed-member"
ACPI::RSDP* findRSDP() {
    char str[] = "RSD PTR ";
    int startAddr = 0x000E0000;
    int i = 0;
    // int finds = 0;
    while (true) {
        if (check(str, startAddr + i)) {
            rsdpPtr = (ACPI::RSDP*)(startAddr + i);

            rsdtPtr = &rsdpPtr->rsdtAddress->header;

            return rsdpPtr;
        }

        i++;
    }
}
#pragma GCC diagnostic pop

void decodeRSDP(ACPI::RSDP* rsdpPointer) {
    serialWriteStr("\r\nRSDP Header: ");

    serialWriteStr("\r\n\tsignature: ");
    serialWriteStr(string(rsdpPointer->signature, 8));
    serialWriteStr("\r\n\tchecksum: ");
    serialWriteStr(intToStr(rsdpPointer->checksum, 10));
    serialWriteStr("\r\n\tOEMID: ");
    serialWriteStr(rsdpPointer->oemId);
    serialWriteStr("\r\n\trevision: ");
    serialWriteStr(intToStr(rsdpPointer->revision, 10));
    serialWriteStr("\r\n\tRsdtAddress: ");
    serialWriteStr(intToStr((int)rsdpPointer->rsdtAddress, 16));
}

void printSDT(ACPI::SDT_header* rsdtPointer) {
    serialWriteStr("\r\nRSDT Header: ");

    serialWriteStr("\r\n\tSignature: ");
    serialWriteStr(string(rsdtPointer->signature, 4));
    serialWriteStr("\r\n\tLength: ");
    serialWriteStr(intToStr(rsdtPointer->length, 10));
    serialWriteStr("\r\n\tRevision: ");
    serialWriteStr(intToStr(rsdtPointer->revision, 10));
    serialWriteStr("\r\n\tChecksum: ");
    serialWriteStr(intToStr(rsdtPointer->checksum, 10));
    serialWriteStr("\r\n\tOEMID: ");
    serialWriteStr(rsdtPointer->oemId);
    serialWriteStr("\r\n\tOEMTableID: ");
    serialWriteStr(rsdtPointer->oemTableID);
    serialWriteStr("\r\n\tOEMRevision: ");
    serialWriteStr(intToStr(rsdtPointer->oemRevision, 10));
    serialWriteStr("\r\n\tCreatorID: ");
    serialWriteStr(intToStr(rsdtPointer->creatorID, 10));
    serialWriteStr("\r\n\tCreatorRevision: ");
    serialWriteStr(intToStr(rsdtPointer->creatorRevision, 10));
}

#include <printf.hpp>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waddress-of-packed-member"
void decode() {
    // Root pointer to Root System Descriptor table
    printf("Finding RDSP\n");
    ACPI::RSDP* rsdp = findRSDP();
    decodeRSDP(rsdp);

    // Root System Descriptor Table
    ACPI::RSDT_full* rsdt = rsdp->rsdtAddress;
    printSDT(&rsdt->header);

    size_t size = (rsdt->header.length - sizeof(rsdt->header)) / 4;

    for (size_t i = 0; i < size; i++) {
        printSDT(rsdt->tables[i]);
    }
}
#pragma GCC diagnostic pop
