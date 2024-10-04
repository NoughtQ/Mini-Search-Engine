#ifndef SEARCH_WORD_H

#define SEARCH_WORD_H

#include <iostream>
#include "porter2_stemmer.h"
#include "insert_index.h"

typedef struct Node_for_Search* Search_Node;
struct Node_for_Search
{
    char word[100];
    int frequency;
    List_Node first_posting_list;
    List_Node last_posting_list;
    Search_Node next , prev;
};
typedef struct Searching_Word_List* Search_Word_List;
struct Searching_Word_List
{
    Search_Node head;
    Search_Node tail;
};

void search_word(Table_Node *Hash_Table);                                                                               //搜索功能主体
void Search_function(Table_Node *Hash_Table , char *operation);                                                         //搜索主函数
Search_Word_List make_Searching_List(Table_Node *Hash_Table , char *word , Search_Word_List search_List);               //创建搜索链表
Search_Word_List Search_List_Sort(Search_Word_List Search_List);                                                        //对搜索链表排序
Search_Word_List Query_Threshold(Search_Word_List Search_List);                                                         //根据阈值进行筛选
void Search_Result_Printing(Search_Word_List Search_List);                                                              //结果输出函数
int Result_Compare(List_Node node1 , List_Node node2);                                                                  //结果比较函数
void print_single_posting_list(Search_Node word_index);                                                                 //打印单个Posting List
void clear_Search_List(Search_Word_List Search_List);                                                                   //清空搜索链表

#endif