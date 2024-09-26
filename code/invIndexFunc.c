// Implementation of methods related to B+ tree in invIndex.h
#include "invIndexHeader.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


BplusTree CreateBP() {
    BplusTree T = (BplusTree)malloc(sizeof(struct nodebp));
    if (T == NULL) {
        printf("Failed to create a B+ Tree!\n");
        return T;
    }

    int i;
    // T->term = (int *)malloc(sizeof(int) * (ORDER + 1));
    // for (i = 0; i <= ORDER; i++) {
    //     T->term[i] = (string)malloc(sizeof(char) * MAXWORDLEN);
    // }
    
    for (i = 0; i <= ORDER; i++) {
        // T->docpos[i] = (DocList)malloc(sizeof(struct doclist));
        // T->docpos[i]->pos = -1;
        // T->docpos[i]->next = NULL;
        T->time[i] = 0;
        T->children[i] = (NodeBP)malloc(sizeof(struct nodebp));
    }

    T->size = 0;
    T->childrenSize = 0;
    T->parent = NULL;

    return T;
}

NodeBP FindBP(string x, BplusTree T, bool * flag) {
    int i;

    if (!T) {
        return T;
    } else if (!T->childrenSize) {
        isSameTerm(x, T, flag);
        return T;
    }


    int pos = -1;
    
    for (i = 0; i < T->size; i++) {
        if (strcmp(x, T->term[i]) < 0) {
            pos = i;
            break;
        }
    }
    if (pos == -1) {
        pos = i;
    }

    return FindBP(x, T->children[pos], flag);
}

void isSameTerm(string term, NodeBP nodebp, bool * flag) {
    int i;
    // DocList tmp;

    if (nodebp->size) {
        for (i = 0; i < nodebp->size; i++) {
            if (!strcmp(term, nodebp->term[i])) {
                nodebp->time[i]++;
                
                // tmp = (DocList)malloc(sizeof(struct doclist));
                // tmp->pos = 

                *flag = true;
                break;
            }
        }
    }

}

void InsertBP(string x, NodeBP nodebp, BplusTree Tree) {
    int i;

    strcpy(nodebp->term[nodebp->size++], x);
    qsort(nodebp->term, nodebp->size, sizeof(char) * MAXWORDLEN, cmpData);

    SplitBP(nodebp, Tree);
}

void SplitBP(NodeBP nodebp, BplusTree Tree) {
    if (!nodebp->childrenSize && nodebp->size <= ORDER 
      || nodebp->childrenSize && nodebp->size < ORDER) {
        return;
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
            strcpy(lnodebp->term[i], nodebp->term[i]);
        }
        lnodebp->size = cut;

        for (j = cut; j < nodebp->size; j++) {
            strcpy(rnodebp->term[j - cut], nodebp->term[j]);
        }
        rnodebp->size = nodebp->size - cut;

    } else {
        cut = NONLEAFCUT;

        for (i = 0; i <= cut; i++) {
            if (i != cut) 
                strcpy(lnodebp->term[i], nodebp->term[i]);
            lnodebp->children[i] = nodebp->children[i];
            lnodebp->children[i]->parent = lnodebp;
        }
        lnodebp->size = cut;
        lnodebp->childrenSize = cut + 1;

        for (j = cut + 1; j < nodebp->size; j++) {
            strcpy(rnodebp->term[j - cut - 1], nodebp->term[j]);
        }
        for (j = cut + 1; j < nodebp->childrenSize; j++) {
            rnodebp->children[j - cut - 1] = nodebp->children[j];
            rnodebp->children[j - cut - 1]->parent = rnodebp;
        }     
        rnodebp->size = nodebp->size - cut - 1;
        rnodebp->childrenSize = nodebp->childrenSize - cut - 1;
    }

    strcpy(parent->term[parent->size++], nodebp->term[cut]);
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

    qsort(parent->term, parent->size, sizeof(parent->term[0]), cmpData);
    qsort(parent->children, parent->childrenSize, sizeof(parent->children[0]), cmpNodeBP);

    free(nodebp);

    SplitBP(parent, Tree);
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
                    printf("%s", nodebp->term[i]);
                } else {
                    printf(", %s", nodebp->term[i]);
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
    Q->term[Q->rear++] = nodebp;
    Q->size++;
}

NodeBP DequeueBP(QueueBP Q) {
    NodeBP returnNodeBP = Q->term[Q->front++];
    Q->size--;
    return returnNodeBP;
}

int cmpData(const void * a, const void * b) {
    const string termA = *(const string*)a;
    const string termB = *(const string*)b;

    return strcmp(termA, termB);
}

int cmpNodeBP(const void * a, const void * b) {
    const NodeBP nodebpA = *(const NodeBP*)a;
    const NodeBP nodebpB = *(const NodeBP*)b;

    return nodebpA->term[0] - nodebpB->term[0];
}