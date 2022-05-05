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
/*��lendrecord�ļ�������һ�����ļ�¼,
*���߽���ʱʹ��
*�ɹ�����1��ʧ�ܷ���0*/
int create_record(char* CurrentReaderAccount, int BookNO);


//���ݶ��ߵ�account��ѯ�������Ѿ�������ٱ�
int LendNumber(char* CurrentReaderAccount);


/*��ѯָ���û����ĵ���Ŀ
*����book������
*���������ͷָ��
*/
struct lendrecord_node * GetLentBooks(char * CurrentReaderAccount);


/*��������¼��lendrecord��ȥ����
*�����ƶ���historyrecord�У�
*���߻���ʱʹ��*/
int finish_record(char* CurrentReaderAccount, int BookNO);




#endif // !__lendrecord_h__