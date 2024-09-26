#include "invIndexHeader.h"
#include <stdio.h>

// BplusTree InvertedIndex(string pathName);

BplusTree InvertedIndex(string pathName) {   
    BplusTree InvIndex;

    InvIndex = CreateBP();

    #ifdef _WIN32
        InvertedIndexWin(InvIndex, pathName);
    #else    // such as Linux, Mac OS, etc.
        InvertedIndexUnix(InvIndex, pathName);
    #endif

    return InvIndex;
}