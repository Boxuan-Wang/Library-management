/*�ļ�����ģ��
*������
*1. �ļ�·���ĺ궨��
*2. ��ȡȫ�����ļ���Ϣ, ��������(˫������
*3. ����������Ϣ, ��д�ļ���Ϣ
*4. �������ܵĺ���
*/


/*�ļ��ṹ���,
*���趨5���ļ�,
*ͼ����Ϣ; �����û�����Ϣ; ����Ա�˻���Ϣ; ��δ�黹�Ľ����¼; �Ѿ��黹�Ľ�����ʷ��¼
*/

#pragma once
#ifndef __filesource_h__
#define __filesource_h__

//����ͼ����Ϣ�洢·��
#define BookFileSource "C:\\projectFileTest\\BookFile.txt"

//��������û���Ϣ�洢·��
#define ReadersFileSource "C:\\projectFileTest\\ReadersFile.txt"

//�������Ա��Ϣ�洢·��
#define AdministratorFileSource "C:\\projectFileTest\\AdministratorFile.txt"

//��������¼�洢·��
#define LendRecordFileSource "C:\\projectFileTest\\LendRecord.txt"

//��ʷ�����¼�洢·��
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

//�ļ������ڴ�,��������,����ͷָ��
struct book_node* read_BookInfo(void);
struct reader_node* read_ReaderInfo(void);
struct AD_node* read_manager_Info(void);
struct lendrecord_node* read_LendRecord(void);
struct historyrecord_node* read_HistoryRecord(void);


//�ڴ��е�����д���ļ�,����ͷָ�룬�ɹ��򷵻�0��ʧ�ܷ��ط�0
int write_BookInfo(struct book_node* book_list_head);
int write_ReaderInfo(struct reader_node * reader_list_head);
int write_AdministratorInfo(struct AD_node* administrator_list_head);
int write_LendRecord(struct lendrecord_node* record_list_head);
int write_HistoryRecord(struct historyrecord_node* history_list_head);

//�����е�����free
void free_all_list();
//�������ܵĺ���,�����ַ���ת���֣����ַ����ַ���-1
long long int translate_StrToNum(const char* numberString);

#endif // !__filesource_h__
