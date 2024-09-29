// Use B+ tree to store and access to the inverted index
// Declaration of properties, methods and some constants related to B+ tree
#include <stdbool.h>
#include <stdio.h>
#include <string>
#include <time.h>

#ifndef INVINDEX_H
#define INVINDEX_H   // In case of re-inclusion of this header file

#define ORDER 4                        // The order of B+ Tree
#define LEAFCUT (ORDER / 2 + 1)        // The position of the middle data in the leaf node of B+ Tree
#define NONLEAFCUT (ORDER / 2)         // The position of the middle data in the non-leaf node of B+ Tree
#define SIZE 1000000                   // The maximum size of the queue used in printing the B+ Tree
#define MAXWORDLEN 31                  // The maximum length of a single word(the longest word is about 27 or 28 in Shakespeare's works)
#define MAXDOCSUM 500000               // The maximum number of documents(files)
#define MAXREADSTRLEN 101              // The maximum lenght of string for one read
#define STOPWORDSUM 300                // The maximum number of stop words
#define STOPWORDPATH "../source/stop_words.txt"  // The path of the file storing stop words
#define DEFAULTFILEPOS "../source/tests"         // The default position of the file(for test mode)
#define SHAKESPEAREDIR "../source/shakespeare_works"
#define ITERATIONS 1

// alias
typedef char * string;
typedef struct data * Data;
typedef struct nodebp * NodeBP;
typedef struct nodebp * BplusTree;
typedef struct poslist * PosList;
typedef struct posdata * PosData;
typedef struct queuebp * QueueBP;
typedef struct hashtb * HashTb;
typedef struct hashsw * HashSW;

enum Kind {Legitimate, Empty};         // The state of the cells in hash table 
extern string docNames[MAXDOCSUM];     // Array containing names of documents(global variable)

// Nodes in B+ Trees
struct nodebp {
    int size;                        // The size of the data in the node
    int childrenSize;                // The size of the children nodes of the node
         
    Data data[ORDER + 1];            // The data of the node
    NodeBP children[ORDER + 1];      // The children nodes
    NodeBP parent;                   // The parent node(for split operation)
};

// Data of the node in B+ Trees
struct data {
    string term;                      // The term 
    PosList poslist;                  // All position where the term appears
};

// List of the position of terms(similar to the queue, but not same)
struct poslist {
    int size;                         // The size of list
    PosData front;                    // The front node of list(dummy node)
    PosData rear;                     // The rear node of list
};

// The specific position info 
struct posdata {
    int pos;                          // Position, i.e. the index of the document
    int time;                         // the frequency in a single document
    PosData next;                     // the next pointer
};

// The queue of nodes in B+ tree(array implementation)
struct queuebp {
    int size;                         // The current size of queue
    int front;                        // The index of the front node
    int rear;                         // The index of the rear node
    NodeBP data[SIZE];                // Data
};

// The hash table for stop words
struct hashtb {
    int size;                         // The maximum size of the hash table
    HashSW data[STOPWORDSUM];         // Data
};

// The cells in hash table
struct hashsw {
    string stopword;                  // Stop word
    enum Kind info;                   // State. either legitimate or empty
};

// All methods are listed here. The explanation of parameters are in the file "invIndexFunc.cpp"
// Methods for building inverted index
// The highest-level function, which users can call it directly
BplusTree InvertedIndex(bool isTest = false, bool containStopWords = false);
// Ask for the position of the directory or file
void askforFilePos(char * dir, char * fname, bool isTest);
// Make a traversal of all files(or a single file) and build the inverted index from them(or it)
BplusTree fileTraversaler(BplusTree T, char * dir, char * fname, bool isTest, bool containStopWords);
// Update the Inverted Index while reading a new document
BplusTree UpdateInvertedIndex(BplusTree T, int docCnt, FILE * fp, bool containStopWords);

// Methods about B+ tree
// Create a B+ tree
BplusTree CreateBP();
// Find a term in B+ tree
NodeBP FindBP(string term, int docCnt, BplusTree T, bool * flag, bool isSearch = false);
// Check if the term is in the B+ tree
void isSameTerm(string term, int docCnt, NodeBP nodebp, bool * flag, bool isSearch = false);
// Insert a term into the B+ tree
BplusTree InsertBP(string term, int docCnt, NodeBP nodebp, BplusTree Tree);
// Split the node when the node is full
BplusTree SplitBP(NodeBP nodebp, BplusTree Tree);
// Print the B+ tree(level-order traversal)
void PrintBPTree(BplusTree T);

// Methods about the queue
// Create the queue
QueueBP CreateQueueBP();
// Put the node of B+ tree into the queue
void EnqueueBP(NodeBP nodebp, QueueBP Q);
// Get the front node and delete it from the queue
NodeBP DequeueBP(QueueBP Q);

// Methods about poslist
// Create the poslist
PosList CreatePL();
// Add new position
void EnqueuePL(int pos, PosList L);
// Retrieve all position in the list
int ** RetrievePL(PosList L);

// Methods about hash table
// Build a hash table
HashTb GenerateHashTb();
// Initialization of the hash table
HashTb InitHashTb();
// Find the stopwords or other words in the hash table
int FindHashSW(string stopword, HashTb H, bool justSearch);
// Insert a new stopword in hash table
void InsertHashSW(string stopword, HashTb H);
// Hashing function
int HashFunc(string stopword, int size);
// Print hash table
void PrintHashTb(HashTb H);

// Comparison functions used in qsort()
int cmpData(const void * a, const void * b);     // Compare data of the node in B+ tree
int cmpNodeBP(const void * a, const void * b);   // Compare the node by their data

// wstring <-> char *, for word stemming
std::wstring chararrToWstring(char * st);
char * wstringToChararr(std::wstring wst);

// Word Stmming wrapper
string WordStem(string term);

// Print the ticks and duration, for -tr or --time function
void PrintTime(clock_t start, clock_t end);

#endif 