#ifndef INSERT_INDEX_H

#define INSERT_INDEX_H

#define HASH_TABLE_SIZE 500000              //哈希表大小
#define MAX_HASH_VALUE 50000                //最大哈希值
#define TXT_NUMBER 42                       //读入文件数量
#define ULL unsigned long long int

#define isRomanNumDot(x)    (strcmp((x), "I.") == 0 || strcmp((x), "II.") == 0 || strcmp((x), "III.") == 0 || strcmp((x), "IV.") == 0 || strcmp((x), "V.") == 0 || strcmp((x), "VI.") == 0 || strcmp((x), "VII.") == 0 || strcmp((x), "VIII.") == 0)
#define isRomanNumNodot(x)  (strcmp((x), "I") == 0 || strcmp((x), "II") == 0 || strcmp((x), "III") == 0 || strcmp((x), "IV") == 0 || strcmp((x), "V") == 0 || strcmp((x), "VI") == 0 || strcmp((x), "VII") == 0 || strcmp((x), "VIII") == 0)                 //判断是否为罗马数字

#define isArticle(x)        (strcmp((x), "a") == 0 || strcmp((x), "an") == 0 || strcmp((x), "the") == 0)                 //判断是否为冠词
#define isPreposition(x)    (strcmp((x), "in") == 0 || strcmp((x), "on") == 0 || strcmp((x), "at") == 0 || strcmp((x), "to") == 0 || strcmp((x), "by") == 0 || strcmp((x), "with") == 0 || strcmp((x), "for") == 0 || strcmp((x), "of") == 0)         //判断是否为介词
#define isConjunction(x)    (strcmp((x), "and") == 0 || strcmp((x), "or") == 0 || strcmp((x), "but") == 0 || strcmp((x), "not") == 0)                 //判断是否为连词
#define isPronous1(x)       (strcmp((x), "he") == 0 || strcmp((x), "she") == 0 || strcmp((x), "it") == 0 || strcmp((x), "him") == 0 || strcmp((x), "her") == 0 || strcmp((x), "its") == 0 || strcmp((x), "his") == 0 || strcmp((x), "hers") == 0 || strcmp((x) , "i") == 0 || strcmp((x) , "you") == 0 || strcmp((x) , "we") == 0 || strcmp((x) , "they") == 0 || strcmp((x), "your") == 0)
#define isPronoun2(x)       (strcmp((x) , "us") == 0 || strcmp((x) , "them") == 0 || strcmp((x) , "our") == 0 || strcmp((x) , "their") == 0 || strcmp((x) , "mine") == 0 || strcmp((x) , "yours") == 0 || strcmp((x) , "ours") == 0 || strcmp((x), "theirs") == 0 || strcmp((x), "me") == 0 || strcmp((x), "my") == 0 || strcmp((x), "that") == 0 || strcmp((x), "this") == 0 || strcmp((x), "these") == 0 || strcmp((x), "those") == 0)        //判断是否为代词
#define isAuxiliary(x)      (strcmp((x), "be") == 0 || strcmp((x), "do") == 0 || strcmp((x), "is") == 0 || strcmp((x), "are") == 0 || strcmp((x), "am") == 0 || strcmp((x), "doe") == 0)        //判断是否为助词
#define isStopWords(x)      (isArticle(x) || isPreposition(x) || isConjunction(x) || isPronous1(x) || isPronoun2(x) || isAuxiliary(x))        //判断是否为停用词

using namespace std;

typedef struct PostingList_Node* List_Node;
struct PostingList_Node         //Posting List节点
{
    int index_type;             //类型
    int index_article;          //文章编号
    int index_act;              //act编号
    int index_scene;            //scene编号
    int index_line;             //行号
    List_Node next;             //下一个节点
};
typedef struct HashTableNode* Table_Node;                   //哈希表结构
struct HashTableNode
{
    char *WORD;                 //单词
    int frequency;              //词频
    List_Node posting_list;     //链表
    List_Node tail_list_node;   //尾节点(便于插入)
};

void choose_file(char *filename , int *type_number , int *article_number);                  //选择文件
void read_file(Table_Node *Hash_Table , int *word_count , int *max_hash_value);                                                     //读文件
int HASH(char *word);                                                                       //hash函数
int find_word_in_hashtable(char *word , int *hash_value , Table_Node *Hash_Table);          //查找单词是否在Dictionary中
void make_table_node(Table_Node *Hash_Table , int hash_value , char *word);                 //建立新的单词节点
void insert_Posting_List(Table_Node *Hash_Table , int hash_value , int type , int article , int act , int scene , int line);             //插入新的索引
void clear_hash_table(Table_Node *Hash_Table);                                              //清空哈希表

void print_posting_list(Table_Node *Hash_Table , int *word_count , int *max_hash_value);                                            //打印Posting List(测试用)
void print_highfrequency_word(Table_Node *Hash_Table);                                      //打印高频词(测试用)

#endif