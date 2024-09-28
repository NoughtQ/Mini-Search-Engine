#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <string.h>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <cctype>
#include <cmath>
#include "wordStem/english_stem.h"
#include "invIndexHeader.h"

#define DOCTOTALNUM 1115

std::unordered_map<std::wstring, double> wordIdf;
std::unordered_map<int, int> fileWordsNum;
std::vector<std::pair<std::wstring,double>> queryWord;
stemming::english_stem<> StemEnglish;

void loadWordIdf(std::string filePath)
{
    std::wifstream infile;
    std::wstring word;
    int freq;
    double idf;
    infile.open(filePath,std::ios::in);
    if(!infile.is_open())
    {
        std::cout << "Error: unable to open file " << filePath << std::endl;
        return;
    }
    while(infile >> word >> freq)
    {
        idf = log(1115.0/(double)(1+freq));
        wordIdf[word] = idf;
    }
    std::cout << "Word Idf loaded successfully." << std::endl;
}

void loadFileWordsNum(std::string filePath)
{
    std::wifstream infile;
    std::wstring filename;
    int num;
    int cnt = 0;
    infile.open(filePath,std::ios::in);
    if(!infile.is_open())
    {
        std::cout << "Error: unable to open file " << filePath << std::endl;
        return;
    }
    while(infile >> filename >> num)
        fileWordsNum[cnt++] = num;
    std::cout << "The word count of every file loaded successfully." << std::endl;
}

void search(BplusTree T)
{
    std::cout << "Please enter your query:" << std::endl;
    std::string query;
    std::vector<std::pair<int,double>> docList;//docId,tf-idf

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
            auto it = wordIdf.find(word);
            if(it == wordIdf.end())
                std::wcout << L"\033[31mWarning: \033[0m" << tmp << L" is not in the inverted index and will be ignored." << std::endl;
            else
                queryWord.push_back({word,wordIdf[word]});
            //std::wcout << L"test Word: " << tmp << L" Stem: " << word << std::endl;
            word = L"";
        }
    }
    sort(queryWord.begin(),queryWord.end(),[](const std::pair<std::wstring,double>& a, const std::pair<std::wstring,double>& b){return a.second < b.second;});
    
    // Search for documents that contain all the query words
    char * wordForSearch;
    if(queryWord.size() == 0)
    {
        std::cout << "Ops, your query is in StopList or empty, so there are no documents retrieved. " << std::endl;
        return;
    }
    else if(queryWord.size() == 1)
    {
        //tf is enough
        std::vector<std::pair<int,double>> posVec;
        std::cout << "Your query has only one valid word, so we will search for it in the inverted index." << std::endl;
        std::cout << "The word was found in files below:"<< std::endl;
        printf("File name                \tTf\n");
        for(auto &p : queryWord)
        {
            wordForSearch = wstringToChararr(p.first);
            posVec = FindBP2(wordForSearch, -1, T);
            sort(posVec.begin(),posVec.end(),[](const std::pair<int,double>& a, const std::pair<int,double>& b){return a.second > b.second;});
            for(auto &pos : posVec)
                printf("%-25s\t%lf\n",docNames[pos.first],pos.second);
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
                freqMap[pos.first] += pos.second * p.second;
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
            printf("File name                \tTf-Idf\n");
            return;
        }
        else
        {
            std::vector<std::pair<int,double>> sortedFreqVec(freqMap.begin(), freqMap.end());
            std::sort(sortedFreqVec.begin(), sortedFreqVec.end(), [](const std::pair<int, double>& a, const std::pair<int, double>& b) {return a.second > b.second; });
            for(auto &p : sortedFreqVec)
            {
                printf("%-25s\t%lf\n",docNames[p.first],p.second);
                cnt++;
            }

        }
    }
}

//return <word, tf>
std::vector<std::pair<int,double>> FindBP2(string term, int docCnt, BplusTree T) 
{
    int i;

    if (!T) 
        return std::vector<std::pair<int,double>>(); 
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

std::vector<std::pair<int,double>> isSameTerm2(string term, int docCnt, NodeBP nodebp) 
{
    int i;
    std::vector<std::pair<int,double>> posVec;

    if (nodebp->size) 
    {
        for (i = 0; i < nodebp->size; i++) {
            if (!strcmp(term, nodebp->data[i]->term)) 
            {
                PosList poslist = nodebp->data[i]->poslist;
                posVec = RetrievePL2(poslist);
                break;
            }
        }
    }
    return posVec;
}


std::vector<std::pair<int,double>> RetrievePL2(PosList L) 
{
    if (!L->size) {
        printf("Empty position-data queue!\n");
        exit(1);
    }

    double tf;
    
    std::vector<std::pair<int,double>> posVec;

    PosData cur = L->front->next;

    while (cur != NULL) {
        tf = (double)cur->time / (double)fileWordsNum[cur->pos];
        posVec.push_back({cur->pos, tf});
        cur = cur->next;
    }

    return posVec;
}