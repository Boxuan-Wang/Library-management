/*文件处理模块
*包括，
*1. 文件路径的宏定义
*2. 读取全部的文件信息, 建立链表(双向链表）
*3. 根据链表信息, 重写文件信息
*4. 辅助功能的函数
*/


/*文件结构设计,
*共设定5个文件,
*图书信息; 读者用户的信息; 管理员账户信息; 尚未归还的借书记录; 已经归还的借书历史记录
*/

#pragma once
#ifndef __filesource_h__
#define __filesource_h__

//定义图书信息存储路径
#define BookFileSource "C:\\projectFileTest\\BookFile.txt"

//定义读者用户信息存储路径
#define ReadersFileSource "C:\\projectFileTest\\ReadersFile.txt"

//定义管理员信息存储路径
#define AdministratorFileSource "C:\\projectFileTest\\AdministratorFile.txt"

//定义借书记录存储路径
#define LendRecordFileSource "C:\\projectFileTest\\LendRecord.txt"

//历史借书记录存储路径
#define HistoryRecordFileSource "C:\\projectFileTest\\HistoryRecord.txt"


#include"bookmanagement.h"
#include"users.h"
#include"lendrecord.h"

struct book_node {
	struct book_node * prev;
	Book book;
	struct book_node * next;
};

struct reader_node {
	struct reader_node * prev;
	Reader reader;
	struct reader_node * next;
};

struct AD_node {
	struct AD_node * prev;
	Administrator administrator;
	struct AD_node * next;
};

struct lendrecord_node {
	struct lendrecord_node * prev;
	struct record record;
	struct lendrecord_node * next;
};

struct historyrecord_node {
	struct historyrecord_node * prev;
	struct history_record history;
	struct historyrecord_node * next;
};

//文件读入内存,建立链表,返回头指针
struct book_node* read_BookInfo(void);
struct reader_node* read_ReaderInfo(void);
struct AD_node* read_manager_Info(void);
struct lendrecord_node* read_LendRecord(void);
struct historyrecord_node* read_HistoryRecord(void);


//内存中的链表写入文件,传入头指针，成功则返回0；失败返回非0
int write_BookInfo(struct book_node* book_list_head);
int write_ReaderInfo(struct reader_node * reader_list_head);
int write_AdministratorInfo(struct AD_node* administrator_list_head);
int write_LendRecord(struct lendrecord_node* record_list_head);
int write_HistoryRecord(struct historyrecord_node* history_list_head);

//将所有的链表free
void free_all_list();
//辅助功能的函数,数字字符串转数字，出现非数字返回-1
long long int translate_StrToNum(const char* numberString);

#endif // !__filesource_h__
