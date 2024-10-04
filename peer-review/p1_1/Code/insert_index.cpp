#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <malloc.h>
#include "porter2_stemmer.h"
#include "insert_index.h"

void print_posting_list(Table_Node *Hash_Table , int *word_count , int *max_hash_value)
{
    for(int i = 0 ; i < HASH_TABLE_SIZE ; ++ i)                    //遍历哈希表
    {
        Table_Node temp_Node = Hash_Table[i];
        if(temp_Node != NULL)
        {
            List_Node temp_List_Node = temp_Node -> posting_list;
            printf("Word: %s , hash_value: %d , frequency: %d " , temp_Node -> WORD , i , temp_Node -> frequency);
            while(temp_List_Node != NULL)
            {
                printf("-> (%d,%d,%d,%d,%d) " , temp_List_Node -> index_type , temp_List_Node -> index_article , temp_List_Node -> index_act , temp_List_Node -> index_scene , temp_List_Node -> index_line);
                temp_List_Node = temp_List_Node -> next;
            }
            printf("\n");
        }
    }
    printf("The number of words is: %d\n" , *word_count);
    printf("The maximum hash value is: %d\n" , *max_hash_value);
    return ;
}

void print_highfrequency_word(Table_Node *Hash_Table)                //打印高频词(测试用)
{
    int thershold , word_number = 0;
    printf("Please input the frequency threshold: ");
    scanf("%d" , &thershold);
    for(int i = 0 ; i < HASH_TABLE_SIZE ; ++ i)
    {
        Table_Node temp_Node = Hash_Table[i];
        if(temp_Node != NULL && temp_Node -> frequency >= thershold)
        {
            printf("Word: %s , frequency: %d \n" , temp_Node -> WORD , temp_Node -> frequency);
            ++ word_number;
        }
    }
    printf("Total number of words: %d\n" , word_number);
    return ;
}

void read_file(Table_Node *Hash_Table , int *word_count , int *max_hash_value)      //读入文件
{
    string word;
    char orig[2500] , temp[100] , filename[100];
    int t , orig_start_place , hash_value ,act_flag = 0 , scene_flag = 0 ,
        type_number = 0 , article_number = 0 , act_number = 0 , scene_number = 0 , line_number = 0;
    ifstream script;
    for(int i = 1 ; i <= TXT_NUMBER ; ++ i)
    {
        choose_file(filename , &type_number , &article_number);     //依次选择读入的文件，同时更新type和article
        script.open(filename);                                      //读文件
        act_number = scene_number = line_number = 0;
        while (script.getline(orig, 2500))                           //逐行读入
        {
            ++ line_number;
            t = orig_start_place = 0;
            while(orig[orig_start_place] != '\0')                   //逐字读入
            {
                while(orig[orig_start_place + t] != ' ' && orig[orig_start_place + t] != '\r' && orig[orig_start_place + t] != '\n' && orig[orig_start_place + t] != '\0'
                      && orig[orig_start_place + t] != '.' && orig[orig_start_place + t] != ',' && orig[orig_start_place + t] != ';' && orig[orig_start_place + t] != ':' && orig[orig_start_place + t] != '!' && orig[orig_start_place + t] != '?' && orig[orig_start_place + t] != '-' && orig[orig_start_place + t] != '\'')//将该单词逐字母复制给temp
                    {
                        temp[t] = orig[orig_start_place + t];
                        t++;
                    }
                orig_start_place += (t + 1);
                temp[t] = '\0';
                t = 0;                                                  //修改位置，便于下一次读写
                if(strcmp(temp , "") == 0)                              //若遇到空格或换行符则跳过
                {
                    continue;
                }
                else if(strcmp(temp, "ACT") == 0)                       //若遇到ACT或SCENE则判断是否变成新的一章
                {
                    act_flag = 1;
                }
                else if(strcmp(temp, "SCENE") == 0)
                {
                    scene_flag = 1;
                }
                else if(isRomanNumDot(temp) || isRomanNumNodot(temp))  //若是，则更新act或scene的编号
                        {
                            if(act_flag == 1)
                            {
                                ++ act_number;
                                scene_number = 0;
                                act_flag = 0;
                            }
                            else if(scene_flag == 1)
                            {
                                ++ scene_number;
                                scene_flag = 0;
                            }
                        }
                else
                {
                    act_flag = 0;
                    scene_flag = 0;                                     //复原标志位
                    word = temp;                                        //将temp转换为string类型
                    Porter2Stemmer::trim(word);
                    Porter2Stemmer::stem(word);
                    strcpy(temp, word.c_str());                         //将string类型转换成char类型，方便操作
                    if(strlen(temp) == 1 || isStopWords(temp))                               //若是停用词则跳过
                    {
                        continue;
                    }
                    hash_value = HASH(temp);                            //计算hash值
                    if(find_word_in_hashtable(temp , &hash_value , Hash_Table))      //若单词在Dictionary中，插入新的索引
                    {
                        insert_Posting_List(Hash_Table , hash_value , type_number , article_number , act_number , scene_number , line_number);   //插入索引
                    }
                    else
                    {
                        make_table_node(Hash_Table , hash_value , temp);        //建立新的单词节点
                        insert_Posting_List(Hash_Table , hash_value , type_number , article_number , act_number , scene_number , line_number);   //插入索引
                        ++ *word_count;
                        if(*max_hash_value < hash_value)     *max_hash_value = hash_value;
                    }
                }
            }
        }
        script.close();
    }
    return ;
}

int HASH(char *word)                                        //hash函数
{
    ULL p = 31 , hash_value = 0 , len = strlen(word);
    for(int i = 0 ; i <= len ; ++ i)
    {
        hash_value = hash_value * p + word[i];
    }
    return (int)(hash_value % (MAX_HASH_VALUE + 1));
}

int find_word_in_hashtable(char *word , int *hash_value , Table_Node *Hash_Table)    //查找单词是否在Dictionary中
{
    while(Hash_Table[*hash_value] != NULL)                              //若没找到且还可以继续找，则一直搜索下去
    {
        if(strcmp(Hash_Table[*hash_value] -> WORD , word) == 0)         //找到了
        {
            return 1;
        }
        *hash_value += MAX_HASH_VALUE;                                  //继续找下一个位置
    }
    return 0;
}

void make_table_node(Table_Node * Hash_Table , int hash_value , char *word)             //建立新的单词节点
{
    Table_Node new_node = (Table_Node)malloc(sizeof(struct HashTableNode));
    new_node -> WORD = (char*)malloc(sizeof(char) * (strlen(word) + 1));
    strcpy(new_node -> WORD , word);
    new_node -> frequency = 0;
    new_node -> posting_list = NULL;
    new_node -> tail_list_node = NULL;
    Hash_Table[hash_value] = new_node;
    return ;
}

void insert_Posting_List(Table_Node *Hash_Table , int hash_value , int type , int article , int act , int scene , int line) //插入新的索引
{
    List_Node new_node = (List_Node)malloc(sizeof(struct PostingList_Node));
    new_node -> index_type = type;
    new_node -> index_article = article;
    new_node -> index_act = act;
    new_node -> index_scene = scene;
    new_node -> index_line = line;
    new_node -> next = NULL;
    Table_Node word_node = Hash_Table[hash_value];            //找到单词节点
    ++ word_node -> frequency;                                //更新词频
    if(word_node -> posting_list == NULL)                     //若链表为空
    {
        word_node -> posting_list = new_node;
        word_node -> tail_list_node = new_node;
    }
    else                                                      //否则直接改变尾节点
    {
        word_node -> tail_list_node -> next = new_node;
        word_node -> tail_list_node = new_node;
    }
    return ;
}

void clear_hash_table(Table_Node * Hash_Table)                     //清空哈希表
{
    Table_Node temp_Node;
    for(int i = 0 ; i < HASH_TABLE_SIZE ; ++ i)                    //遍历哈希表
    {
        temp_Node = Hash_Table[i];
        if(temp_Node != NULL)                                      //若节点不为空
        {
            List_Node temp_List_Node = temp_Node -> posting_list , next_List_Node;
            do
            {
                next_List_Node = temp_List_Node -> next;
                free(temp_List_Node);
                temp_List_Node = next_List_Node;
            } while (temp_List_Node != NULL);                       //释放Posting List
            free(temp_Node -> WORD);
            free(temp_Node);                                        //释放单词节点
        }
    }
    return ;
}

void choose_file(char *filename , int *type_number , int *article_number)                //选择文件
{
    static int flag = 0;
    ++ flag;
    switch (flag)
    {
        case 1:
            strcpy(filename, ".\\txt_source\\script1.txt");
            *type_number = 1;
            *article_number = 1;
            break;
        case 2:
            strcpy(filename, ".\\txt_source\\script2.txt");
            *type_number = 1;
            *article_number = 2;
            break;
        case 3:
            strcpy(filename, ".\\txt_source\\script3.txt");
            *type_number = 1;
            *article_number = 3;
            break;
        case 4:
            strcpy(filename, ".\\txt_source\\script4.txt");
            *type_number = 1;
            *article_number = 4;
            break;
        case 5:
            strcpy(filename, ".\\txt_source\\script5.txt");
            *type_number = 1;
            *article_number = 5;
            break;
        case 6:
            strcpy(filename, ".\\txt_source\\script6.txt");
            *type_number = 1;
            *article_number = 6;
            break;
        case 7:
            strcpy(filename, ".\\txt_source\\script7.txt");
            *type_number = 1;
            *article_number = 7;
            break;
        case 8:
            strcpy(filename, ".\\txt_source\\script8.txt");
            *type_number = 1;
            *article_number = 8;
            break;
        case 9:
            strcpy(filename, ".\\txt_source\\script9.txt");
            *type_number = 1;
            *article_number = 9;
            break;
        case 10:
            strcpy(filename, ".\\txt_source\\script10.txt");
            *type_number = 1;
            *article_number = 10;
            break;
        case 11:
            strcpy(filename, ".\\txt_source\\script11.txt");
            *type_number = 1;
            *article_number = 11;
            break;
        case 12:
            strcpy(filename, ".\\txt_source\\script12.txt");
            *type_number = 1;
            *article_number = 12;
            break;
        case 13:
            strcpy(filename, ".\\txt_source\\script13.txt");
            *type_number = 1;
            *article_number = 13;
            break;
        case 14:
            strcpy(filename, ".\\txt_source\\script14.txt");
            *type_number = 1;
            *article_number = 14;
            break;
        case 15:
            strcpy(filename, ".\\txt_source\\script15.txt");
            *type_number = 1;
            *article_number = 15;
            break;
        case 16:
            strcpy(filename, ".\\txt_source\\script16.txt");
            *type_number = 1;
            *article_number = 16;
            break;
        case 17:
            strcpy(filename, ".\\txt_source\\script17.txt");
            *type_number = 1;
            *article_number = 17;
            break;
        case 18:
            strcpy(filename, ".\\txt_source\\script18.txt");
            *type_number = 2;
            *article_number = 1;
            break;
        case 19:
            strcpy(filename, ".\\txt_source\\script19.txt");
            *type_number = 2;
            *article_number = 2;
            break;
        case 20:
            strcpy(filename, ".\\txt_source\\script20.txt");
            *type_number = 2;
            *article_number = 3;
            break;
        case 21:
            strcpy(filename, ".\\txt_source\\script21.txt");
            *type_number = 2;
            *article_number = 4;
            break;
        case 22:
            strcpy(filename, ".\\txt_source\\script22.txt");
            *type_number = 2;
            *article_number = 5;
            break;
        case 23:
            strcpy(filename, ".\\txt_source\\script23.txt");
            *type_number = 2;
            *article_number = 6;
            break;
        case 24:
            strcpy(filename, ".\\txt_source\\script24.txt");
            *type_number = 2;
            *article_number = 7;
            break;
        case 25:
            strcpy(filename, ".\\txt_source\\script25.txt");
            *type_number = 2;
            *article_number = 8;
            break;
        case 26:
            strcpy(filename, ".\\txt_source\\script26.txt");
            *type_number = 2;
            *article_number = 9;
            break;
        case 27:
            strcpy(filename, ".\\txt_source\\script27.txt");
            *type_number = 2;
            *article_number = 10;
            break;
        case 28:
            strcpy(filename, ".\\txt_source\\script28.txt");
            *type_number = 3;
            *article_number = 1;
            break;
        case 29:
            strcpy(filename, ".\\txt_source\\script29.txt");
            *type_number = 3;
            *article_number = 2;
            break;
        case 30:
            strcpy(filename, ".\\txt_source\\script30.txt");
            *type_number = 3;
            *article_number = 3;
            break;
        case 31:
            strcpy(filename, ".\\txt_source\\script31.txt");
            *type_number = 3;
            *article_number = 4;
            break;
        case 32:
            strcpy(filename, ".\\txt_source\\script32.txt");
            *type_number = 3;
            *article_number = 5;
            break;
        case 33:
            strcpy(filename, ".\\txt_source\\script33.txt");
            *type_number = 3;
            *article_number = 6;
            break;
        case 34:
            strcpy(filename, ".\\txt_source\\script34.txt");
            *type_number = 3;
            *article_number = 7;
            break;
        case 35:
            strcpy(filename, ".\\txt_source\\script35.txt");
            *type_number = 3;
            *article_number = 8;
            break;
        case 36:
            strcpy(filename, ".\\txt_source\\script36.txt");
            *type_number = 3;
            *article_number = 9;
            break;
        case 37:
            strcpy(filename, ".\\txt_source\\script37.txt");
            *type_number = 3;
            *article_number = 10;
            break;
        case 38:
            strcpy(filename, ".\\txt_source\\script38.txt");
            *type_number = 4;
            *article_number = 1;
            break;
        case 39:
            strcpy(filename, ".\\txt_source\\script39.txt");
            *type_number = 4;
            *article_number = 2;
            break;
        case 40:
            strcpy(filename, ".\\txt_source\\script40.txt");
            *type_number = 4;
            *article_number = 3;
            break;
        case 41:
            strcpy(filename, ".\\txt_source\\script41.txt");
            *type_number = 4;
            *article_number = 4;
            break;
        case 42:
            strcpy(filename, ".\\txt_source\\script42.txt");
            *type_number = 4;
            *article_number = 5;
            break;
        default:
            return ;
    }
    return ;
}