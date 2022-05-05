#pragma once
#ifndef __lendrecord_h__
#define __lendrecord_h__

#include<time.h>
#include"bookmanagement.h"
#define time_dayToInt(x) ((x)*24*3600)
#define	INT_MAX_LEN 20

struct record {
	int BookNO;
	char reader_account[WORD_LEN_MAX];
	time_t lendTime;
	time_t ddlTime;
};

struct history_record {
	int BookNO;
	char reader_account[WORD_LEN_MAX];
	time_t lendTime;
	time_t ddlTime;
	time_t actual_ReturnTime;
};
#include"filesource.h"
/*在lendrecord文件里新增一条借阅记录,
*读者借书时使用
*成功返回1，失败返回0*/
int create_record(char* CurrentReaderAccount, int BookNO);


//根据读者的account查询他现在已经借书多少本
int LendNumber(char* CurrentReaderAccount);


/*查询指定用户借阅的书目
*建立book的链表
*返回链表的头指针
*/
struct lendrecord_node * GetLentBooks(char * CurrentReaderAccount);


/*将这条记录在lendrecord中去除，
*将其移动到historyrecord中，
*读者还书时使用*/
int finish_record(char* CurrentReaderAccount, int BookNO);




#endif // !__lendrecord_h__