#include <iostream>
#include <cstring>
#include "insert_index.h"
#include "search_word.h"

using namespace std;
Table_Node Hash_Table[HASH_TABLE_SIZE];             //哈希表
int main()
{
    int word_count = 0;                             //单词数量
    int max_hash_value = 0;                         //最大哈希值

    printf("Welcome to the Dictionary Search System!\n");
    printf("The file is reading...\n");
    memset(Hash_Table, 0, sizeof(Table_Node) * HASH_TABLE_SIZE);
    read_file(Hash_Table , &word_count, &max_hash_value);
    //print_posting_list(Hash_Table , &word_count, &max_hash_value);
    //print_highfrequency_word(Hash_Table);
    search_word(Hash_Table);
    clear_hash_table(Hash_Table);                    //释放内存
    return 0;
}