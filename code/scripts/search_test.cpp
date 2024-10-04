#include <iostream>
#include <fstream>          // for file input/output
#include <string>           
#include <time.h>            // for clock()
#include "invIndexHeader.h"

const std::string root_path = "data/search_test/input";

int main()
{
    clock_t start, end;

    //load inverted index and other necessary resources
    std::cout << "Please wait for resources to load and inverted index to be established..."<<std::endl;
    loadWordIdf(WORDDOCSPATH);              //load word-idf dictionary
    loadFileWordsNum(FILEWORDCOUNTPATH);    //load file-word count dictionary
    loadStopWords(STOPWORDPATH);            //load stop words list
    BplusTree InvIndex = CreateBP();        //create B+ tree for inverted index
    InvIndex = fileTraversaler(InvIndex, (char *)SHAKESPEAREDIR, (char *)"", false, true);
    std::cout << "Inverted Index has been established successfully!"<<std::endl;
    std::cout << "--------------------------------------------------" << std::endl;
    
    std::cout << "Welcome to ShakespeareFinder's test program!" << std::endl;
    std::cout << "The program tests the performance of the search engine in different situations" << std::endl;
    
    //test loop begin
    testbegin:
    std::cout << "Please enter the serial number of the input you want to test: " << std::endl;
    int serial_num;          //serial number of input file
    std::cin >> serial_num;
    std::string input_file_path = root_path + std::to_string(serial_num);    //input file path
    std::ifstream input_file;                           //input file object
    input_file.open(input_file_path, std::ios::in);     //open input file
    // if input file is not found, return
    if (!input_file.is_open())
        std::cout << "Invalid input serial number!" << std::endl;
    // if input file is found, continue
    else
    {
        //read input parameters
        int pageSize, k;
        double threshold;
        std::cout << "Input file opened successfully!" << std::endl;
        std::cout << "Please enter the pageSize, threshold and the times of cycles : " << std::endl;
        std::cin >> pageSize >> threshold >> k;
        std::string query;
        std::getline(input_file, query);
        query += " ";
        
        //test performance
        start = clock();
        for(int i=0; i<k; i++)
        {
            std::cout << "Cycle " << i+1 << " starts..." << std::endl;  //print cycle number
            search(query, InvIndex , pageSize, threshold);
        }
        end = clock();
        
        PrintTime(start,end);
    }
    
    //ask user if they want to test another input
    std::cout << "Do you want to test another input? (y/n)" << std::endl;
    char choice;
    std::cin >> choice;
    if(choice == 'y')
    {
        getchar();
        goto testbegin;         //jump to testbegin
    }    
    else
        return 0;

}