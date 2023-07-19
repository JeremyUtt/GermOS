#include <converts.hpp>
#include <libACPI.hpp>
#include <libSerial.hpp>

RSDPDescriptor* rsdpPtr;
ACPISDTHeader* rsdtPtr;

bool check(char str[], int address) {
    for (int i = 0; i < getStrLen(str); i++) {
        if (*(char*)(address + i) != str[i]) { return false; }
    }
    return true;
}

RSDPDescriptor* findRSDP() {
    char str[] = "RSD PTR ";
    int start_addr = 0x000E0000;
    int i = 0;
    // int finds = 0;
    while (true) {
        if (check(str, start_addr + i)) {
            rsdpPtr = (RSDPDescriptor*)(start_addr + i);
            return (RSDPDescriptor*)(start_addr + i);
        }

        i++;
    }
}

void decodeRSDP(RSDPDescriptor* rsdpPointer) {
    rsdtPtr = (ACPISDTHeader*)rsdpPointer->RsdtAddress;

    serialWriteStr("\r\nRSDP Header: ");

    serialWriteStr("\r\n\tsignature: ");
    serialWriteStr(rsdpPointer->Signature);
    serialWriteStr("\r\n\tchecksum: ");
    serialWriteStr(intToStr(rsdpPointer->Checksum, 10));
    serialWriteStr("\r\n\tOEMID: ");
    serialWriteStr(rsdpPointer->OEMID);
    serialWriteStr("\r\n\trevision: ");
    serialWriteStr(intToStr(rsdpPointer->Revision, 10));
    serialWriteStr("\r\n\tRsdtAddress: ");
    serialWriteStr(intToStr(rsdpPointer->RsdtAddress, 16));
}

void decodeRSDT(ACPISDTHeader* rsdtPointer) {
    serialWriteStr("\r\nRSDT Header: ");

    serialWriteStr("\r\n\tSignature: ");
    serialWriteStr(rsdtPointer->Signature);
    serialWriteStr("\r\n\tLength: ");
    serialWriteStr(intToStr(rsdtPointer->Length, 10));
    serialWriteStr("\r\n\tRevision: ");
    serialWriteStr(intToStr(rsdtPointer->Revision, 10));
    serialWriteStr("\r\n\tChecksum: ");
    serialWriteStr(intToStr(rsdtPointer->Checksum, 10));
    serialWriteStr("\r\n\tOEMID: ");
    serialWriteStr(rsdtPointer->OEMID);
    serialWriteStr("\r\n\tOEMTableID: ");
    serialWriteStr(rsdtPointer->OEMTableID);
    serialWriteStr("\r\n\tOEMRevision: ");
    serialWriteStr(intToStr(rsdtPointer->OEMRevision, 10));
    serialWriteStr("\r\n\tCreatorID: ");
    serialWriteStr(intToStr(rsdtPointer->CreatorID, 10));
    serialWriteStr("\r\n\tCreatorRevision: ");
    serialWriteStr(intToStr(rsdtPointer->CreatorRevision, 10));
}