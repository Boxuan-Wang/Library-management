/*图书信息管理函数库
*包含功能，
*1.管理员新增图书
*2.管理员删除图书
*3.管理员更改信息
*4.读者借书
*5.读者还书
*6.根据图书编号编号，返回图书全部信息的字符串
*7. 辅助功能，用于参数传递，在其他模块里更改、读取获得focus的书
*/

#pragma once
#ifndef __bookmanagement_h__
#define __bookmanagement_h__

#define NAME_LEN_MAX 400
#define WORD_LEN_MAX 50
typedef struct {
	int BookNO;		//图书编号
	char name[NAME_LEN_MAX];
	char keyword[5][WORD_LEN_MAX];//最多5个关键字
	char author[3][WORD_LEN_MAX];//最多3人
	char press[NAME_LEN_MAX];
	int publishYear;
	int totalNum;	//总共有多少本
	int currentNum;		//现有多少本未借出
}Book;

/*管理员新增图书
*p_newbook 是指向新书信息的结构指针
*cnt是本次要增加的数量
*如果是已有的图书，则增加totalNum；若是新书，则要新建
*添加或新建成功，返回0；
*输入的信息不合法，返回1；
*/
int ADaddbook(Book* p_newbook, int cnt);

/*管理员删除图书
*num 图书的编号，据此找到图书将tatalNum-cnt
*成功返回0，
*cnt大于现有量 返回1
*没有这本书 返回2
*/
int ADdeleteBook(int BookNO, int cnt);

/*管理员更改图书信息
*p_newInfofBook 是新的图书信息的结构指针
*根据num检索书籍，并更改信息
*成功返回0
*没找到对应图书 返回1
*输入信息格式不合法 返回2
*/
int ADrecomposeBook(Book* p_newInfofBook);

/*读者借书
！！注意，读者不可以借两本一样的书
*对应书目的currentNum-1
*借书成功  返回0
*剩余不足 返回1
*读者已经借阅了相同的书 返回2
*读者借书数量达到上限  返回3
*/
int ReaderLend(int BookNO, char* ReaderAccount);

/*读者还书
*对应书目currentNum+1
*还书成功返回0
*没找到相关记录 返回1
*/
int ReaderReturn(int bookNO, char* ReaderAccount);

/*根据图书序号返回图书全部信息
*！！使用方法！！ book=*GetBookInf(123);
*返回指向Book结构
*/
Book* GetBookInf(int num);

//更改和读取聚焦的书号
void change_focusbook(int BookNO);
int get_focusbook(void);

//读取和更改借书的时间限制,单位day
int get_lendTimeLimit(void);
void change_lendTimeLimit(int new_limit);

//读取和更改借书的数量限制
int get_lendNumLimit(void);
void change_lendNUMLimit(int new_limit);

//free list
void free_book_list();
void free_lendrecord_list();
void free_historyrecord_list();
#endif 