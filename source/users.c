#include "users.h"
#include"lendrecord.h"
#include"bookmanagement.h"
#include"filesource.h"
#include"search.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>



//读者信息在内存中用链表存储，一次运行只读写文件一次，链表常开
static struct reader_node * reader_list_head = NULL;	
static struct AD_node * AD_list_head = NULL;
static struct LoginStatus active_account = { Not_Active,"" };




struct LoginStatus * GetLogStatus(void)
{
	//先行调试用
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

/*读者登录
*若账户存在，且密码正确，则返回登录的账号；
*若账户存在但密码错误，返回“__wrong/password__”
*没有对应的账号，返回"__no/account__"
*/
char * ReaderLogin(char * account, char * password)
{
	//if (reader_list_head == NULL) {
	//	reader_list_head = read_ReaderInfo();
	//}
	Reader cur_reader;
	if (GetReaderInf(account) == NULL)	{
		return "__no/account__";
		//无对应的账号
	}
	else {
		cur_reader = *GetReaderInf(account);
		if (strcmp(cur_reader.password, password) == 0) {
			//更改代表登录状态的全局变量
			active_account.type = Reader_active;
			strcpy(active_account.account, cur_reader.account);
			return cur_reader.account;
		}
		else {
			return "__wrong/password__";
		}
	}
	
}

/*读者修改密码
*修改成功		返回0
*未登陆		返回1
*原密码错误		返回2
*密码格式错误，包含了‘/'	返回3
*两次输入不一致返回			返回4
*密码太短或密码未包含任何字母	返回5
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
	//判断是否含有字母
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
	//符合全部的要求
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

//管理员登录
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
/*读者注册
*passworkCheck 是“再次输入密码”中的字符串
*gender_string是用户输入的性别字符串
*p_newReader是Reader的结构指针
*输入的格式不正确，包含‘/‘的情况，	返回1
*性别字符串输入的不合规范，				返回2
*账号已经被注册							返回3
*两次密码不一致，						返回4
*密码不合格，（6位以上，含字母	）		返回5
*全部正确，注册成功						返回0
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

	//判断输入是否规范，不含有‘/’，长度合适（好像不用考虑，因为有gui）？
	if (strchr(new_reader.account, '/') || strchr(new_reader.password, '/') ||
		strchr(new_reader.eMail, '/') || strchr(new_reader.WorkPlace, '/') ||
		strchr(passwordCheck, '/')) {
		return 1;
	}
	//性别的输入是否规范
	if (strcmp(gender_string, "Male") && strcmp(gender_string, "Female")) {
		return 2;
	}
	else if (strcmp(gender_string, "Male") == 0) {
		new_reader.gender = Male;
	}
	else if (strcmp(gender_string, "Female") == 0) {
		new_reader.gender = Female;
	}
	//判断account是否有人使用
	{
		if (GetReaderInf(new_reader.account) != NULL) {
		return 3;
		}
	}
	//判断两次密码输入是否相同
	{
		if (strcmp(new_reader.password, passwordCheck) != 0) {
		return 4;
		}
	}
	//判断密码安全性是否合格,
	{
		if (strlen(new_reader.password) < PASSWORD_LEN_MIN) {
		return 5;
		}
		//判断是否含有字母
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

	//全部符合要求的情况
	struct reader_node * reader_list_tail;
	for (reader_list_tail = reader_list_head; reader_list_tail->next != NULL; 
		reader_list_tail = reader_list_tail->next);//找到链表的tail
	//向链表为新增节点
	reader_list_tail->next = (struct reader_node *)malloc(sizeof(struct reader_node));
	reader_list_tail->next->prev = reader_list_tail;
	reader_list_tail->next->reader = new_reader;
	reader_list_tail->next->next = NULL;
	write_ReaderInfo(reader_list_head);
	return 0;

}

/*提取读者信息
*！！使用方法！！ reader=*GetReaderInf(2321@sa);
*提取出读者的信息
*返回Reader结构指针
*失败返回NULL
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

/*读者修改个人信息,其中不包括修改密码
*结构指针中，account，email，workplace有效
*修改成功		返回0
*输入不规范，包含'/'	返回1
*性别输入格式错误		返回2
*account已经被占用		返回3
*根据account找到该读者，其他信息
*/
int recomposeReaderInf( Reader * p_newReaderInf ,char * gender_string )
{
	//判断输入是否规范，不含有‘/’，长度合适（好像不用考虑，因为有gui）？
	if (strchr(p_newReaderInf->account, '/') || 
		strchr(p_newReaderInf->eMail, '/') || strchr(p_newReaderInf->WorkPlace, '/') ) {
		return 1;
	}
	//性别的输入是否规范
	if (strcmp(gender_string, "Male") && strcmp(gender_string, "Female")) {
		return 2;
	}
	else if (strcmp(gender_string, "Male") == 0) {
		p_newReaderInf->gender = Male;
	}
	else if (strcmp(gender_string, "Female") == 0) {
		p_newReaderInf->gender = Female;
	}
	//判断account是否有人使用
	{
		if (GetReaderInf(p_newReaderInf->account) != NULL &&
			strcmp(active_account.account, p_newReaderInf->account)) {
			return 3;
		}
	}
	//全部正确，开始更改
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
	//登录状中的account更改
	strcpy(active_account.account, p_newReaderInf->account);
	//重写文件
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

/*创建新管理员
*成功，				返回0
*输入不规范,包含'/'	返回1
*account已经被占用	返回2
*密码不安全			返回3
*两次密码输入不同	返回4
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
