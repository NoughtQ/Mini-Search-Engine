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
    if (InvIndex->size) {
        printf("Build successfully!\n");
    } else {
        printf("Fail to build an inverted index!\n");
    }
    

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
        T->data[i]->poslist = CreatePL();

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
                    EnqueuePL(docCnt, nodebp->data[i]->poslist);
                } else {
                    PosList poslist = nodebp->data[i]->poslist;
                    int size = poslist->size;
                    int cnt = 0;

                    printf("Successfully find the word!\n");
                    printf("The word was found in files below:\n");
                    
                    int j;
                    int ** posArr = (int **)malloc(sizeof(int *) * size);
                    for (j = 0; j < size; j++) {
                        posArr[i] = (int *)malloc(sizeof(int) * 2);
                    }

                    posArr = RetrievePL(poslist);

                    for (j = 0; j < size; j++) {
                        if (posArr[j][1] <= 1) 
                            printf("%s: %d time\n", docNames[posArr[j][0]], posArr[j][1]);
                        else
                            printf("%s: %d times\n", docNames[posArr[j][0]], posArr[j][1]);
                        cnt += posArr[j][1];
                    }
                    printf("Frequency: %d\n", cnt);
                    printf("-----------------------------------\n");
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
    EnqueuePL(docCnt, nodebp->data[nodebp->size++]->poslist);
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
            lnodebp->data[i] = nodebp->data[i];
        }
        lnodebp->size = cut;

        for (j = cut; j < nodebp->size; j++) {
            rnodebp->data[j - cut] = nodebp->data[j];
        }
        rnodebp->size = nodebp->size - cut;

    } else {
        cut = NONLEAFCUT;

        for (i = 0; i <= cut; i++) {
            if (i != cut) 
                lnodebp->data[i] = nodebp->data[i];
            lnodebp->children[i] = nodebp->children[i];
            lnodebp->children[i]->parent = lnodebp;
        }
        lnodebp->size = cut;
        lnodebp->childrenSize = cut + 1;

        for (j = cut + 1; j < nodebp->size; j++) {
            rnodebp->data[j - cut - 1] = nodebp->data[j];
        }
        for (j = cut + 1; j < nodebp->childrenSize; j++) {
            rnodebp->children[j - cut - 1] = nodebp->children[j];
            rnodebp->children[j - cut - 1]->parent = rnodebp;
        }     
        rnodebp->size = nodebp->size - cut - 1;
        rnodebp->childrenSize = nodebp->childrenSize - cut - 1;
    }

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

PosList CreatePL() {
    PosList L;

    L = (PosList)malloc(sizeof(struct poslist));
    L->size = 0;
    L->front = (PosData)malloc(sizeof(struct posdata));
    L->rear = (PosData)malloc(sizeof(struct posdata));
    L->front = L->rear;
    L->rear->pos = -1;

    return L;
}

void EnqueuePL(int pos, PosList L) {
    if (L->rear->pos != pos) {
        PosData tmp = (PosData)malloc(sizeof(struct posdata));
        if (!tmp) {
            printf("Fail to create a new position data!\n");
            exit(1);
        }

        tmp->pos = pos;
        tmp->time = 1;
        tmp->next = L->rear->next;
        L->rear->next = tmp;
        L->rear = tmp;
        L->size++;
    } else {
        L->rear->time++;
    }

}

int ** RetrievePL(PosList L) {
    if (!L->size) {
        printf("Empty position-data queue!\n");
        exit(1);
    }

    int i = 0, j;
    
    int ** posArr = (int **)malloc(sizeof(int *) * L->size);
    for (j = 0; j < L->size; j++) {
        posArr[j] = (int *)malloc(sizeof(int) * 2);
    }

    PosData cur = L->front->next;

    while (cur != NULL) {
        posArr[i][0] = cur->pos;
        posArr[i][1] = cur->time;
        cur = cur->next;
        i++;
    }

    return posArr;
}

HashTb GenerateHashTb() {
    int i;
    int pre, cur;
    HashTb H;
    FILE * fp;
    string fname;
    char tmp[MAXREADSTRLEN];

    H = InitHashTb();

    strcpy(fname, "stop_words.txt");
    fp = fopen(fname, "r");
    if (!fp) {
        printf("Fail to open the file of stopwords!\n");
        exit(1);
    }

    while (fgets(tmp, MAXREADSTRLEN - 1, fp) != NULL) {
        pre = cur = 0;
        for (i = 0; i < strlen(tmp); i++) {
            if (!isalpha(tmp[i])) {
                cur = i;
                if (cur > pre) {
                    term = (char *)malloc(sizeof(char) * (cur - pre + 1));
                    strncpy(term, tmp + pre, cur - pre);
                    term[cur - pre] = '\0';


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

    fclose(fp);
    return H;
}

HashTb InitHashTb() {
    HashTb H;
    int i;

    H = (HashTb)malloc(sizeof(struct hashtb));
    if (H == NULL) {
        printf("Fail to create a hash table for stopwords!"\n);
        exit(1);
    }

    H->size = STOPWORDSUM;
    H->data = (HashSW)malloc(sizeof(struct hashsw) * H->size);
    if (H->data == NULL) {
        printf("Fail to create a hash table for stopwords!\n");
        exit(1);    
    }

    for (i = 0; i < H->size; i++) {
        H->data[i]->info = Empty;
    }

    return H;
}

int FindHashSW(string stopword, HashTb H) {
    int pos;
    int collisionNum = 0;
    pos = HashFunc(stopword, H->size);
    while (H->data[pos]->info != Empty && strcmp(H->data[pos]->stopword, stopword)) {
        pos += 2 * ++collisionNum - 1;  
        if (pos >= H->size)        
            pos -= H->size;     
    }
    return pos;  
}

void InsertHashSW(string stopword, HashTb H) {
    int pos;
    pos = FindHashSW(stopword, H);
    if (H->TheCells[pos]->info != Legitimate)  
    {
        H->TheCells[pos]->info = Legitimate;
        strcpy(H->TheCells[pos]->stopword, stopword);
    }
}

int HashFunc(string stopword, int size) {
    unsigned int val = 0;
    while (*stopword != '\0')
        val = (val << 5) + *stopword++;
    return val % size;
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