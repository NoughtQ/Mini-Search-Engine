// Implementation of methods related to B+ tree in invIndex.h
#include "invIndexHeader.h"
#include "wordStem/english_stem.h"
#include <codecvt>
#include <filesystem>
#include <locale>
#include <string>
// To avoid unexpected import problems
extern "C" {
    #include <ctype.h>
    #include <stdbool.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
}
// file system namespace
namespace fs = std::filesystem;    

string docNames[MAXDOCSUM];      // Array containing names of documents(global variable)

// The highest-level function, which users can call it directly
// isTest: -t or --test mode, just use one particular file
// containStopWords: -s or --stopwords mode, contain stop words when building inverted index
BplusTree InvertedIndex(bool isTest, bool containStopWords) {
    char dir[MAXREADSTRLEN];             // Directory name
    char fname[MAXREADSTRLEN];           // File name
    BplusTree InvIndex = CreateBP();     // Inverted index, stored in B+ tree

    askforFilePos(dir, fname, isTest);   // Ask for the position of file or directory
    // Make a traversal in the directory(or a single file) and build the inverted index from it
    InvIndex = fileTraversaler(InvIndex, dir, fname, isTest, containStopWords);
    if (InvIndex->size) {                // If the inverted index contains the data, it indicates the success of building
        printf("Build successfully!\n");
    } else {                             // Otherwise, it fails
        printf("Fail to build an inverted index!\n");
    }

    return InvIndex;                     // Return the final inverted index
}

// Ask for the position of the directory or file
// dir: directory name
// fname: file name
// isTest: -t or --test mode, just use one particular file
void askforFilePos(char * dir, char * fname, bool isTest) {
    if (isTest) {    // If we choose the test mode in the test file, 
        printf("Now testing the correctness of inverted Index:\n");
        printf("Please input the name of the input sample file:\nName: ");
        scanf("%s", fname);  // then we should input the filename
    } else {         // Otherwise(in the main program or other default situations), 
        printf("Now building an inverted Index:\n");
        printf("Please input the directory of the documents:\nPath: ");
        scanf("%s", dir);    // we should input the name of the directory
    }
}

// Make a traversal of all files(or a single file) and build the inverted index from them(or it)
// T: B+ tree containing the inverted index
// dir: directory name
// fname: file name
// isTest: -t or --test mode, just use one particular file
// containStopWords: -s or --stopwords mode, contain stop words when building inverted index
BplusTree fileTraversaler(BplusTree T, char * dir, char * fname, bool isTest, bool containStopWords) {
    int docCnt = 0;      // Count the number of documents and act as the index of the documents at the same time
    char * wholePath;    // The whole path name
    FILE * fp = NULL;    // File pointer

    wholePath = new char[MAXREADSTRLEN];
    if (!isTest) {       // If we choose the test mode in the test file, 
        fs::path dirPath(dir);
        if (fs::exists(dirPath) && fs::is_directory(dirPath)) {   // Make a traversal in the directory 
            for (const auto& entry : fs::directory_iterator(dirPath)) {
                if (fs::is_regular_file(entry)) {    // entry: a single file
                    std::string filename = entry.path().filename().string();  // Get the file name
                    docNames[docCnt] = new char[filename.length() + 1]; 
                    strcpy(docNames[docCnt], filename.c_str());   // Store the filename
                    strcpy(wholePath, (dirPath.string() + "/" + filename).c_str());  // Get the whole path name
                    // Open the file
                    fp = fopen(wholePath, "r");
                    if (!fp) {  // Error handler
                        printf("Couldn't open the file!\n");
                        exit(1);
                    }
                    // Update the inverted index
                    T = UpdateInvertedIndex(T, docCnt++, fp, containStopWords);
                }
            }
            if (fp)  // Don't forget close the file pointer
                fclose(fp);
        } else {  // Input wrong directory
            perror("Could not open directory");
            
        }
    } else {
        strcpy(dir, DEFAULTFILEPOS);       // The file is in the default position
        std::string sdir(dir);
        std::string sfname(fname);
        strcpy(wholePath, (sdir + "/" + sfname).c_str());  // Get the whole path name

        docNames[docCnt] = (string)malloc(sizeof(char) * (strlen(fname) + 1));
        strcpy(docNames[docCnt], fname);                   // Store the filename
        // Open the file
        fp = fopen(wholePath, "r");
        if (!fp) {  // Error handler
            printf("Couldn't open the file!\n");
            exit(1);
        }
        // Update the inverted index
        T = UpdateInvertedIndex(T, docCnt++, fp, containStopWords);   
        fclose(fp);    // Don't forget close the file pointer
    }

    return T;
}

// Update the Inverted Index while reading a new document
// T: B+ tree containing the inverted index
// docCnt: the index of the document
// fp: file pointer
// containStopWords: -s or --stopwords mode, contain stop words when building inverted index
BplusTree UpdateInvertedIndex(BplusTree T, int docCnt, FILE * fp, bool containStopWords) {
    int i;         
    int pre, cur;                          // Mark the start and the end of one word
    char tmp[MAXREADSTRLEN];               // Memory space storing the reading data temporarily
    char * term;                           // Term(or word)
    bool isDuplicated;                     // A flag, record whether the term exists in the B+ tree
    NodeBP nodebp;                         // Node in B+ tree
    HashTb H;                              // Hash table storing the stop wors
    std::wstring term_wstr;                // the wstring form of the term
    stemming::english_stem<> StemEnglish;  // Word stemming function(a little clumsy)

    H = GenerateHashTb();                  // Build a hash table for stop words

    while (fgets(tmp, MAXREADSTRLEN - 1, fp) != NULL) {  // Continue reading the file, until arrive at the end of file
        pre = cur = 0;                        // Initialization
        for (i = 0; i < strlen(tmp); i++) {   // Retrieve all characters in the tmp string
            if (!isalpha(tmp[i])) {           // Maybe it's time to record a word
                cur = i;
                if (cur > pre) {              // Legitimate situation
                    term = (char *)malloc(sizeof(char) * (cur - pre + 1));
                    strncpy(term, tmp + pre, cur - pre);
                    term[cur - pre] = '\0';   // Don't forget this step

                    // If we consider the stop words(default) and assure the term is a stop word, 
                    if (!containStopWords && FindHashSW(term, H, true) >= 0) {
                        pre = cur + 1;
                        continue;  // then we should ignore it
                    }

                    // Word stemming
                    term_wstr = chararrToWstring(term);
                    StemEnglish(term_wstr);
                    term = wstringToChararr(term_wstr);
                    
                    isDuplicated = false;
                    nodebp = FindBP(term, docCnt, T, &isDuplicated);  // Find the appropriate position for the term
                    // If isDuplicated is true, then the time of the term will +1 in function isSameTerm()
                    if (!isDuplicated) {  // If it's a new term, insert it!
                        T = InsertBP(term, docCnt, nodebp, T);
                    }      
                }

                pre = cur + 1;     // Move the start position for possible new word
            }
        }

        // Handle the last possible word in the tmp string
        if (!cur || pre > cur && pre != i) {
            cur = i;
            term = (char *)malloc(sizeof(char) * (cur - pre + 1));
            strncpy(term, tmp + pre, cur - pre);
            term[cur - pre] = '\0';

            // Word stemming
            term_wstr = chararrToWstring(term);
            StemEnglish(term_wstr);
            term = wstringToChararr(term_wstr);
            
            isDuplicated = false;
            nodebp = FindBP(term, docCnt, T, &isDuplicated); // Find the appropriate position for the term
            // If isDuplicated is true, then the time of the term will +1 in function isSameTerm()
            if (!isDuplicated) {  // If it's a new term, insert it!
                T = InsertBP(term, docCnt, nodebp, T);
            }               
        }     
    }

    return T;
}

// Create a B+ tree
BplusTree CreateBP() {
    BplusTree T = (BplusTree)malloc(sizeof(struct nodebp));  // Allocate the memory space for new B+ tree
    if (T == NULL) {    // Allocation failure
        printf("Failed to create a B+ Tree!\n");
        return T;
    }

    int i;
    
    // Memory allocation and initialization of data and children
    for (i = 0; i <= ORDER; i++) {
        T->data[i] = (Data)malloc(sizeof(struct data));
        T->data[i]->term = (string)malloc(sizeof(char) * MAXWORDLEN);
        T->data[i]->poslist = CreatePL();
        T->children[i] = (NodeBP)malloc(sizeof(struct nodebp));
    }
    // Initialization of other fields
    T->size = 0;
    T->childrenSize = 0;
    T->parent = NULL;

    return T;
}

// Find a term in B+ tree
// term: term
// docCnt: the index of the document
// T: inverted index
// flag: true if the term is found, false otherwise
// isSearch: mark the find mode(-f or --find)
NodeBP FindBP(string term, int docCnt, BplusTree T, bool * flag, bool isSearch) {
    int i;

    if (!T) {    // If the tree is empty, return the tree(actually, it's impossible in our program)
        return T;
    } else if (!T->childrenSize) {  // If we arrive at the leaf node, search its data
        isSameTerm(term, docCnt, T, flag, isSearch);  
        return T;
    }

    int pos = -1;  // The index of the appopriate non-leaf node
    for (i = 0; i < T->size; i++) {
        if (strcmp(term, T->data[i]->term) < 0) { // Find the first node which have term with higher lexicographic number
            pos = i;
            break;
        }
    }
    if (pos == -1) {  // If no position found in above loop, choose the last node
        pos = i;
    }

    return FindBP(term, docCnt, T->children[pos], flag, isSearch);  // Continue finding in the children node
}

// Check if the term is in the B+ tree
// term: term
// docCnt: the index of the document
// nodebp: the appropriate node where the term may exists or will exists after insertion
// flag: true if the term is found, false otherwise
// isSearch: mark the find mode(-f or --find)
void isSameTerm(string term, int docCnt, NodeBP nodebp, bool * flag, bool isSearch) {
    int i;

    if (nodebp->size) {     // If it's not an empty node, start searching
        for (i = 0; i < nodebp->size; i++) {
            if (!strcmp(term, nodebp->data[i]->term)) {     // If the term exists in the inverted index
                if (!isSearch) {     // If it's not in the find mode
                    EnqueuePL(docCnt, nodebp->data[i]->poslist);  // Update the poslist of the term
                } else {    // Otherwise, print all info of the term
                    PosList poslist = nodebp->data[i]->poslist;     // Position list
                    int size = poslist->size;                       // The number of all documents where the term appears
                    int cnt = 0;                                    // Record the total frequency of the term

                    printf("Successfully find the word!\n");        // Some banners
                    printf("The word was found in files below:\n");
                    
                    int j;                                          
                    int ** posArr = (int **)malloc(sizeof(int *) * size);  // Allocation of a 2D array
                    for (j = 0; j < size; j++) {
                        posArr[i] = (int *)malloc(sizeof(int) * 2);
                    }

                    posArr = RetrievePL(poslist);                    // Put the poslist in a 2D array

                    for (j = 0; j < size; j++) {  // Print the name of documents and their frequency respectively
                        if (posArr[j][1] <= 1)  // Singular
                            printf("%s: %d time\n", docNames[posArr[j][0]], posArr[j][1]);
                        else   // Plural
                            printf("%s: %d times\n", docNames[posArr[j][0]], posArr[j][1]);
                        cnt += posArr[j][1];
                    }
                    printf("Frequency: %d\n", cnt);                  // The total frequency
                    printf("-----------------------------------\n");
                }

                *flag = true;      // mark the flag, indicating we find the term
                break;
            }
        }
    }
}

// Insert a term into the B+ tree
// term: term
// docCnt: the index of the document
// nodebp: the appropriate node where the term will be inserted
// Tree: B+ tree containing the inverted index
BplusTree InsertBP(string term, int docCnt, NodeBP nodebp, BplusTree Tree) {
    int i;

    strcpy(nodebp->data[nodebp->size]->term, term);
    EnqueuePL(docCnt, nodebp->data[nodebp->size++]->poslist);   // Add the data info
    qsort(nodebp->data, nodebp->size, sizeof(nodebp->data[0]), cmpData);   // Sort the data in time

    Tree = SplitBP(nodebp, Tree);    // Split the node
    return Tree;
}

// Split the node when the node is full
// nodebp: the appropriate node where the term will be inserted
// Tree: B+ tree containing the inverted index
BplusTree SplitBP(NodeBP nodebp, BplusTree Tree) {
    if (!nodebp->childrenSize && nodebp->size <= ORDER     // If the node is not full
      || nodebp->childrenSize && nodebp->size < ORDER) {   // (consider both leaf node and non-leaf node), 
        return Tree;                                       // do nothing!
    }

    // lnodebp, rnodebp: the left and right part of the split node
    // tmpNodebp: store the node temporarily
    // parent: the parent node of nodebp
    NodeBP lnodebp, rnodebp, tmpNodebp, parent;            
    int cut;    // The position of the middle data
    int i, j;   

    parent = nodebp->parent;

    if (!parent) {  // If the node has no parent(i.e. this node is the root), 
        tmpNodebp = CreateBP();  // create a new node as the parent(and also the root of the tree)
        parent = (NodeBP)malloc(sizeof(struct nodebp));
        Tree = parent = tmpNodebp;
    }

    lnodebp = CreateBP();
    rnodebp = CreateBP();
    lnodebp->parent = rnodebp->parent = parent;  // Connect the two parts with the parent node

    if (!nodebp->childrenSize) {   // If the node is the leaf node
        cut = LEAFCUT;
        
        for (i = 0; i < cut; i++) {    // Assign the data in the left part of original node to lnodebp
            lnodebp->data[i] = nodebp->data[i];
        }
        lnodebp->size = cut;

        for (j = cut; j < nodebp->size; j++) {   // Assign the data in the right part of original node to rnodebp
            rnodebp->data[j - cut] = nodebp->data[j];
        }
        rnodebp->size = nodebp->size - cut;

    } else {                       // If the node is the non-leaf node
        cut = NONLEAFCUT;

        for (i = 0; i <= cut; i++) {    // Assign the data and children in the left part of original node to lnodebp
            if (i != cut) 
                lnodebp->data[i] = nodebp->data[i];
            lnodebp->children[i] = nodebp->children[i];
            lnodebp->children[i]->parent = lnodebp;
        }
        lnodebp->size = cut;
        lnodebp->childrenSize = cut + 1;

        // Assign the data and children in the right part of original node to rnodebp
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

    // Assign the middle data in the original node to its parent
    parent->data[parent->size++] = nodebp->data[cut];
    if (parent->childrenSize) {    // If the parent has children(not be created newly)
        for (i = 0; i < parent->childrenSize; i++) {
            if (parent->children[i] == nodebp) {    // Replace the original node with lnodebp
                parent->children[i] = lnodebp;
                break;
            }
        }
    } else {    // newly created parent
        parent->children[parent->childrenSize++] = lnodebp;  // Insert the lnodebp
    }
    parent->children[parent->childrenSize++] = rnodebp;      // Insert the rnodebp

    // Sort the data and children of the parent
    qsort(parent->data, parent->size, sizeof(parent->data[0]), cmpData);
    qsort(parent->children, parent->childrenSize, sizeof(parent->children[0]), cmpNodeBP);

    free(nodebp); // Free the memory of the original node

    Tree = SplitBP(parent, Tree);  // Continue spliting the upper node

    return Tree;
}

// Print the B+ tree(level-order traversal)
// T: B+ tree containing the inverted index
void PrintBPTree(BplusTree T) {
    int i;
    NodeBP nodebp;        // The node obtained from the queue
    QueueBP q;            // The queue containing the nodes from B+ tree

    printf("B+ Tree of Inverted Index:\n");

    q = CreateQueueBP();  // Create an empty queue
    EnqueueBP(T, q);      // Put the root of the tree into the queue first
    EnqueueBP(NULL, q);   // Put the NULL pointer, for creation of newline

    while (q->size) {     // If the queue isn't empty, repeat the following steps
        nodebp = DequeueBP(q);  // Get the front node
        if (!nodebp) {          // If it's an NULL pointer, it's time to add a newline
            printf("\n");
            if (q->size) {      // If the queue isn't empty, continue add a new NULL pointer
                EnqueueBP(NULL, q);
            }
        } else {
            printf("[");        // Print the node's data(just the term)
            for (i = 0; i < nodebp->size; i++) {
                if (!i) {
                    printf("%s", nodebp->data[i]->term);
                } else {
                    printf(", %s", nodebp->data[i]->term);
                }
            }
            printf("]");
        }

        if (nodebp) {           // If nodebp isn't a NULL pointer, then put its children into the queue
            for (i = 0; i < nodebp->childrenSize; i++) {
                EnqueueBP(nodebp->children[i], q);
            }
        }
    }
}

// Create the queue
QueueBP CreateQueueBP() {
    QueueBP Q = (QueueBP)malloc(sizeof(struct queuebp));
    Q->size = 0;
    Q->front = Q->rear = 0;

    return Q;
}

// Put the node of B+ tree into the queue
// nodebp: the newly added node
// Q: the queue
void EnqueueBP(NodeBP nodebp, QueueBP Q) {
    if (Q->size >= SIZE) {  // If the queue is full, enqueue operation fails
        printf("Full B+-tree-item queue!\n");
        exit(1);
    }
    Q->data[Q->rear++] = nodebp;  // Add new node
    Q->size++;
}

// Get the front node and delete it from the queue
// Q: the queue
NodeBP DequeueBP(QueueBP Q) {
    if (!Q->size) {         // If the queue is empty, dequeue operation fails
        printf("Empty B+-tree-item queue!\n");
        exit(1);
    }
    NodeBP returnNodeBP = Q->data[Q->front++];  // Get the front node
    Q->size--;     // Delete the node from queue
    return returnNodeBP;
}

// Create the poslist
PosList CreatePL() {
    PosList L;

    L = (PosList)malloc(sizeof(struct poslist));
    L->size = 0;
    L->front = (PosData)malloc(sizeof(struct posdata));
    L->rear = (PosData)malloc(sizeof(struct posdata));
    L->front = L->rear;
    L->rear->pos = -1;   // Distinguish from other nodes

    return L;
}

// Add new position
// pos: the position
// L: the position list
void EnqueuePL(int pos, PosList L) {
    if (L->rear->pos != pos) {  // If it's a new position
        PosData tmp = (PosData)malloc(sizeof(struct posdata));
        if (!tmp) {
            printf("Fail to create a new position data!\n");
            exit(1);
        }  // Insert the new one in the position list
        tmp->pos = pos;
        tmp->time = 1;
        tmp->next = L->rear->next;
        L->rear->next = tmp;
        L->rear = tmp;
        L->size++;
    } else {    // Otherwise, just increment the frequency
        L->rear->time++;
    }

}

// Retrieve all position in the list
// L: the position list
int ** RetrievePL(PosList L) {
    if (!L->size) {      // If the list is empty, retrieve operation fails
        printf("Empty position-data queue!\n");
        exit(1);
    }

    int i = 0, j;
    int ** posArr = (int **)malloc(sizeof(int *) * L->size);
    for (j = 0; j < L->size; j++) {  // Memory Allocation for 2D array
        posArr[j] = (int *)malloc(sizeof(int) * 2);
    }

    PosData cur = L->front->next;

    while (cur != NULL) {  // Make a traversal in the position list
        posArr[i][0] = cur->pos;  // Get the specific info of the position
        posArr[i][1] = cur->time;
        cur = cur->next;
        i++;
    }

    return posArr;
}

// Build a hash table
HashTb GenerateHashTb() {
    int i;
    int pre, cur;                 // Mark the start and the end of one word
    HashTb H;                     // The hash table containing the stop words
    FILE * fp;                    // File pointer
    char fname[MAXWORDLEN];       // File name
    char tmp[MAXREADSTRLEN];      // Memory space storing the reading data temporarily
    char * term;                  // Term(or word)

    H = InitHashTb();             // Initialization

    strcpy(fname, STOPWORDPATH);  // Use default path(stop_words.txt)
    fp = fopen(fname, "r");       // Open the file
    if (!fp) { 
        printf("Fail to open the file of stopwords!\n");
        exit(1);
    }

    while (fgets(tmp, MAXREADSTRLEN - 1, fp) != NULL) {  // Continue reading the file, until arrive at the end of file
        pre = cur = 0;                             // Initialization
        for (i = 0; i < strlen(tmp); i++) {        // Retrieve all characters in the tmp string
            if (!isalpha(tmp[i])) {                // Maybe it's time to record a word
                cur = i;
                if (cur > pre) {                   // Legitimate situation
                    term = (char *)malloc(sizeof(char) * (cur - pre + 1));
                    strncpy(term, tmp + pre, cur - pre);
                    term[cur - pre] = '\0';
                    InsertHashSW(term, H);         // Insert the new term
                }

                pre = cur + 1;
            }
        }

        // Handle the last possible word in the tmp string
        if (!cur || pre > cur && pre != i) {
            cur = i;
            term = (char *)malloc(sizeof(char) * (cur - pre + 1));
            strncpy(term, tmp + pre, cur - pre);
            term[cur - pre] = '\0';
            InsertHashSW(term, H);                 // Insert the new term  
        }  
    }
    fclose(fp);
    return H;
}

// Initialization of the hash table
HashTb InitHashTb() {
    HashTb H;             // Hash table
    int i;

    H = (HashTb)malloc(sizeof(struct hashtb));  // Memory allocation for the whole table
    if (H == NULL) {
        printf("Fail to create a hash table for stopwords!\n");
        exit(1);
    }

    H->size = STOPWORDSUM;    // maxixum size
    
    for (i = 0; i < H->size; i++) {
        H->data[i] = (HashSW)malloc(sizeof(hashsw));
        if (H->data[i] == NULL) {    // Memory allocation for cells
            printf("Fail to create a hash table for stopwords!\n");
            exit(1);    
        }
        H->data[i]->stopword = (string)malloc(sizeof(char) * MAXWORDLEN);
        H->data[i]->info = Empty;
    }

    return H;
}

// Find the stopwords or other words in the hash table
// stopword: stop word
// H: hash table containing the stop words
// justSearch: find the term without subsequent insertion
int FindHashSW(string stopword, HashTb H, bool justSearch) {
    int pos;                              // Appropraite position
    int collisionNum = 0;                 // collision number, for quadratic probe
    pos = HashFunc(stopword, H->size);    // Use hashing function first

    // If we just search a term in the hash table and assure it's not a stop word, then return
    if (justSearch && (H->data[pos]->info == Empty || strcmp(H->data[pos]->stopword, stopword))) {
        return -1;
    }

    // Collision occurs!
    while (H->data[pos]->info != Empty && strcmp(H->data[pos]->stopword, stopword)) {
        pos += 2 * ++collisionNum - 1;  // Quadratic probe
        if (pos >= H->size)        
            pos -= H->size;     
    }
    return pos;  
}

// Insert a new stopword in hash table
// stopword: stop word
// H: hash table containing the stop words
void InsertHashSW(string stopword, HashTb H) {
    int pos;
    pos = FindHashSW(stopword, H, false);   // Find the correct position
    if (H->data[pos]->info != Legitimate)   // Insert the stop word
    {
        H->data[pos]->info = Legitimate;
        strcpy(H->data[pos]->stopword, stopword);
    }
}

// Hashing function
// stopword: stop word
// size: the maximum size of the hash table
int HashFunc(string stopword, int size) {
    unsigned int val = 0;
    while (*stopword != '\0')
        val = (val << 5) + *stopword++;    // Generate the hash value from every character in the string
    return val % size;
}

// Comparison functions used in qsort()
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

// wstring <-> char *, for word stemming
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

// Print the ticks and duration, for -tr or --time function
void PrintTime(clock_t start, clock_t end) {
    clock_t tick;       // ticks
    double duration;    // duration(unit: seconds)

    tick = end - start;
    duration = ((double)(tick)) / CLOCKS_PER_SEC;
    printf("Ticks: %lu\n", (long)tick);    // Print the info
    printf("Duration: %.2fs\n", duration);
}