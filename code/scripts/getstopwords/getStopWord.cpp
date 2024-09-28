#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include <cctype>
#include "../wordStem/english_stem.h"

using namespace std;

typedef pair<wstring, set<string>> Pair;
map<wstring, int> wordList;
map<string, int> wordNumOfDoc;
stemming::english_stem<> StemEnglish;
map<wstring, set<string>> wordDocs;

int main()
{
    string file;
    ifstream infile;
    ofstream outfile;

    infile.open("code/source/txt_title.txt");
    outfile.open("code/source/file_word_count.txt",ios::out);
    while(infile >> file)
    {
        string line;
        ifstream in;
        
        in.open("code/source/shakespeare_works/"+file+".txt", ios::in);
        while(getline(in, line))
        {
            wstring word = L"";
            for(char &c : line)
            {
                if(isalnum(c))
                    word += tolower(c);
                else if(word.length() > 0)
                {
                    StemEnglish(word);
                    if(wordList.find(word) == wordList.end())
                        wordList[word] = 1;
                    else
                        wordList[word]++;
                    wordNumOfDoc[file]++;
                    wordDocs[word].insert(file);
                    word = L"";
                }
            }
        }
        outfile << file << " " << wordNumOfDoc[file] << endl;
        in.close();
    }
    infile.close();
    
    vector<Pair> vec(wordDocs.begin(), wordDocs.end());
    wofstream out,out2,out3;
    sort(vec.begin(), vec.end(), [](const Pair& a, const Pair& b) { return a.second.size() > b.second.size(); });
    out.open("code/source/word_count.txt", ios::out);
    out2.open("code/source/stop_words.txt", ios::out);
    out3.open("code/source/word_docs.txt", ios::out);
    for(Pair &word : vec)
    {
        out3 << word.first << " " << word.second.size() << endl;
        if(word.second.size() >= 334)
            out2 << word.first << endl;
        out << word.first << " " << wordList[word.first] << endl;
    }
    out.close();
    out2.close();
    out3.close();
}

// g++ -o code/build/getStopWord code/scripts/getstopwords/getStopWord.cpp -Werror -Wall -Wextra