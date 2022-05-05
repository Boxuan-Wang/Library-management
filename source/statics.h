#pragma once
#ifndef __statics_h__
#define __statics_h__

#include "users.h"
#include"lendrecord.h"
#include"bookmanagement.h"
#include"filesource.h"

typedef enum
{
	lend,
	return_

}StatisticsStatus;

//读入各文件信息，建立链表
void read_doccument(void);

//返回图书总库量（含重复）
int GetvolumeofBooks(void);

//返回图书总库量（不含重复）
int GetvarietyofBooks(void);

//返回注册用户数
int GetnumberofUsers(void);

/*检查日期的合法性，同时将输入放入全局静态变量以进行之后处理
*不合法返回0
合法返回1*/
int check_Date(char* fromY, char* fromM, char* fromD, char* toY, char* toM, char* toD);

/*检查字符串内容
*是空或含有非数字则返回0
是数字则返回相应的int*/
int check_String(char* string);

//获得借书总数
int Get_numberofLend(void);

//获得还书总数
int Get_numberofReturn(void);

/*检查输入的周期是否过大
过大返回0
合适返回1*/
int check_Date_interval(void);

//打印统计图
void print_statisticalChart(int flag, char * color);

#endif 