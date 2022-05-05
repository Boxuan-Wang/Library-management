#include "users.h"
#include"lendrecord.h"
#include"bookmanagement.h"
#include"filesource.h"
#include"search.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

static struct book_node* book_list_head = NULL;
/*搜索方式
*依次查找包含搜索关键字符串子串的书籍
*链表中，靠前的是有相同字符较多的结果，
*即结果按相关度递减排序
*/
struct book_node * search_name(char * word_name)
{

		book_list_head = read_BookInfo();

	struct book_node * search_result_head = NULL;
	struct book_node * search_result_tail = NULL;
	struct book_node * p_search_node = NULL;
	struct book_node * p_book_node = book_list_head;

	int keyword_len = strlen(word_name);
	char cur_keyword[NAME_LEN_MAX];
	//搜索的字符串长度递减
	for (int len = keyword_len; len > 0; len--) {
		//字符串开始的位置顺延
		for (int start_location = 0; start_location < keyword_len - len + 1; start_location++) {
			//获得本次的搜索关键字符串
			for (int i = 0; i < len; i++) {
				cur_keyword[i] = *(word_name + start_location + i);
			}
			cur_keyword[len] = '\0';
			//在bookfile链表中顺序查找，找到且前面没有则添加到搜索结果
			for (p_book_node = book_list_head; p_book_node != NULL; p_book_node = p_book_node->next) {
				int found = 0;
				for (struct book_node * p = search_result_head; p != NULL; p = p->next) {
					if (p->book.BookNO == p_book_node->book.BookNO) {
						found = 1;
						break;
					}
				}
				if (found == 1) continue;

				if (strstr(p_book_node->book.name, cur_keyword)) {
	
					p_search_node = (struct book_node*)malloc(sizeof(struct book_node));
					p_search_node->book = p_book_node->book;
					p_search_node->next = NULL;

					if (search_result_head == NULL) {
						search_result_head = p_search_node;
						p_search_node->prev = NULL;
					}
					else {
						search_result_tail->next = p_search_node;
						p_search_node->prev = search_result_tail;
					}
					search_result_tail = p_search_node;
				}
			}
		
		}
	}
	for (struct book_node* p = book_list_head; p != NULL; ) {
		if (p->next == NULL) {
			free(p);
			break;
		}
		else {
			p = p->next;
			free(p->prev);
		}
	}
	book_list_head = NULL;
	return search_result_head;
}

struct book_node * search_keyword(char * word_keyword)
{

		book_list_head = read_BookInfo();

	struct book_node * search_result_head = NULL;
	struct book_node * search_result_tail = NULL;
	struct book_node * p_search_node = NULL;
	struct book_node * p_book_node = book_list_head;

	int keyword_len = strlen(word_keyword);
	char cur_keyword[NAME_LEN_MAX];
	//搜索的字符串长度递减
	for (int len = keyword_len; len > 0; len--) {
		//字符串开始的位置顺延
		for (int start_location = 0; start_location < keyword_len - len + 1; start_location++) {
			//获得本次的搜索关键字符串
			for (int i = 0; i < len; i++) {
				cur_keyword[i] = *(word_keyword + start_location + i);
			}
			cur_keyword[len] = '\0';
			//在bookfile链表中顺序查找，找到且前面没有则添加到搜索结果
			for (p_book_node = book_list_head; p_book_node != NULL; p_book_node = p_book_node->next) {
				int found = 0;
				for (struct book_node * p = search_result_head; p != NULL; p = p->next) {
					if (p->book.BookNO == p_book_node->book.BookNO) {
						found = 1;
						break;
					}
				}
				if (found == 1) continue;
				if (strstr(p_book_node->book.keyword[0], cur_keyword)||
					strstr(p_book_node->book.keyword[1], cur_keyword)||
					strstr(p_book_node->book.keyword[2], cur_keyword)||
					strstr(p_book_node->book.keyword[3], cur_keyword)||
					strstr(p_book_node->book.keyword[4], cur_keyword)) 
				{

					p_search_node = (struct book_node*)malloc(sizeof(struct book_node));
					p_search_node->book = p_book_node->book;
					p_search_node->next = NULL;

					if (search_result_head == NULL) {
						search_result_head = p_search_node;
						p_search_node->prev = NULL;
					}
					else {
						search_result_tail->next = p_search_node;
						p_search_node->prev = search_result_tail;
					}
					search_result_tail = p_search_node;
				}
			}
		}
	}

	for (struct book_node* p = book_list_head; p != NULL; ) {
		if (p->next == NULL) {
			free(p);
			break;
		}
		else {
			p = p->next;
			free(p->prev);
		}
	}
	book_list_head = NULL;
	return search_result_head;
}

struct book_node* search_author(char * word_author) 
{

		book_list_head = read_BookInfo();

	struct book_node * search_result_head = NULL;
	struct book_node * search_result_tail = NULL;
	struct book_node * p_search_node = NULL;
	struct book_node * p_book_node = book_list_head;

	int keyword_len = strlen(word_author);
	char cur_keyword[NAME_LEN_MAX];
	//搜索的字符串长度递减
	for (int len = keyword_len; len > 0; len--) {
		//字符串开始的位置顺延
		for (int start_location = 0; start_location < keyword_len - len + 1; start_location++) {
			//获得本次的搜索关键字符串
			for (int i = 0; i < len; i++) {
				cur_keyword[i] = *(word_author + start_location + i);
			}
			cur_keyword[len] = '\0';
			//在bookfile链表中顺序查找，找到且前面没有则添加到搜索结果
			for (p_book_node = book_list_head; p_book_node != NULL; p_book_node = p_book_node->next) {
				int found = 0;
				for (struct book_node * p = search_result_head; p != NULL; p = p->next) {
					if (p->book.BookNO == p_book_node->book.BookNO) {
						found = 1;
						break;
					}
				}
				if (found == 1) continue;
				if (strstr(p_book_node->book.author[0], cur_keyword)||
					strstr(p_book_node->book.author[1], cur_keyword)||
					strstr(p_book_node->book.author[2], cur_keyword)) 
				{
					p_search_node = (struct book_node*)malloc(sizeof(struct book_node));
					p_search_node->book = p_book_node->book;
					p_search_node->next = NULL;

					if (search_result_head == NULL) {
						search_result_head = p_search_node;
						p_search_node->prev = NULL;
					}
					else {
						search_result_tail->next = p_search_node;
						p_search_node->prev = search_result_tail;
					}
					search_result_tail = p_search_node;
				}
			}
		}
	}

	for (struct book_node* p = book_list_head; p != NULL; ) {
		if (p->next == NULL) {
			free(p);
			break;
		}
		else {
			p = p->next;
			free(p->prev);
		}
	}
	book_list_head = NULL;
	return search_result_head;
}

Book * find_num(int num)
{

		book_list_head = read_BookInfo();

	struct book_node * p_book_node = book_list_head;
	Book result_book;
	int found = 0;
	for (p_book_node = book_list_head; p_book_node != NULL; p_book_node = p_book_node->next) {
		if (p_book_node->book.BookNO==num)
		{
			result_book = p_book_node->book;
			found = 1;
			break;
		}
	}
	for (struct book_node* p = book_list_head; p != NULL; ) {
		if (p->next == NULL) {
			free(p);
			break;
		}
		else {
			p = p->next;
			free(p->prev);
		}
	}
	book_list_head = NULL;
	if (found == 1)
		return &result_book;
	else if (found == 0)
		return NULL;
}

char * cut_string(char * source, int len_limit)
{
	if (strlen(source) > len_limit) {
		source[len_limit] = '.';
		source[len_limit+1] = '.';
		source[len_limit+2] = '.';
		source[len_limit+3] = '\0';
	}
	return source;
}
