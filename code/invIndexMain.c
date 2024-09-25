#include "invIndexHeader.h"
#include <stdio.h>


int main() {   // void InvertedIndex();
    BplusTree InvertedIndex;

    #ifdef _WIN32
        InvertedIndexWin(InvertedIndex);
    #else    // such as Linux, Mac OS, etc.
        InvertedIndexUnix(InvertedIndex);
    #endif

    return 0;
}