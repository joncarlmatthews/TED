#include <windows.h>
#include "memory.h"

void *allocateMemory(LPVOID memoryStartAddress, uint64 memorySizeInBytes)
{
    void *mem = VirtualAlloc(memoryStartAddress,
                        memorySizeInBytes,
                        MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    if(mem){
        SecureZeroMemory(mem, memorySizeInBytes);
    }

    return mem;
}

void freeMemory(void *address)
{
    VirtualFree(address, 0, MEM_RELEASE);
}