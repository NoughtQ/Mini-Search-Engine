#include <iostream>
#include <cstdlib>
#include "invIndexHeader.h"

int main()
{
    std::cout << "Please wait for resources to load and inverted index to be established..."<<std::endl;
    loadWordIdf(WORDDOCSPATH);
    loadFileWordsNum(FILEWORDCOUNTPATH);
    BplusTree InvIndex = CreateBP();
    InvIndex = fileTraversaler(InvIndex, (char *)SHAKSPEAREDIR, (char *)"", false, true);
    std::cout << "Inverted Index has been established successfully!"<<std::endl;
    std::cout << "--------------------------------------------------"<< std::endl;
    //system("clear");
    std::cout << "\033[1mWelcome to ***ShakespeareFinder***\033[0m" << std::endl;
    std::cout << "Please set the size of page:" << std::endl;
    int pageSize;
    std::cin >> pageSize;
    std::cout << "Please set the threshold(0-1) of the query:" << std::endl;
    double threshold;
    std::cin >> threshold;
    getchar();

    Searchbegin:
    search(InvIndex, pageSize, threshold);
    std::cout << "Do you want to search again? (y/n): ";
    char choice;
    std::cin >> choice;
    if(choice == 'y' || choice == 'Y')
    {
        getchar();
        goto Searchbegin;
    }
    else
    {
        std::cout << "\033[1mThank you for using ShakespeareFinder!" << std::endl << "Have a nice day!\033[0m" << std::endl;
        return 0;
    }
}