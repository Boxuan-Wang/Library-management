/*读者用户/管理员账户模块
*包含功能，
*1. 读者登录
*2. 读者注册
*3. 读者提取个人信息
*4. 读者修改个人信息
*5. 管理员登录
*6. 新增管理员

*8. 程序运行时，查看登录的状态
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

/*读者登录
*若账户存在，且密码正确，则返回登录的账号；
*若账户存在但密码错误，返回“__wrong/password__”
*没有对应的账号，返回"__no/account__"
*/
char* ReaderLogin(char* account, char* password);

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
int ReaderRegister(Reader* p_newReader,char* passwordCheck,char* gender_string);

/*提取读者信息
*！！使用方法！！ reader=*GetReaderInf(2321@sa);
*提取出读者的信息
*返回Reader结构指针
*失败返回NULL
*/
Reader* GetReaderInf(char* account);

/*读者修改个人信息,其中不包括修改密码
*结构指针中，account，email，workplace有效
*修改成功		返回0
*输入不规范，包含'/'	返回1
*性别输入格式错误		返回2
*account已经被占用		返回3
*根据account找到该读者，其他信息
*/
int recomposeReaderInf( Reader* p_newReaderInf, char * gender_string);

/*读者修改密码
*修改成功		返回0
*未登陆		返回1
*原密码错误		返回2
*密码格式错误，包含了‘/'	返回3
*两次输入不一致返回			返回4
*密码太短或密码未包含任何字母	返回5
*/
int recomposeReaderPassword(char* prev, char* new, char* check);

/*管理员登录
*若账户存在，且密码正确，则返回登录的账号；
*若账户存在但密码错误，返回“__wrong/password__”
*没有对应的账号，返回"__no/account__"
*/
char* manager_login(char * account, char * password);

//获取管理员信息
Administrator* GetAdministratorInf(char* account_AD);

/*创建新管理员
*成功，				返回0
*输入不规范			返回1
*account已经被占用	返回2
*密码不安全			返回3
*两次密码输入不同	返回4
*/
int GenNewAdimistrator(Administrator* p_newAD, char * password_check);

/*检查登录状态
*在其他模块中调用，用来查看登录的状态和活跃的用户account
*返回LoginStatus结构指针
*/
struct LoginStatus* GetLogStatus(void);

//free lists
void free_reader_list();
void free_AD_list();

//登出功能
void reader_logout(void);
void AD_logout(void);
#endif