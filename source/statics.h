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

//������ļ���Ϣ����������
void read_doccument(void);

//����ͼ���ܿ��������ظ���
int GetvolumeofBooks(void);

//����ͼ���ܿ����������ظ���
int GetvarietyofBooks(void);

//����ע���û���
int GetnumberofUsers(void);

/*������ڵĺϷ��ԣ�ͬʱ���������ȫ�־�̬�����Խ���֮����
*���Ϸ�����0
�Ϸ�����1*/
int check_Date(char* fromY, char* fromM, char* fromD, char* toY, char* toM, char* toD);

/*����ַ�������
*�ǿջ��з������򷵻�0
�������򷵻���Ӧ��int*/
int check_String(char* string);

//��ý�������
int Get_numberofLend(void);

//��û�������
int Get_numberofReturn(void);

/*�������������Ƿ����
���󷵻�0
���ʷ���1*/
int check_Date_interval(void);

//��ӡͳ��ͼ
void print_statisticalChart(int flag, char * color);

#endif 