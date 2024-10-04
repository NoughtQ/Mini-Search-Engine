#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <malloc.h>
#include "porter2_stemmer.h"
#include "search_word.h"

using namespace std;

void search_word(Table_Node *Hash_Table)                           //搜索功能主函数
{
    printf("Finish reading the file!\n");
    char operation[1000];
    memset(operation, '\0', 1000);
    printf("Here is the format of the search result:(type,article,act,scene,line)\n");
    printf("If you want to exit the program, please enter the word '--exit--'.\n");
    printf("If you want to clear the terminal, please enter the word '--clear--'.\n");
    printf("Please enter the word you want to search:\n");
    while(cin.getline(operation, 1000))
    {
        if(strcmp(operation, "--exit--") == 0)
        {
            return ;
        }
        else if (strcmp(operation, "--clear--") == 0)
        {
            system("clear");
            printf("Here is the format of the search result:(type,article,act,scene,line)\n");
            printf("If you want to exit the program, please enter the word '--exit--'.\n");
            printf("If you want to clear the terminal, please enter the word '--clear--'.\n");
            printf("Please enter the word you want to search:\n");
        }
        else
        {
            printf("\n");
            Search_function(Hash_Table , operation);
            printf("\n");
            printf("Here is the format of the search result:(type,article,act,scene,line)\n");
            printf("If you want to exit the program, please enter the word '--exit--'.\n");
            printf("If you want to clear the terminal, please enter the word '--clear--'.\n");
            printf("Please enter the word you want to search:\n");
        }
        memset(operation, '\0', 1000);
    }
    return ;
}

void Search_function(Table_Node * Hash_Table , char * operation)                      //搜索词句
{
    int ope_start_place = 0, t = 0;
    char temp[100];
    string word;
    Search_Word_List Search_List = NULL;
    while(operation[ope_start_place] != '\0')
    {
        while(operation[ope_start_place + t] != ' ' && operation[ope_start_place + t] != '\0' && operation[ope_start_place + t] != '\r' && operation[ope_start_place + t] != '\n')
        {
            temp[t] = operation[ope_start_place + t];
            t++;
        }
        ope_start_place += (t + 1);
        temp[t] = '\0';
        t = 0;
        word = temp;
        Porter2Stemmer::trim(word);
        Porter2Stemmer::stem(word);
        strcpy(temp, word.c_str());
        Search_List = make_Searching_List(Hash_Table , temp , Search_List);
        if(Search_List == NULL)
        {
            printf("Can't find the word. Please try again.\n");
            return ;
        }
    }
    Search_List = Search_List_Sort(Search_List);
    Search_List = Query_Threshold(Search_List);
    Search_Result_Printing(Search_List);
    clear_Search_List(Search_List);
    return ;
}

Search_Word_List make_Searching_List(Table_Node *Hash_Table , char *word , Search_Word_List Search_List)                           //创建搜索链表
{
    int hash_value = HASH(word);
    if(find_word_in_hashtable(word , &hash_value , Hash_Table))                         //若单词在Dictionary中
    {
        Search_Node new_node = (Search_Node)malloc(sizeof(struct Node_for_Search));     //创建新节点
        strcpy(new_node -> word , word);
        new_node -> frequency = Hash_Table[hash_value] -> frequency;
        new_node -> first_posting_list = Hash_Table[hash_value] -> posting_list;
        new_node -> last_posting_list = Hash_Table[hash_value] -> tail_list_node;
        new_node -> next = NULL;
        if(Search_List == NULL)
        {
            new_node -> prev = NULL;                                                    //前驱节点也为空

            Search_List = (Search_Word_List)malloc(sizeof(struct Searching_Word_List));
            Search_List -> head = new_node;
            Search_List -> tail = new_node;
        }
        else
        {
            new_node -> prev = Search_List -> tail;                                     //前驱节点为尾节点

            Search_List -> tail -> next = new_node;
            Search_List -> tail = new_node;
        }
        return Search_List;
    }
    else    return Search_List;
}

Search_Word_List Search_List_Sort(Search_Word_List Search_List)                   //对搜索链表排序
{
    if(Search_List == NULL || Search_List -> head == Search_List -> tail)               //若链表为空或只有一个节点
    {
        return Search_List;
    }
    Search_Node head_Node = Search_List -> head , tail_Node = Search_List -> tail;      //保存头尾节点，方便操作
    Search_Node start_Node = head_Node -> next , move_Node , temp_Node;                 //排序开始节点
    while(start_Node != NULL)
    {
        move_Node = start_Node;                                                         //从start_Node节点开始进行冒泡排序
        while(move_Node ->prev != NULL)
        {
            temp_Node = move_Node -> prev;                                              //对前驱节点进行比较
            if(move_Node -> frequency < temp_Node -> frequency)
            {
                if(move_Node == start_Node)
                {
                    start_Node = temp_Node;
                }
                if(move_Node == tail_Node)
                {
                    tail_Node = temp_Node;
                }
                else if(temp_Node == head_Node)                                         //遇到特殊情况，先将start_Node,tail_Node或者head_Node更新
                {
                    head_Node = move_Node;
                }
                move_Node -> prev = temp_Node -> prev;
                temp_Node -> prev = move_Node;
                temp_Node -> next = move_Node -> next;
                move_Node -> next = temp_Node;
            }
            else    break;
        }
        start_Node = start_Node -> next;
    }
    Search_List -> head = head_Node;
    Search_List -> tail = tail_Node;                                                    //更新头尾节点
    return Search_List;
}

Search_Word_List Query_Threshold(Search_Word_List Search_List)                    //根据阈值进行筛选
{
    if(Search_List == NULL)
    {
        return NULL;
    }
    int all_frequency = 0 , num_word = 0 , temp_frequency = 0;
    float thershold = 0 , ans = 0;
    Search_Node temp_Node = Search_List -> head , next_Node;
    while(temp_Node != NULL)                                                            //计算总词频和总词数
    {
        all_frequency += temp_Node -> frequency;
        temp_Node = temp_Node -> next;
        ++ num_word;
    }
    if(num_word <= 3)                                                                   //若总词数小于等于3，则不进行阈值筛选
    {
        return Search_List;
    }
    else if(num_word <= 8)                                                              //若总词数小于等于8，则阈值为0.5
    {
        thershold = 0.8;
    }
    else
    {
        thershold = 0.7;
    }
    temp_Node = Search_List -> head;
    temp_frequency = temp_Node -> frequency;
    ans = (float)temp_frequency / (float)all_frequency;
    while(ans <= thershold)                                                             //找到第一个大于阈值的节点
    {
        temp_Node = temp_Node -> next;
        temp_frequency += temp_Node -> frequency;
        ans = (float)temp_frequency / (float)all_frequency;
    }
    if(temp_Node == NULL)
    {
        printf("ERROR: in Query_Threshold function!\n");
        return NULL;
    }
    temp_Node -> prev -> next = NULL;
    Search_List -> tail = temp_Node -> prev;                                            //更新链表
    while(temp_Node != NULL)                                                            //将多余的点全部删除
    {
        next_Node = temp_Node -> next;
        free(temp_Node);
        temp_Node = next_Node;
    }
    return Search_List;
}

void Search_Result_Printing(Search_Word_List Search_List)                            //结果输出函数
{
    if(Search_List == NULL)
    {
        printf("ERROR: in Search_Result_Printing function!\n");
        return ;
    }
    Search_Node first_Node = Search_List -> head , Second_Node = first_Node -> next , temp_Node;
    List_Node target_index = first_Node -> first_posting_list , temp_index;
    int Search_flag = 0 , Result_flag = 0;
    printf("The result of searching is:\n");
    if(Second_Node == NULL)
    {
        print_single_posting_list(first_Node);
        return;
    }
    while(target_index != NULL)
    {
        temp_Node = Second_Node;
        while(temp_Node != NULL)
        {
            Search_flag = 0;
            temp_index = temp_Node -> first_posting_list;
            while(temp_index != NULL)
            {
                if(Result_Compare(target_index , temp_index))
                {
                    Search_flag = 1;
                    break;
                }
                temp_index = temp_index -> next;
            }
            if(Search_flag == 0)    break;
            temp_Node = temp_Node -> next;
        }
        if(temp_Node == NULL && Search_flag == 1)
        {
            printf("(%d,%d,%d,%d,%d) " , target_index -> index_type , target_index -> index_article , target_index -> index_act , target_index -> index_scene , target_index -> index_line);
            Result_flag = 1;
        }
        target_index = target_index -> next;
    }
    if(Result_flag == 0)
    {
        printf("Not find any result!\n");
    }
    else    printf("\n");
    return ;
}

int Result_Compare(List_Node node1 , List_Node node2)                                   //结果比较函数
{
    if(node1 -> index_type != node2 -> index_type)
    {
        return 0;
    }
    if(node1 -> index_article != node2 -> index_article)
    {
        return 0;
    }
    if(node1 -> index_act != node2 -> index_act)
    {
        return 0;
    }
    if(node1 -> index_scene != node2 -> index_scene)
    {
        return 0;
    }
    if(node1 -> index_line != node2 -> index_line)
    {
        return 0;
    }
    return 1;
}

void print_single_posting_list(Search_Node word_index)                                  //打印单个Posting List
{
    if(word_index == NULL)
    {
        printf("ERROR: in print_single_posting_list function!\n");
        return ;
    }
    List_Node temp_index = word_index -> first_posting_list;
    while(temp_index != NULL)
    {
        printf("(%d,%d,%d,%d,%d) " , temp_index -> index_type , temp_index -> index_article , temp_index -> index_act , temp_index -> index_scene , temp_index -> index_line);
        temp_index = temp_index -> next;
    }
    printf("\n");
    return ;
}

void clear_Search_List(Search_Word_List Search_List)                                                                    //清空搜索链表
{
    Search_Node temp_Node = Search_List -> head , next_Node;
    while(temp_Node != NULL)
    {
        next_Node = temp_Node -> next;
        free(temp_Node);
        temp_Node = next_Node;
    }
    free(Search_List);
    return ;
}