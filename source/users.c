#include "users.h"
#include"lendrecord.h"
#include"bookmanagement.h"
#include"filesource.h"
#include"search.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>



//������Ϣ���ڴ���������洢��һ������ֻ��д�ļ�һ�Σ�������
static struct reader_node * reader_list_head = NULL;	
static struct AD_node * AD_list_head = NULL;
static struct LoginStatus active_account = { Not_Active,"" };




struct LoginStatus * GetLogStatus(void)
{
	//���е�����
	//struct LoginStatus active_account = { Reader_active,"reader1" };
	//return &active_account;
	return &active_account;
}

void free_reader_list()
{
	struct reader_node *p_reader_temp = reader_list_head;
	while (p_reader_temp != NULL) {
		struct reader_node * t = p_reader_temp;
		p_reader_temp = p_reader_temp->next;
		free(t);
	}
}

void free_AD_list()
{
	struct AD_node * p_AD_temp = AD_list_head;
	while (p_AD_temp != NULL) {
		struct AD_node * t = p_AD_temp;
		p_AD_temp = p_AD_temp->next;
		free(t);
	}
}

void reader_logout(void)
{
	if (active_account.type == Reader_active) {
		active_account.type = Not_Active;
		strcpy(active_account.account, "");
	}
}

void AD_logout(void)
{
	if (active_account.type == AD_acitve) {
		active_account.type = Not_Active;
		strcpy(active_account.account, "");
	}
}

/*���ߵ�¼
*���˻����ڣ���������ȷ���򷵻ص�¼���˺ţ�
*���˻����ڵ�������󣬷��ء�__wrong/password__��
*û�ж�Ӧ���˺ţ�����"__no/account__"
*/
char * ReaderLogin(char * account, char * password)
{
	//if (reader_list_head == NULL) {
	//	reader_list_head = read_ReaderInfo();
	//}
	Reader cur_reader;
	if (GetReaderInf(account) == NULL)	{
		return "__no/account__";
		//�޶�Ӧ���˺�
	}
	else {
		cur_reader = *GetReaderInf(account);
		if (strcmp(cur_reader.password, password) == 0) {
			//���Ĵ����¼״̬��ȫ�ֱ���
			active_account.type = Reader_active;
			strcpy(active_account.account, cur_reader.account);
			return cur_reader.account;
		}
		else {
			return "__wrong/password__";
		}
	}
	
}

/*�����޸�����
*�޸ĳɹ�		����0
*δ��½		����1
*ԭ�������		����2
*�����ʽ���󣬰����ˡ�/'	����3
*�������벻һ�·���			����4
*����̫�̻�����δ�����κ���ĸ	����5
*/
int recomposeReaderPassword(char * prev, char * new, char * check)
{

	if (active_account.type != Reader_active) {
		return 1;
	}
	Reader cur_reader=*GetReaderInf(active_account.account);
	if (strcmp(cur_reader.password, prev) != 0) {
		return 2;
	}
	if (strchr(new, '/') != NULL){
		return 3;
	}
	if (strcmp(new, check) != 0) {
		return 4;
	}
	if (strlen(new) < PASSWORD_LEN_MIN) {
		return 5;
	}
	//�ж��Ƿ�����ĸ
	{
		int found = 0;
		
		int found_letter = 0;
		for (int i = 0; i < strlen(new); i++) {
			if ((new[i] >= 'a'&&new[i] <= 'z') ||
				(new[i] >= 'A'&&new[i] <= 'Z')) {
				found_letter = 1;
				break;
			}
		}
		if (found_letter == 0)
		{
			return 5;
		}
	}
	//����ȫ����Ҫ��
	if (reader_list_head == NULL) {
		reader_list_head = read_ReaderInfo();
	}
	struct reader_node * p_found;
	for (p_found = reader_list_head; 
		p_found != NULL && strcmp(p_found->reader.account , active_account.account)!= 0;
		p_found = p_found->next);
	strcpy(p_found->reader.password, new);

	write_ReaderInfo(reader_list_head);

	return 0;
}

//����Ա��¼
char * manager_login(char* account, char * password)
{
	Administrator cur_AD;
	if (GetAdministratorInf(account) == NULL) {
		return "__no/account__";
	}
	else {
		cur_AD = *GetAdministratorInf(account);
		if (strcmp(cur_AD.AD_password, password) == 0) {
			active_account.type = AD_acitve;
			strcpy(active_account.account, cur_AD.AD_account);
			return cur_AD.AD_account;
		}
		else {
			return "__wrong/password__";
		}
	}
}
/*����ע��
*passworkCheck �ǡ��ٴ��������롱�е��ַ���
*gender_string���û�������Ա��ַ���
*p_newReader��Reader�Ľṹָ��
*����ĸ�ʽ����ȷ��������/���������	����1
*�Ա��ַ�������Ĳ��Ϲ淶��				����2
*�˺��Ѿ���ע��							����3
*�������벻һ�£�						����4
*���벻�ϸ񣬣�6λ���ϣ�����ĸ	��		����5
*ȫ����ȷ��ע��ɹ�						����0
*/
int ReaderRegister(Reader * p_newReader, char * passwordCheck, char *gender_string)
{
	if (reader_list_head == NULL) {
		reader_list_head = read_ReaderInfo();
	}
	if (p_newReader == NULL) {
		printf("register ERROR\n");
		exit(1);
	}
	Reader new_reader = *p_newReader;

	//�ж������Ƿ�淶�������С�/�������Ⱥ��ʣ������ÿ��ǣ���Ϊ��gui����
	if (strchr(new_reader.account, '/') || strchr(new_reader.password, '/') ||
		strchr(new_reader.eMail, '/') || strchr(new_reader.WorkPlace, '/') ||
		strchr(passwordCheck, '/')) {
		return 1;
	}
	//�Ա�������Ƿ�淶
	if (strcmp(gender_string, "Male") && strcmp(gender_string, "Female")) {
		return 2;
	}
	else if (strcmp(gender_string, "Male") == 0) {
		new_reader.gender = Male;
	}
	else if (strcmp(gender_string, "Female") == 0) {
		new_reader.gender = Female;
	}
	//�ж�account�Ƿ�����ʹ��
	{
		if (GetReaderInf(new_reader.account) != NULL) {
		return 3;
		}
	}
	//�ж��������������Ƿ���ͬ
	{
		if (strcmp(new_reader.password, passwordCheck) != 0) {
		return 4;
		}
	}
	//�ж����밲ȫ���Ƿ�ϸ�,
	{
		if (strlen(new_reader.password) < PASSWORD_LEN_MIN) {
		return 5;
		}
		//�ж��Ƿ�����ĸ
		int found_letter = 0;
		for (int i = 0; i < strlen(new_reader.password); i++) {
			if ((new_reader.password[i] >= 'a'&&new_reader.password[i] <= 'z') ||
				(new_reader.password[i] >= 'A'&&new_reader.password[i] <= 'Z')) {
				found_letter = 1;
				break;
			}		
		}
		if (found_letter == 0)
		{
			return 5;
		}
	}

	//ȫ������Ҫ������
	struct reader_node * reader_list_tail;
	for (reader_list_tail = reader_list_head; reader_list_tail->next != NULL; 
		reader_list_tail = reader_list_tail->next);//�ҵ������tail
	//������Ϊ�����ڵ�
	reader_list_tail->next = (struct reader_node *)malloc(sizeof(struct reader_node));
	reader_list_tail->next->prev = reader_list_tail;
	reader_list_tail->next->reader = new_reader;
	reader_list_tail->next->next = NULL;
	write_ReaderInfo(reader_list_head);
	return 0;

}

/*��ȡ������Ϣ
*����ʹ�÷������� reader=*GetReaderInf(2321@sa);
*��ȡ�����ߵ���Ϣ
*����Reader�ṹָ��
*ʧ�ܷ���NULL
*/
Reader * GetReaderInf(char * account)
{
	if (reader_list_head == NULL) {
		reader_list_head = read_ReaderInfo();
	}

	struct reader_node* p_reader_node = reader_list_head;

	int found = 0;
	while (p_reader_node != NULL) {
		if (strcmp(account, p_reader_node->reader.account)==0) {
			found = 1;
			break;
		}
		else {
			p_reader_node = p_reader_node->next;
		}
	}
	Reader* ret=NULL;
	if (found == 0) {
		ret = NULL;
	}
	else if (found==1){
		ret = &p_reader_node->reader;
	}
	return ret;

}

/*�����޸ĸ�����Ϣ,���в������޸�����
*�ṹָ���У�account��email��workplace��Ч
*�޸ĳɹ�		����0
*���벻�淶������'/'	����1
*�Ա������ʽ����		����2
*account�Ѿ���ռ��		����3
*����account�ҵ��ö��ߣ�������Ϣ
*/
int recomposeReaderInf( Reader * p_newReaderInf ,char * gender_string )
{
	//�ж������Ƿ�淶�������С�/�������Ⱥ��ʣ������ÿ��ǣ���Ϊ��gui����
	if (strchr(p_newReaderInf->account, '/') || 
		strchr(p_newReaderInf->eMail, '/') || strchr(p_newReaderInf->WorkPlace, '/') ) {
		return 1;
	}
	//�Ա�������Ƿ�淶
	if (strcmp(gender_string, "Male") && strcmp(gender_string, "Female")) {
		return 2;
	}
	else if (strcmp(gender_string, "Male") == 0) {
		p_newReaderInf->gender = Male;
	}
	else if (strcmp(gender_string, "Female") == 0) {
		p_newReaderInf->gender = Female;
	}
	//�ж�account�Ƿ�����ʹ��
	{
		if (GetReaderInf(p_newReaderInf->account) != NULL &&
			strcmp(active_account.account, p_newReaderInf->account)) {
			return 3;
		}
	}
	//ȫ����ȷ����ʼ����
	if (reader_list_head == NULL) {
		reader_list_head = read_ReaderInfo();
	}
	struct reader_node * p_found;
	for (p_found = reader_list_head;
		p_found != NULL && strcmp(p_found->reader.account, active_account.account);
		p_found = p_found->next);
	strcpy(p_found->reader.account, p_newReaderInf->account);
	strcpy(p_found->reader.eMail, p_newReaderInf->eMail);
	strcpy(p_found->reader.WorkPlace, p_newReaderInf->WorkPlace);
	p_found->reader.gender = p_newReaderInf->gender;
	//��¼״�е�account����
	strcpy(active_account.account, p_newReaderInf->account);
	//��д�ļ�
	write_ReaderInfo(reader_list_head);
	return 0;

}

Administrator * GetAdministratorInf(char * account_AD)
{
	if (AD_list_head == NULL) {
		AD_list_head = read_manager_Info();
	}
	struct AD_node * p_AD_node = AD_list_head;
	int found = 0;
	while (p_AD_node != NULL) {
		if (strcmp(account_AD, p_AD_node->administrator.AD_account) == 0) {
			found = 1;
			break;
		}
		else {
			p_AD_node = p_AD_node->next;
		}
	}
	Administrator * ret = NULL;
	if (found == 0) {
		ret = NULL;
	}
	else if (found == 1) {
		ret = &p_AD_node->administrator;
	}
	return ret;
}

/*�����¹���Ա
*�ɹ���				����0
*���벻�淶,����'/'	����1
*account�Ѿ���ռ��	����2
*���벻��ȫ			����3
*�����������벻ͬ	����4
*/
int GenNewAdimistrator(Administrator * p_newAD, char* password_check)
{
	if (AD_list_head == NULL) {
		AD_list_head = read_manager_Info();
	}
	if (strchr(p_newAD->AD_account, '/') != NULL || strchr(p_newAD->AD_password, '/') != NULL) {
		return 1;
	}
	if (GetAdministratorInf(p_newAD->AD_account) != NULL) {
		return 2;
	}
	if (strlen(p_newAD->AD_password) < PASSWORD_LEN_MIN) {
		return 3;
	}
	{
		int found_letter = 0;
		for (int i = 0; i < strlen(p_newAD->AD_password); i++) {
			if ((p_newAD->AD_password[i] >= 'a'&&p_newAD->AD_password[i] <= 'z') ||
				(p_newAD->AD_password[i] >= 'A'&&p_newAD->AD_password[i] <= 'Z')) {
				found_letter = 1;
				break;
			}
		}
		if (found_letter == 0)
		{
			return 5;
		}
	}
	if (strcmp(p_newAD->AD_password, password_check) != 0) {
		return 4;
	}
	//all correct
	struct AD_node * AD_list_tail;
	for (AD_list_tail = AD_list_head; AD_list_tail->next != NULL; 
		AD_list_tail = AD_list_tail->next);
	AD_list_tail->next = (struct AD_node*)malloc(sizeof(struct AD_node));
	AD_list_tail->next->prev = AD_list_tail;
	AD_list_tail->next->administrator = *p_newAD;
	AD_list_tail->next->next = NULL;
	AD_list_tail = AD_list_tail->next;
	write_AdministratorInfo(AD_list_head);
	return 0;
}
