/**************************************** 
 * The following code is the implementation of the inverted index search algorithm.
 * The algorithm takes a query from the user and searches for the documents that contain all the valid query words.
 * The algorithm uses the B+ tree to search for the documents and the inverted index to get the word frequencies.
 * The algorithm also uses the stop list to ignore the common words and the stemming algorithm to reduce the words to their root form.
*****************************************/

#include <iostream>                  // for input/output
#include <fstream>                   // for file input/output
#include <string>
#include <string.h>
#include <map>                       // for map data structure
#include <unordered_map>             // for unordered_map data structure (hash)
#include <algorithm>                 // for sorting
#include <cctype>
#include <cmath>                     // for log()
#include "wordStem/english_stem.h"   // for stemming
#include "invIndexHeader.h"

#define DOCTOTALNUM 992             // total number of documents

//hash table: {word:wordIdf}
std::unordered_map<std::wstring, double> wordIdf;
//hash table: {docId:fileWordCount}
std::unordered_map<std::wstring, int> fileWordsNum;
//hash table: {word:bool}  PS: Bool is just a placeholder and has no actual function.
std::unordered_map<std::wstring, bool> stopWords;
stemming::english_stem<> StemEnglish;

/****************************************
 * This function is the main function for searching.
 * In this function, we analyzes the query 
 *  and search for the documents that contain all the valid query words.
 * Input: T: Inverted index (B+ tree)
 *        pageSize: number of documents to be displayed per query
 *        threshold: the proportion of search terms to the total number of terms
*****************************************/
void search(std::string query, BplusTree T, int pageSize, double threshold)
{
    /*load the query and analyze it*/
    //vector: Elements are {word:wordIdf}
    std::vector<std::pair<std::wstring,double>> queryWord;
    std::wstring word=L"";
    //Iterate through each character in the string and extract the words
    for(char &c : query)
    {
        // if the character is alpha or number, add it to the word. It solve the problem of the Punctuation and Whitespace
        if(isalnum(c))
            word += tolower(c);
        else if (word.length() > 0)
        {
            std::wstring tmp = word;    // save original word
            StemEnglish(word);
            // check if the word is in the stop list
            auto it = stopWords.find(word);
            if(it != stopWords.end())
                // if the word is in the stop list, print a warning message and ignore it.
                std::wcout << L"\033[1;31mWarning: \033[0m\"" << tmp << L"\" is in the stop list and will be ignored." << std::endl;
            else
            {
                // if the word is not in the stop list, check if it is in the inverted index.
                auto it2 = wordIdf.find(word);
                if(it2 == wordIdf.end())
                    // if the word is not in the inverted index, print a warning message and ignore it.
                    std::wcout << L"\033[1;31mWarning: \033[0m\"" << tmp << L"\" is not in the inverted index and will be ignored." << std::endl;
                else
                     // if the word is in the inverted index, add it to the queryWord vector.
                    queryWord.push_back({word,wordIdf[word]});
            }
            word = L"";    // reset the word
        }
    }
    // Sort the queryWord vector by wordIdf in descending order
    sort(queryWord.begin(),queryWord.end(),[](const std::pair<std::wstring,double>& a, const std::pair<std::wstring,double>& b){return a.second > b.second;});
    
    /* Search for documents that contain all the query words */
    int cntForDoc = 0;                                       // counter for pageSize
    int cntForWord = (int)(queryWord.size() * threshold);    // counter for threshold
    //control the size of cntForWord, to avoid bugs
    if (cntForWord == 0)
        cntForWord = 1;
    else if (cntForWord > queryWord.size())
        cntForWord = queryWord.size();
    
    char * wordForSearch;                                    // char array for search
    //if the query is empty or only has stop words, return
    if(queryWord.size() == 0)
    {
        std::cout << "\033[1;31mOps, your query is in StopList or empty, so there are no documents retrieved. \033[0m" << std::endl;
        return;
    }
    //if the query has only one valid word, search for it in the inverted index
    //here, we only use "Tf" to sort the documents in rusult
    else if(queryWord.size() == 1)
    {
        //vector: Elements are {docId:tf}
        std::vector<std::pair<int,double>> posVec;
        std::cout << "Your query has only one valid word, so we will search for it in the inverted index." << std::endl;
        std::cout << "The word was found in files below:"<< std::endl;
        printf("\033[1,32mFile name                \tTf\033[0m\n");
        //don't need to traverse here, but I wrote the case of multiple words first, so I reused it directly
        for(auto &p : queryWord)
        {
            wordForSearch = wstringToChararr(p.first);
            //find all the positions of the word in the B+ tree
            posVec = FindBP2(wordForSearch, -1, T); 
            //sort the positions by tf in descending order
            sort(posVec.begin(),posVec.end(),[](const std::pair<int,double>& a, const std::pair<int,double>& b){return a.second > b.second;});
            //print the file name and tf of the word
            for(auto &pos : posVec)
            {
                if(cntForDoc == pageSize)
                    break;
                printf("\033[34m%-25s\t%lf\033[0m\n",docNames[pos.first],pos.second);
                cntForDoc++;
            }
        }
    }
    // if the query has multiple valid words, search for them in the inverted index
    // Here, we use "Tf-Idf" to sort the documents in result
    else
    {
        std::cout << "Your query has multiple valid words, so we will search for them in the inverted index." << std::endl;
        std::cout << "The words were found in files below:"<< std::endl;
        //hash table: {docId : sum of tf-idf}
        std::unordered_map<int, double> freqMap;
        //traverse the queryWord vector and search for each word in the inverted index
        bool flag=false;
        for(auto &p : queryWord)
        {
            if(cntForWord == 0)
                break;
            
            wordForSearch = wstringToChararr(p.first);
            auto currentPosVec = FindBP2(wordForSearch, -1, T);
        
            //Transfer the values ​​in the vector to a temporary hash table, mainly for efficiency reasons.
            //The temporary hash table only contains the pair of docId and idf.
            std::unordered_map<int, double> currentDocIdMap;
            if(flag == false)
            {
                for(auto &pos : currentPosVec)
                    freqMap[pos.first] += pos.second * p.second;    // tf-idf = tf * idf
                flag = true;
            }
            else
            {
                for(auto &pos : currentPosVec)
                    currentDocIdMap[pos.first] = pos.second * p.second;
                if(freqMap.size()>0)
                {
                    //remove "it++" in "for(;;)"" to remove duplicates
                    for (auto it = freqMap.begin(); it != freqMap.end(); )
                    {
                        if (currentDocIdMap.find(it->first) == currentDocIdMap.end()) 
                            it = freqMap.erase(it);//safe deletion
                        else
                        {
                            it->second += currentDocIdMap[it->first];
                            ++it;
                        }
                    }
                }
                else
                    break;
            }
            cntForWord--;
        }
        //if there is no document that contains all the query words, return
        if(freqMap.size() == 0)
        {
            std::cout << "Ops, your query is not in the inverted index, so there are no documents retrieved. " << std::endl;
            std::cout << "Please try again with different query." << std::endl;
            return;
        }
        //if there is at least one document that contains all the query words, sort them by tf-idf in descending order
        else
        {
            printf("\033[1;32mFile name                \tTf-Idf\033[0m\n");
            //convert the unordered_map to a vector and sort it by tf-idf in descending order
            std::vector<std::pair<int,double>> sortedFreqVec(freqMap.begin(), freqMap.end());
            std::sort(sortedFreqVec.begin(), sortedFreqVec.end(), [](const std::pair<int, double>& a, const std::pair<int, double>& b) {return a.second > b.second; });
            //print the file name and tf-idf of the word
            for(auto &p : sortedFreqVec)
            {
                if(cntForDoc == pageSize)
                    break;
                printf("\033[34m%-25s\t%lf\033[0m\n",docNames[p.first],p.second);
                cntForDoc++;
            }

        }
    }
}

// similar to FindBP() in invIndexFunc.cpp, but desgined just for searching 
// return <word, tf>
std::vector<std::pair<int,double>> FindBP2(string term, int docCnt, BplusTree T) 
{
    int i;

    // if the tree is empty, return an empty vector
    if (!T) 
        return std::vector<std::pair<int,double>>(); 
    else if (!T->childrenSize) 
        return isSameTerm2(term, docCnt, T);

    int pos = -1;
    
    // find the position of the term in the current node
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

//similar to isSameTerm() in invIndexFunc.cpp, but designed just for searching
//return <docId, tf>
std::vector<std::pair<int,double>> isSameTerm2(string term, int docCnt, NodeBP nodebp) 
{
    int i;
    // vector: Elements are {docId:idf}
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

//similar to RetrievePL() in invIndexFunc.cpp, but designed just for searching
//return <docId, tf>
std::vector<std::pair<int,double>> RetrievePL2(PosList L) 
{
    // if the position-data queue is empty, return an empty vector
    if (!L->size) {
        printf("Empty position-data queue!\n");
        exit(1);
    }

    double tf;
    std::vector<std::pair<int,double>> posVec;
    PosData cur = L->front->next;

    while (cur != NULL) {
        std::wstring docName = chararrToWstring(docNames[cur->pos]);
        // calculate tf : tf = word freq/total words in file
        tf = (double)cur->time / (double)(fileWordsNum[docName]);
        posVec.push_back({cur->pos, tf});
        cur = cur->next;
    }

    return posVec;
}

/*The functions below load the wordIdf, fileWordsNum, and stopWords from files*/

void loadWordIdf(std::string filePath)
{
    std::wifstream infile;
    std::wstring word;
    int freq;
    double idf;
    infile.open(filePath,std::ios::in);
    //if the file is empty, return
    if(!infile.is_open())
    {
        std::cout << "Error: unable to open file " << filePath << std::endl;
        return;
    }
    //read the word and its frequency from the file and calculate its idf
    while(infile >> word >> freq)
    {
        //calculate idf : idf = log(total number of documents/(1 + number of documents contain the word))
        idf = log((double)DOCTOTALNUM/(double)(1+freq));
        wordIdf[word] = idf;
    }
    infile.close();
    std::cout << "Word Idf loaded successfully." << std::endl;
}

void loadFileWordsNum(std::string filePath)
{
    std::wifstream infile;
    std::wstring filename;
    int num;
    infile.open(filePath,std::ios::in);
    //if the file is empty, return
    if(!infile.is_open())
    {
        std::cout << "Error: unable to open file " << filePath << std::endl;
        return;
    }
    //read the file name and its word count from the infile
    while(infile >> filename >> num)
    {
        filename += L".txt";
        fileWordsNum[filename] = num;
    }
    infile.close();
    std::cout << "The word count of every file loaded successfully." << std::endl;
}

void loadStopWords(std::string filePath)
{
    std::wifstream infile;
    std::wstring word;
    infile.open(filePath,std::ios::in);
    //if the file is empty, return
    if(!infile.is_open())
    {
        std::cout << "Error: unable to open file " << filePath << std::endl;
        return;
    }
    while(infile >> word)
        stopWords[word] = true;
    std::cout << "Stop words loaded successfully." << std::endl;
}
