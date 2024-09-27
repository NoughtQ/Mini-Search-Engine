// Use B+ tree to store and access to the inverted index
// Declaration of properties, methods and some constants related to B+ tree
#include <stdbool.h>
#include <stdio.h>
#include <string>

#ifndef INVINDEX_H
#define INVINDEX_H   // In case of re-inclusion of this header file

#define ORDER 3
#define LEAFCUT (ORDER / 2 + 1)
#define NONLEAFCUT (ORDER / 2)
#define SIZE 1000000
#define MAXWORDLEN 31
#define MAXDOCSUM 500000
#define MAXREADSTRLEN 101
#define STOPWORDSUM 200
#define TESTMODE true

enum Kind {Legitimate, Empty, Deleted};

typedef char * string;
typedef struct data * Data;
typedef struct nodebp * NodeBP;
typedef struct nodebp * BplusTree;
typedef struct poslist * PosList;
typedef struct posdata * PosData;
typedef struct queuebp * QueueBP;
typedef struct hashtb * HashTb;
typedef struct hashsw * HashSW;

extern string docNames[MAXDOCSUM];


struct nodebp {
    int size;
    int childrenSize;

    string term[ORDER + 1];
    Data data[ORDER + 1];

    NodeBP children[ORDER + 1];
    NodeBP parent;
};


struct data {
    string term;
    PosList poslist;
};

struct poslist {
    int size;
    PosData front;
    PosData rear;
};

struct posdata {
    int pos;
    int time;
    PosData next;
};

struct queuebp {
    int size;
    int front;
    int rear;
    NodeBP data[SIZE];
};

struct hashtb {
    int size;
    HashSW data[STOPWORDSUM];
};

struct hashsw {
    string stopword;
    enum Kind info;
}


BplusTree InvertedIndex(bool isTest = false);
void askforFilePos(char * dir, char * fname, bool isTest);
BplusTree fileTraversaler(BplusTree T, char * dir, char * fname, bool isTest);

BplusTree GenerateInvertedIndex(BplusTree T, int docCnt, FILE * fp);

BplusTree CreateBP();
NodeBP FindBP(string term, int docCnt, BplusTree T, bool * flag, bool isSearch = false);
void isSameTerm(string term, int docCnt, NodeBP nodebp, bool * flag, bool isSearch = false);
BplusTree InsertBP(string term, int docCnt, NodeBP nodebp, BplusTree Tree);
BplusTree SplitBP(NodeBP nodebp, BplusTree Tree);
void PrintBPTree(BplusTree T);

QueueBP CreateQueueBP();
void EnqueueBP(NodeBP nodebp, QueueBP Q);
NodeBP DequeueBP(QueueBP Q);

PosList CreatePL();
void EnqueuePL(int pos, PosList L);
int ** RetrievePL(PosList L);

HashTb GenerateHashTb();
HashTb InitHashTb();
int FindHashSW(string stopword, HashTb H);
void InsertHashSW(string stopword, HashTb H);
int HashFunc(string stopword, int size);

int cmpData(const void * a, const void * b);
int cmpNodeBP(const void * a, const void * b);

std::wstring chararrToWstring(char * st);
char * wstringToChararr(std::wstring wst);

#endif 