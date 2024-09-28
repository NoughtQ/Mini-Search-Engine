#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <cctype>
#include "wordStem/english_stem.h"
#include "invIndexHeader.h"

std::unordered_map<std::wstring, int> wordFreq;
std::unordered_map<std::wstring, int> stopWords;
std::vector<std::pair<std::wstring,int>> queryWord;
stemming::english_stem<> StemEnglish;

void loadWordFreq(std::string filePath)
{
    std::wifstream infile;
    std::wstring word;
    int freq;
    infile.open(filePath,std::ios::in);
    if(!infile.is_open())
    {
        std::cout << "Error: unable to open file " << filePath << std::endl;
        return;
    }
    while(infile >> word >> freq)
        wordFreq[word] = freq;
    std::cout << "Word frequency loaded successfully." << std::endl;
}

void search(BplusTree T)
{
    std::cout << "Please enter your query:" << std::endl;
    std::string query;
    std::vector<std::pair<int, int>> docList;

    // Read query from user
    std::getline(std::cin,query);
    query += " ";
    std::wstring word=L"";
    queryWord.clear();
    for(char &c : query)
    {
        if(isalnum(c))
            word += tolower(c);
        else if (word.length() >= 0)
        {
            std::wstring tmp = word;
            StemEnglish(word);
            auto it = wordFreq.find(word);
            if(it == wordFreq.end())
                std::wcout << L"\033[31mWarning: \033[0m" << tmp << L" is not in the inverted index and will be ignored." << std::endl;
            else
                queryWord.push_back({word,wordFreq[word]});
            //std::wcout << L"test Word: " << tmp << L" Stem: " << word << std::endl;
            word = L"";
        }
    }
    sort(queryWord.begin(),queryWord.end(),[](const std::pair<std::wstring,int>& a, const std::pair<std::wstring,int>& b){return a.second < b.second;});
    
    // Search for documents that contain all the query words
    bool isFound;
    char * wordForSearch;
    if(queryWord.size() == 0)
    {
        std::cout << "Ops, your query is in StopList or empty, so there are no documents retrieved. " << std::endl;
        return;
    }
    else if(queryWord.size() == 1)
    {
        std::vector<std::pair<int,int>> posVec;
        std::cout << "Your query has only one valid word, so we will search for it in the inverted index." << std::endl;
        std::cout << "The word was found in files below:"<< std::endl;
        for(auto &p : queryWord)
        {
            wordForSearch = wstringToChararr(p.first);
            posVec = FindBP2(wordForSearch, -1, T);
            sort(posVec.begin(),posVec.end(),[](const std::pair<int,int>& a, const std::pair<int,int>& b){return a.second > b.second;});
            for(auto &pos : posVec)
                std::cout << docNames[pos.first] << " " << pos.second << std::endl;
        }
    }
    else
    {
        std::cout << "Your query has multiple valid words, so we will search for them in the inverted index." << std::endl;
        std::cout << "The words were found in files below:"<< std::endl;
        int cnt = 0;
        std::unordered_map<int, int> freqMap;//ID,freq
        for(auto &p : queryWord)
        {
            wordForSearch = wstringToChararr(p.first);
            auto currentPosVec = FindBP2(wordForSearch, -1, T);
            std::unordered_map<int, bool> currentDocIdMap;
            for(auto &pos : currentPosVec)
            {
                currentDocIdMap[pos.first] = true;
                freqMap[pos.first] += pos.second;
            }
            if(freqMap.size()>0)
            {
                for (auto it = freqMap.begin(); it != freqMap.end(); )//remove "it++" to remove duplicates
                {
                    if (currentDocIdMap.find(it->first) == currentDocIdMap.end()) 
                        it = freqMap.erase(it);//safe deletion
                    else
                        ++it;
                }
            }
        }
        if(freqMap.size() == 0)
        {
            std::cout << "Ops, your query is not in the inverted index, so there are no documents retrieved. " << std::endl;
            std::cout << "Please try again with different query." << std::endl;
            return;
        }
        else
        {
            std::vector<std::pair<int, int>> sortedFreqVec(freqMap.begin(), freqMap.end());
            std::sort(sortedFreqVec.begin(), sortedFreqVec.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {return a.second > b.second; });
            for(auto &p : sortedFreqVec)
            {
                std::cout << docNames[p.first] << std::endl;
                cnt++;
            }

        }
    }
}

std::vector<std::pair<int,int>> FindBP2(string term, int docCnt, BplusTree T) 
{
    int i;

    if (!T) 
        return std::vector<std::pair<int, int>>(); 
    else if (!T->childrenSize) 
        return isSameTerm2(term, docCnt, T);

    int pos = -1;
    
    for (i = 0; i < T->size; i++) {
        if (strcmp(term, T->data[i]->term) < 0) {
            pos = i;
            break;
        }
    }
    if (pos == -1) {
        pos = i;
    }

    return FindBP2(term, docCnt, T->children[pos]);
}

std::vector<std::pair<int,int>> isSameTerm2(string term, int docCnt, NodeBP nodebp) 
{
    int i;
    std::vector<std::pair<int,int>> posVec;

    if (nodebp->size) 
    {
        for (i = 0; i < nodebp->size; i++) {
            if (!strcmp(term, nodebp->data[i]->term)) 
            {
                PosList poslist = nodebp->data[i]->poslist;
                int size = poslist->size;
                int cnt = 0;
                posVec = RetrievePL2(poslist);
                break;
            }
        }
    }
    return posVec;
}


std::vector<std::pair<int,int>> RetrievePL2(PosList L) 
{
    if (!L->size) {
        printf("Empty position-data queue!\n");
        exit(1);
    }

    int i = 0, j;
    
    std::vector<std::pair<int,int>> posVec;

    PosData cur = L->front->next;

    while (cur != NULL) {
        posVec.push_back({cur->pos,cur->time});
        cur = cur->next;
        i++;
    }

    return posVec;
}
