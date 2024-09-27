// Implementation of methods related to B+ tree in invIndex.h
#include "invIndexHeader.h"
#include "wordStem/english_stem.h"
#include <codecvt>
#include <filesystem>
#include <locale>
#include <string>

extern "C" {
    #include <ctype.h>
    #include <stdbool.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
}

namespace fs = std::filesystem;


string docNames[MAXDOCSUM];

BplusTree InvertedIndex(bool isTest) {
    char dir[MAXREADSTRLEN];
    char fname[MAXREADSTRLEN];
    BplusTree InvIndex = CreateBP();

    askforFilePos(dir, fname, isTest);
    InvIndex = fileTraversaler(InvIndex, dir, fname, isTest);
    printf("Build successfully!\n");

    return InvIndex;
}

void askforFilePos(char * dir, char * fname, bool isTest) {
    if (isTest) {
        printf("Now testing the correctness of inverted Index:\n");
        printf("Please input the name of the input sample file:\nName: ");
        scanf("%s", fname);
    } else {
        printf("Now building an inverted Index:\n");
        printf("Please input the directory of the documents:\nPath: ");
        scanf("%s", dir);
    }
}

BplusTree fileTraversaler(BplusTree T, char * dir, char * fname, bool isTest) {
    int docCnt = 0;
    char * wholePath;
    FILE * fp = NULL;

    wholePath = new char[MAXREADSTRLEN];
    if (!isTest) {
        fs::path dirPath(dir);
        if (fs::exists(dirPath) && fs::is_directory(dirPath)) {
            for (const auto& entry : fs::directory_iterator(dirPath)) {
                if (fs::is_regular_file(entry)) {
                    std::string filename = entry.path().filename().string();
                    // std::cout << filename << std::endl;
                    docNames[docCnt] = new char[filename.length() + 1]; 
                    strcpy(docNames[docCnt], filename.c_str());
                    strcpy(wholePath, (dirPath.string() + "/" + filename).c_str());
                    
                    fp = fopen(wholePath, "r");
                    if (!fp) {
                        printf("Couldn't open the file!\n");
                        exit(1);
                    }

                    T = GenerateInvertedIndex(T, docCnt++, fp);
                }
            }
            if (fp)
                fclose(fp);
        } else {
            perror("Could not open directory");
            
        }
    } else {
        strcpy(dir, "tests");
        std::string sdir(dir);
        std::string sfname(fname);
        strcpy(wholePath, (sdir + "/" + sfname).c_str());
        // printf("The whole path: %s\n", wholePath);

        docNames[docCnt] = (string)malloc(sizeof(char) * (strlen(fname) + 1));
        strcpy(docNames[docCnt], fname);

        fp = fopen(wholePath, "r");
        if (!fp) {
            printf("Couldn't open the file!\n");
            exit(1);
        }
        
        T = GenerateInvertedIndex(T, docCnt++, fp);   
        fclose(fp);
    }

    return T;
}

BplusTree GenerateInvertedIndex(BplusTree T, int docCnt, FILE * fp) {
    int i;
    int pre, cur;
    char tmp[MAXREADSTRLEN];
    char * term;
    bool isDuplicated;
    NodeBP nodebp;
    std::wstring term_wstr;
    stemming::english_stem<> StemEnglish;

    while (fgets(tmp, MAXREADSTRLEN - 1, fp) != NULL) {
        pre = cur = 0;
        // tmp[strlen(tmp)] = '\n';
        for (i = 0; i < strlen(tmp); i++) {
            if (!isalpha(tmp[i])) {
                cur = i;
                if (cur > pre) {
                    term = (char *)malloc(sizeof(char) * (cur - pre + 1));
                    strncpy(term, tmp + pre, cur - pre);
                    term[cur - pre] = '\0';

                    term_wstr = chararrToWstring(term);
                    StemEnglish(term_wstr);
                    term = wstringToChararr(term_wstr);
                    
                    isDuplicated = false;
                    nodebp = FindBP(term, docCnt, T, &isDuplicated);
                    if (!isDuplicated) {
                        T = InsertBP(term, docCnt, nodebp, T);
                    }      
                }

                pre = cur + 1;
            }
        }

        if (!cur || pre > cur && pre != i) {
            cur = i;
            term = (char *)malloc(sizeof(char) * (cur - pre + 1));
            strncpy(term, tmp + pre, cur - pre);
            term[cur - pre] = '\0';

            term_wstr = chararrToWstring(term);
            StemEnglish(term_wstr);
            term = wstringToChararr(term_wstr);
            
            isDuplicated = false;
            nodebp = FindBP(term, docCnt, T, &isDuplicated);
            if (!isDuplicated) {
                T = InsertBP(term, docCnt, nodebp, T);
            }               
        }     
    }

    return T;
}


BplusTree CreateBP() {
    BplusTree T = (BplusTree)malloc(sizeof(struct nodebp));
    if (T == NULL) {
        printf("Failed to create a B+ Tree!\n");
        return T;
    }

    int i;
    
    for (i = 0; i <= ORDER; i++) {
        T->data[i] = (Data)malloc(sizeof(struct data));
        T->data[i]->term = (string)malloc(sizeof(char) * MAXWORDLEN);
        T->data[i]->poslist = (PosList)malloc(sizeof(struct poslist));
        T->data[i]->poslist->size = 0;
        T->data[i]->poslist->front = (PosData)malloc(sizeof(struct posdata));
        T->data[i]->poslist->rear = (PosData)malloc(sizeof(struct posdata));
        T->data[i]->poslist->front = T->data[i]->poslist->rear;

        T->children[i] = (NodeBP)malloc(sizeof(struct nodebp));
    }

    T->size = 0;
    T->childrenSize = 0;
    T->parent = NULL;

    return T;
}

NodeBP FindBP(string term, int docCnt, BplusTree T, bool * flag, bool isSearch) {
    int i;

    if (!T) {
        return T;
    } else if (!T->childrenSize) {
        isSameTerm(term, docCnt, T, flag, isSearch);
        return T;
    }

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

    return FindBP(term, docCnt, T->children[pos], flag, isSearch);
}

void isSameTerm(string term, int docCnt, NodeBP nodebp, bool * flag, bool isSearch) {
    int i;

    if (nodebp->size) {
        for (i = 0; i < nodebp->size; i++) {
            if (!strcmp(term, nodebp->data[i]->term)) {
                if (!isSearch) {
                    // nodebp->data[i]->time++;
                    EnqueuePD(docCnt, nodebp->data[i]->poslist);
                } else {
                    int size = nodebp->data[i]->poslist->size;
                    printf("Successfully find the word!\n");
                    printf("Frequency: %d\n", size);
                    printf("The word was found in files below:\n");
                    
                    // PosData cur = nodebp->data[i]->poslist->front->next;
                    // int abspos = 0;
                    int j;
                    int * posArr = (int *)malloc(sizeof(int) * size);

                    posArr = RetrievePD(nodebp->data[i]->poslist);

                    for (j = 0; j < size; j++) {
                        // abspos += posArr[j];
                        printf("%s\n", docNames[posArr[j]]);
                    }
                    printf("-----------------------------------\n");

                    // while (cur != NULL) {
                    //     abspos += cur->pos;
                    //     printf("%s\n", docNames[abspos]);
                    //     cur = cur->next;
                    // }
                }

                *flag = true;
                break;
            }
        }
    }
}

BplusTree InsertBP(string term, int docCnt, NodeBP nodebp, BplusTree Tree) {
    int i;

    strcpy(nodebp->data[nodebp->size]->term, term);
    // nodebp->data[nodebp->size++]->time++;
    EnqueuePD(docCnt, nodebp->data[nodebp->size++]->poslist);
    qsort(nodebp->data, nodebp->size, sizeof(nodebp->data[0]), cmpData);

    Tree = SplitBP(nodebp, Tree);
    return Tree;
}

BplusTree SplitBP(NodeBP nodebp, BplusTree Tree) {
    if (!nodebp->childrenSize && nodebp->size <= ORDER 
      || nodebp->childrenSize && nodebp->size < ORDER) {
        return Tree;
    }

    NodeBP lnodebp, rnodebp, tmpNodeBP, parent;
    int cut;
    int i, j;

    parent = nodebp->parent;

    if (!parent) {
        tmpNodeBP = CreateBP();
        parent = (NodeBP)malloc(sizeof(struct nodebp));
        Tree = parent = tmpNodeBP;
    }

    lnodebp = CreateBP();
    rnodebp = CreateBP();
    lnodebp->parent = rnodebp->parent = parent;

    if (!nodebp->childrenSize) {
        cut = LEAFCUT;
        
        for (i = 0; i < cut; i++) {
            // strcpy(lnodebp->data[i]->term, nodebp->data[i]->term);
            lnodebp->data[i] = nodebp->data[i];
        }
        lnodebp->size = cut;

        for (j = cut; j < nodebp->size; j++) {
            // strcpy(rnodebp->data[j - cut]->term nodebp->data[j]->term);
            rnodebp->data[j - cut] = nodebp->data[j];
        }
        rnodebp->size = nodebp->size - cut;

    } else {
        cut = NONLEAFCUT;

        for (i = 0; i <= cut; i++) {
            if (i != cut) 
                lnodebp->data[i] = nodebp->data[i];
                // strcpy(lnodebp->data[i]->term, nodebp->data[i]->term);
            lnodebp->children[i] = nodebp->children[i];
            lnodebp->children[i]->parent = lnodebp;
        }
        lnodebp->size = cut;
        lnodebp->childrenSize = cut + 1;

        for (j = cut + 1; j < nodebp->size; j++) {
            rnodebp->data[j - cut - 1] = nodebp->data[j];
            // strcpy(rnodebp->data[j - cut - 1]->term, nodebp->data[j]->term);
        }
        for (j = cut + 1; j < nodebp->childrenSize; j++) {
            rnodebp->children[j - cut - 1] = nodebp->children[j];
            rnodebp->children[j - cut - 1]->parent = rnodebp;
        }     
        rnodebp->size = nodebp->size - cut - 1;
        rnodebp->childrenSize = nodebp->childrenSize - cut - 1;
    }

    // strcpy(parent->data[parent->size++]->term, nodebp->data[cut]->term);
    parent->data[parent->size++] = nodebp->data[cut];
    if (parent->childrenSize) {
        for (i = 0; i < parent->childrenSize; i++) {
            if (parent->children[i] == nodebp) {
                parent->children[i] = lnodebp;
                break;
            }
        }
    } else {
        parent->children[parent->childrenSize++] = lnodebp;
    }
    parent->children[parent->childrenSize++] = rnodebp;

    qsort(parent->data, parent->size, sizeof(parent->data[0]), cmpData);
    qsort(parent->children, parent->childrenSize, sizeof(parent->children[0]), cmpNodeBP);

    free(nodebp);

    Tree = SplitBP(parent, Tree);

    return Tree;
}

void PrintBPTree(BplusTree T) {
    int i;
    int flag;
    NodeBP nodebp;
    QueueBP q;

    q = CreateQueueBP();
    EnqueueBP(T, q);
    EnqueueBP(NULL, q);

    while (q->size) {
        nodebp = DequeueBP(q);
        if (!nodebp) {
            printf("\n");
            if (q->size) {
                EnqueueBP(NULL, q);
            }
        } else {
            printf("[");
            for (i = 0; i < nodebp->size; i++) {
                if (!i) {
                    printf("%s", nodebp->data[i]->term);
                } else {
                    printf(", %s", nodebp->data[i]->term);
                }
            }
            printf("]");
        }

        if (nodebp) {
            for (i = 0; i < nodebp->childrenSize; i++) {
                EnqueueBP(nodebp->children[i], q);
            }
        }

    }

}

QueueBP CreateQueueBP() {
    QueueBP Q = (QueueBP)malloc(sizeof(struct queuebp));
    Q->size = 0;
    Q->front = Q->rear = 0;

    return Q;
}

void EnqueueBP(NodeBP nodebp, QueueBP Q) {
    if (Q->size >= SIZE) {
        printf("Full B+-tree-item queue!\n");
        exit(1);
    }
    Q->data[Q->rear++] = nodebp;
    Q->size++;
}

NodeBP DequeueBP(QueueBP Q) {
    if (!Q->size) {
        printf("Empty B+-tree-item queue!\n");
        exit(1);
    }
    NodeBP returnNodeBP = Q->data[Q->front++];
    Q->size--;
    return returnNodeBP;
}

void EnqueuePD(int pos, PosList L) {
    PosData tmp = (PosData)malloc(sizeof(struct posdata));
    if (!tmp) {
        printf("Fail to create a new position data!\n");
        exit(1);
    }

    tmp->pos = pos;
    tmp->next = L->rear->next;
    L->rear->next = tmp;
    L->rear = tmp;
    L->size++;
}

int * RetrievePD(PosList L) {
    if (!L->size) {
        printf("Empty position-data queue!\n");
        exit(1);
    }

    int i = 0;
    int * posArr = (int *)malloc(sizeof(int) * L->size);
    PosData cur = L->front->next;

    while (cur != NULL) {
        posArr[i++] = cur->pos;
        cur = cur->next;
    }

    return posArr;
}

int cmpData(const void * a, const void * b) {
    const Data dataA = *(const Data*)a;
    const Data dataB = *(const Data*)b;

    return strcmp(dataA->term, dataB->term);
}

int cmpNodeBP(const void * a, const void * b) {
    const NodeBP nodebpA = *(const NodeBP*)a;
    const NodeBP nodebpB = *(const NodeBP*)b;

    return strcmp(nodebpA->data[0]->term, nodebpB->data[0]->term);
}

std::wstring chararrToWstring(char * st) {
    std::string tmp(st);
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::wstring wstr = converter.from_bytes(tmp);

    return wstr;
}

char * wstringToChararr(std::wstring wst) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::string tmp = converter.to_bytes(wst);
    char * st = new char[tmp.size() + 1];
    strcpy(st, tmp.c_str());

    return st;
}