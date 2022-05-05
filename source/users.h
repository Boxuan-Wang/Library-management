/*�����û�/����Ա�˻�ģ��
*�������ܣ�
*1. ���ߵ�¼
*2. ����ע��
*3. ������ȡ������Ϣ
*4. �����޸ĸ�����Ϣ
*5. ����Ա��¼
*6. ��������Ա

*8. ��������ʱ���鿴��¼��״̬
*/

#pragma once
#ifndef __users_h__
#define __users_h__
#include"bookmanagement.h"
#define PASSWORD_LEN_MAX 21
#define PASSWORD_LEN_MIN 6
typedef enum {
	Male,
	Female,
}Gender;

typedef struct {
	char account[WORD_LEN_MAX];
	char password[PASSWORD_LEN_MAX];
	Gender gender;
	char WorkPlace[NAME_LEN_MAX];
	char eMail[WORD_LEN_MAX];
}Reader;

typedef struct{
	char AD_account[WORD_LEN_MAX];
	char AD_password[PASSWORD_LEN_MAX];
}Administrator;

typedef enum {
	AD_acitve,
	Reader_active,
	Not_Active,
}LoginType;

struct LoginStatus {
	LoginType type;
	char account[WORD_LEN_MAX];
};

/*���ߵ�¼
*���˻����ڣ���������ȷ���򷵻ص�¼���˺ţ�
*���˻����ڵ�������󣬷��ء�__wrong/password__��
*û�ж�Ӧ���˺ţ�����"__no/account__"
*/
char* ReaderLogin(char* account, char* password);

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
int ReaderRegister(Reader* p_newReader,char* passwordCheck,char* gender_string);

/*��ȡ������Ϣ
*����ʹ�÷������� reader=*GetReaderInf(2321@sa);
*��ȡ�����ߵ���Ϣ
*����Reader�ṹָ��
*ʧ�ܷ���NULL
*/
Reader* GetReaderInf(char* account);

/*�����޸ĸ�����Ϣ,���в������޸�����
*�ṹָ���У�account��email��workplace��Ч
*�޸ĳɹ�		����0
*���벻�淶������'/'	����1
*�Ա������ʽ����		����2
*account�Ѿ���ռ��		����3
*����account�ҵ��ö��ߣ�������Ϣ
*/
int recomposeReaderInf( Reader* p_newReaderInf, char * gender_string);

/*�����޸�����
*�޸ĳɹ�		����0
*δ��½		����1
*ԭ�������		����2
*�����ʽ���󣬰����ˡ�/'	����3
*�������벻һ�·���			����4
*����̫�̻�����δ�����κ���ĸ	����5
*/
int recomposeReaderPassword(char* prev, char* new, char* check);

/*����Ա��¼
*���˻����ڣ���������ȷ���򷵻ص�¼���˺ţ�
*���˻����ڵ�������󣬷��ء�__wrong/password__��
*û�ж�Ӧ���˺ţ�����"__no/account__"
*/
char* manager_login(char * account, char * password);

//��ȡ����Ա��Ϣ
Administrator* GetAdministratorInf(char* account_AD);

/*�����¹���Ա
*�ɹ���				����0
*���벻�淶			����1
*account�Ѿ���ռ��	����2
*���벻��ȫ			����3
*�����������벻ͬ	����4
*/
int GenNewAdimistrator(Administrator* p_newAD, char * password_check);

/*����¼״̬
*������ģ���е��ã������鿴��¼��״̬�ͻ�Ծ���û�account
*����LoginStatus�ṹָ��
*/
struct LoginStatus* GetLogStatus(void);

//free lists
void free_reader_list();
void free_AD_list();

//�ǳ�����
void reader_logout(void);
void AD_logout(void);
#endif