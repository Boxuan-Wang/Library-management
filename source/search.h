/*检索模块
*包括功能，
*1. 对书名的模糊搜索
*2. 对关键词的模糊搜索
*3. 根据编号的明确检索
*4. 根据作者的模糊搜索
*/

#pragma once
#ifndef __searchbook_h__
#define __searchbook_h__

#include"bookmanagement.h"
#include "filesource.h"

/*对书名的模糊搜索
*返回搜索结果的链表头指针
*/
struct book_node* search_name( char* word_name);

/*对关键词的模糊搜索
*将 MAX_search_results 个相关度最高的图书的结构指针写入result中
*并返回该指针
*/
struct book_node* search_keyword(char * word_keyword);

/*对作者的模糊搜索
*将 MAX_search_results 个相关度最高的图书的结构指针写入result中
*并返回该指针
*/
struct book_node* search_author(char * word_author);

/*对编号的明确检索
**/
Book* find_num(int num);

/*对字符串进行长度限制
*裁剪字符串，末尾为省略号
*/
char * cut_string(char * source, int len_limit);
#endif 
