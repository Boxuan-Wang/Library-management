/*ͼ����Ϣ��������
*�������ܣ�
*1.����Ա����ͼ��
*2.����Աɾ��ͼ��
*3.����Ա������Ϣ
*4.���߽���
*5.���߻���
*6.����ͼ���ű�ţ�����ͼ��ȫ����Ϣ���ַ���
*7. �������ܣ����ڲ������ݣ�������ģ������ġ���ȡ���focus����
*/

#pragma once
#ifndef __bookmanagement_h__
#define __bookmanagement_h__

#define NAME_LEN_MAX 400
#define WORD_LEN_MAX 50
typedef struct {
	int BookNO;		//ͼ����
	char name[NAME_LEN_MAX];
	char keyword[5][WORD_LEN_MAX];//���5���ؼ���
	char author[3][WORD_LEN_MAX];//���3��
	char press[NAME_LEN_MAX];
	int publishYear;
	int totalNum;	//�ܹ��ж��ٱ�
	int currentNum;		//���ж��ٱ�δ���
}Book;

/*����Ա����ͼ��
*p_newbook ��ָ��������Ϣ�Ľṹָ��
*cnt�Ǳ���Ҫ���ӵ�����
*��������е�ͼ�飬������totalNum���������飬��Ҫ�½�
*��ӻ��½��ɹ�������0��
*�������Ϣ���Ϸ�������1��
*/
int ADaddbook(Book* p_newbook, int cnt);

/*����Աɾ��ͼ��
*num ͼ��ı�ţ��ݴ��ҵ�ͼ�齫tatalNum-cnt
*�ɹ�����0��
*cnt���������� ����1
*û���Ȿ�� ����2
*/
int ADdeleteBook(int BookNO, int cnt);

/*����Ա����ͼ����Ϣ
*p_newInfofBook ���µ�ͼ����Ϣ�Ľṹָ��
*����num�����鼮����������Ϣ
*�ɹ�����0
*û�ҵ���Ӧͼ�� ����1
*������Ϣ��ʽ���Ϸ� ����2
*/
int ADrecomposeBook(Book* p_newInfofBook);

/*���߽���
����ע�⣬���߲����Խ�����һ������
*��Ӧ��Ŀ��currentNum-1
*����ɹ�  ����0
*ʣ�಻�� ����1
*�����Ѿ���������ͬ���� ����2
*���߽��������ﵽ����  ����3
*/
int ReaderLend(int BookNO, char* ReaderAccount);

/*���߻���
*��Ӧ��ĿcurrentNum+1
*����ɹ�����0
*û�ҵ���ؼ�¼ ����1
*/
int ReaderReturn(int bookNO, char* ReaderAccount);

/*����ͼ����ŷ���ͼ��ȫ����Ϣ
*����ʹ�÷������� book=*GetBookInf(123);
*����ָ��Book�ṹ
*/
Book* GetBookInf(int num);

//���ĺͶ�ȡ�۽������
void change_focusbook(int BookNO);
int get_focusbook(void);

//��ȡ�͸��Ľ����ʱ������,��λday
int get_lendTimeLimit(void);
void change_lendTimeLimit(int new_limit);

//��ȡ�͸��Ľ������������
int get_lendNumLimit(void);
void change_lendNUMLimit(int new_limit);

//free list
void free_book_list();
void free_lendrecord_list();
void free_historyrecord_list();
#endif 