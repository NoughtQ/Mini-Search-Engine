/****************************************  
 * This is the main function of the search engine.
 * It first loads the inverted index and the necessary resources.
 * Then it enters a loop to repeatedly search for a query until the user chooses to exit.
****************************************/
#include <iostream>
#include "invIndexHeader.h"

int main()
{
    //load necessary resources and inverted index
    std::cout << "Please wait for resources to load and inverted index to be established..."<<std::endl;
    loadWordIdf(WORDDOCSPATH);              //load word-idf dictionary
    loadFileWordsNum(FILEWORDCOUNTPATH);    //load file-word count dictionary
    loadStopWords(STOPWORDPATH);            //load stop words list
    BplusTree InvIndex = CreateBP();        //create B+ tree for inverted index
    InvIndex = fileTraversaler(InvIndex, (char *)SHAKSPEAREDIR, (char *)"", false, true);
    std::cout << "Inverted Index has been established successfully!"<<std::endl;
    std::cout << "--------------------------------------------------"<< std::endl;

    //set default values for pageSize and threshold
    std::cout << "\033[1;32mWelcome to ***ShakespeareFinder***\033[0m" << std::endl;
    std::cout << "\033[34mPlease set the maximum number of records to be displayed per query\033[0m" << std::endl;
    int pageSize = 10;          //default value
    std::cout << "Size(int):";
    std::cin >> pageSize;
    std::cout << "\033[34mPlease set the threshold (0-1) of the query:\033[0m" << std::endl;
    std::cout << "Threshold(double):";
    double threshold = 0.8;     //default value
    std::cin >> threshold;
    getchar();                  //clear the buffer to avoid input error

    //start the search loop
    Searchbegin:
    std::cout << "Please enter your query:" << std::endl;
    std::string query;
    /* Read query from user and analyse it*/
    std::getline(std::cin,query);
    query += " ";                 // add a space at the end of the query to avoid errors
    
    search(query, InvIndex, pageSize, threshold);
    //ask user if they want to search again
    std::cout << "Do you want to search again?\033[1;31m(y/n)\033[0m: ";
    char choice;
    std::cin >> choice;
    if(choice == 'y' || choice == 'Y')
    {
        getchar();
        goto Searchbegin;
    }
    //exit the program
    else
    {
        std::cout << "\033[1;32mThank you for using ShakespeareFinder!" << std::endl << "Have a nice day~\033[0m" << std::endl;
        return 0;
    }
}