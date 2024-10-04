/*************************************** 
 * This program is used to get stop words from the Shakespeare works. 
 * At the same time, it also counts the number of occurrences of 
 * each word in each document and then gets the stop words. 
 * The output files are word_count.txt, stop_words.txt, and word_docs.txt.
 * All of them are stored in the code/data directory.
****************************************/
#include <iostream>
#include <fstream>      // for file input/output
#include <string>
#include <map>
#include <vector>
#include <set>
#include <algorithm>    // for sort()
#include <cctype>
#include "wordStem/english_stem.h"

using namespace std;

typedef pair<wstring, set<string>> Pair;
map<wstring, int> wordList;             // word -> count(The total number of times a word appears in the corpus)
map<string, int> wordNumOfDoc;          // file -> word_count(Total word count of the file)
stemming::english_stem<> StemEnglish;
map<wstring, set<string>> wordDocs;      // word -> count(The number of articles containing the word)

int main()
{
    string file;        // file name
    ifstream infile;    // input file
    ofstream outfile;   // output file

    infile.open("data/txt_title.txt");    // open the file containing the file names
    outfile.open("data/file_word_count.txt",ios::out);
    while(infile >> file)
    {
        string line;
        ifstream in;
        
        // Read in the file and stem each word
        in.open("data/shakespeare_works/"+file+".txt", ios::in);
        while(getline(in, line))
        {
            wstring word = L"";
            for(char &c : line)
            {
                // if the character is alpha or number, add it to the word. It solve the problem of the Punctuation and Whitespace
                if(isalnum(c))                
                    word += tolower(c);
                else if(word.length() > 0)
                {
                    StemEnglish(word);              //stem the word
                    wordList[word]++;
                    wordNumOfDoc[file]++;
                    wordDocs[word].insert(file);    //get the number of files by the size of the file name vector
                    word = L"";
                }
            }
        }
        outfile << file << " " << wordNumOfDoc[file] << endl;   
        in.close();
    }
    infile.close();
    
    // Sort the words by their frequency
    // PS : Map does not have a built-in sorting algorithm, so I need to convert the map into a vector and then use sort().
    vector<Pair> vec(wordDocs.begin(), wordDocs.end());
    sort(vec.begin(), vec.end(), [](const Pair& a, const Pair& b) { return a.second.size() > b.second.size(); });
    
    wofstream out,out2,out3;     // output files
    out.open("data/word_count.txt", ios::out);
    out2.open("data/stop_words.txt", ios::out);
    out3.open("data/word_docs.txt", ios::out);
    for(Pair &word : vec)
    {
        out3 << word.first << " " << word.second.size() << endl;
        // To be honest, the threshold of stop words has no scientific basis.
        if(word.second.size() >= 334)
            out2 << word.first << endl; // stop words
        out << word.first << " " << wordList[word.first] << endl;   // word count
    }
    out.close();
    out2.close();
    out3.close();
}

// To compile the program, run the following command in the terminal:
// Depends on where you are, maybe you need to change the path accordingly.
// (I assume you are in the main dictionary of the project)
// g++ -o code/scripts/getstopwords/getStopWord code/scripts/getstopwords/getStopWord.cpp -Werror -Wall -Wextra 