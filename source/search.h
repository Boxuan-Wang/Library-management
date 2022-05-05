/*����ģ��
*�������ܣ�
*1. ��������ģ������
*2. �Թؼ��ʵ�ģ������
*3. ���ݱ�ŵ���ȷ����
*4. �������ߵ�ģ������
*/

#pragma once
#ifndef __searchbook_h__
#define __searchbook_h__

#include"bookmanagement.h"
#include "filesource.h"

/*��������ģ������
*�����������������ͷָ��
*/
struct book_node* search_name( char* word_name);

/*�Թؼ��ʵ�ģ������
*�� MAX_search_results ����ض���ߵ�ͼ��Ľṹָ��д��result��
*�����ظ�ָ��
*/
struct book_node* search_keyword(char * word_keyword);

/*�����ߵ�ģ������
*�� MAX_search_results ����ض���ߵ�ͼ��Ľṹָ��д��result��
*�����ظ�ָ��
*/
struct book_node* search_author(char * word_author);

/*�Ա�ŵ���ȷ����
**/
Book* find_num(int num);

/*���ַ������г�������
*�ü��ַ�����ĩβΪʡ�Ժ�
*/
char * cut_string(char * source, int len_limit);
#endif 
