#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include "invIndexHeader.h"

using namespace std;

int main()
{
    cout << "Please wait for resources to load and inverted index to be established...";
    loadWordFreq("code/source/word_count.txt");
    loadStopWords("code/source/stop_words.txt");
    BplusTree InvIndex = CreateBP();
    InvIndex = fileTraversaler(InvIndex, "code/source/shakespeare_works", "", false);
    cout << "--------------------------------------------------"<< endl;
    system("clear");
    cout << "Welcome to ShakespeareFinder" << endl;

    Searchbegin:
    search(InvIndex);
    cout << "Do you want to search again? (y/n): ";
    char choice;
    cin >> choice;
    if(choice == 'y' || choice == 'Y')
    {
        goto Searchbegin;
    }
    else
    {
        cout << "Thank you for using ShakespeareFinder!" << endl;
        return 0;
    }
    
}