#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cctype>
#include "../wordStem/english_stem.h"
#include "invIndexHeader.h"

using namespace std;

unordered_map<wstring, int> wordFreq;
unordered_map<wstring, int> stopWords;
vector<pair<wstring,int>> queryWord;
stemming::english_stem<> StemEnglish;

void loadWordFreq(std::string filePath)
{
    wifstream infile;
    wstring word;
    int freq;
    infile.open(filePath,ios::in);
    while(!infile.eof())
    {
        infile >> word >> freq;
        wordFreq[word] = freq; 
    }
}

void loadStopWords(std::string filePath)
{
    wifstream infile;
    wstring word;
    infile.open(filePath,ios::in);
    while(!infile.eof())
    {
        infile >> word;
        stopWords[word] = 1;
    }
}

void search(BplusTree T)
{
    cout << "Please enter your query:" << endl;
    std::string query;
    while(getline(cin,query))
    {
        wstring word=L"";
        for(char &c : query)
        {
            if(isalnum(c))
                word += tolower(c);
            else if (word.length() >= 0)
            {
                wstring tmp = word;
                StemEnglish(word);
                auto it = stopWords.find(word);
                if(it == stopWords.end())
                {
                    wcout << L"Warning:" << tmp << L" is a stop word and will be ignored." << endl;
                    word = L"";
                }
                else
                {
                    auto it = wordFreq.find(word);
                    if(it == wordFreq.end())
                        wcout << L"Warning:" << tmp << L" is not in the inverted index and will be ignored." << endl;
                    else
                        queryWord.push_back({word,wordFreq[word]});
                    word = L"";
                }
            }
        }
    }
    sort(queryWord.begin(),queryWord.end(),[](const pair<wstring,int>& a, const pair<wstring,int>& b){return a.second < b.second;});
    
    bool isFound;
    char * wordForSearch;
    for(auto &p : queryWord)
    {
        wordForSearch = wstringToChararr(p.first);
        FindBP(wordForSearch, -1, T, &isFound, true);
    }
}