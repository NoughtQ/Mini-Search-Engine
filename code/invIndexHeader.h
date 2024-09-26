// Use B+ tree to store and access to the inverted index
// Declaration of properties, methods and some constants related to B+ tree
#include <stdbool.h>
#include <stdio.h>

#ifndef INVINDEX_H
#define INVINDEX_H   // In case of re-inclusion of this header file

#define ORDER 3
#define LEAFCUT (ORDER / 2 + 1)
#define NONLEAFCUT (ORDER / 2)
#define SIZE 100000
#define MAXWORDLEN 30
#define MAXDOCSUM 500000
#define MAXREADSTRLEN 101

typedef struct nodebp * NodeBP;
typedef struct nodebp * BplusTree;
typedef char * string;

extern string docNames[MAXDOCSUM];

struct nodebp {
    int size;
    int childrenSize;

    string term[ORDER + 1];
    int time[ORDER + 1];
    // DocList docpos[ORDER + 1];

    NodeBP children[ORDER + 1];
    NodeBP parent;
};

typedef struct queuebp * QueueBP;

struct queuebp {
    int size;
    int front;
    int rear;
    NodeBP term[SIZE];
};

// typedef struct docpos * DocPos;

// struct docpos {
//     int pos;
//     DocPos next;
// };

// BplusTree InvertedIndex(string pathName);
BplusTree InvertedIndex(void);
void askforDirPath(char * path);
void fileTraversaler(BplusTree T, char * path);

// void InvertedIndexWin(BplusTree T, string pathName);
// void InvertedIndexUnix(BplusTree T, string pathName);
void GenerateInvertedIndex(BplusTree T, int docCnt, FILE * fp);

BplusTree CreateBP();
NodeBP FindBP(string x, BplusTree T, bool * flag);
void isSameTerm(string term, NodeBP nodebp, bool * flag);
void InsertBP(string x, NodeBP nodebp, BplusTree Tree);
void SplitBP(NodeBP nodebp, BplusTree Tree);
void PrintBPTree(BplusTree T);

QueueBP CreateQueueBP();
void EnqueueBP(NodeBP nodebp, QueueBP Q);
NodeBP DequeueBP(QueueBP Q);

int cmpData(const void * a, const void * b);
int cmpNodeBP(const void * a, const void * b);



#endif 