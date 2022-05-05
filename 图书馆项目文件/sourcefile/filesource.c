#include"lendrecord.h"
#include"bookmanagement.h"
#include"filesource.h"
#include"search.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

//typedef struct {
//	int BookNO;		//图书编号
//	char name[NAME_LEN_MAX];
//	char keyword[5][WORD_LEN_MAX];//最多5个关键字
//	char author[3][WORD_LEN_MAX];//最多3人
//	char press[NAME_LEN_MAX];
//	int publishYear;
//	int totalNum;	//总共有多少本
//	int currentNum;		//现有多少本未借出
//}Book;
//struct book_node {
//	struct book_node * prev;
//	Book book;
//	struct book_node * next;
//};
struct book_node* read_BookInfo(void)
{
	FILE * p_file_book = fopen(BookFileSource, "r");
	if (p_file_book == NULL)
	{
		printf("Open book file ERROR\n");
		exit(1);
	}
	struct book_node* p_node, *head=NULL, *tail=NULL;
	char line[2 * NAME_LEN_MAX + 8 * WORD_LEN_MAX + 15];
	fgets(line, 2 * NAME_LEN_MAX + 8 * WORD_LEN_MAX + 15, p_file_book);
	while (!feof(p_file_book)&& fgets(line, 2 * NAME_LEN_MAX + 8 * WORD_LEN_MAX + 15, p_file_book)!= NULL)
	{
		

		//有时会多出莫名其妙的一行？？
		if (strlen(line) < 14)	break;
		p_node = (struct book_node*)malloc(sizeof(struct book_node));
		//将一行字符数据转化为Book结构
		char *location[14];
		location[0] = line;
		for (int i = 1; i < sizeof(location) / sizeof(char*); i++) {
			location[i] = strchr(location[i - 1], '/');
			if (location[i] == NULL) {
				printf("文件中有格式错误，process: read_BookInfo()\n");
				exit(1);
			}
			*location[i] = '\0';
			location[i]++;
		}
		if (strchr(location[13], '\n') != NULL)
			*strchr(location[13], '\n') = '\0';
		p_node->book.BookNO = translate_StrToNum(location[0]);
		strcpy_s(p_node->book.name, NAME_LEN_MAX, location[1]);
		strcpy_s(p_node->book.keyword[0], WORD_LEN_MAX, location[2]);
		strcpy_s(p_node->book.keyword[1], WORD_LEN_MAX, location[3]);
		strcpy_s(p_node->book.keyword[2], WORD_LEN_MAX, location[4]);
		strcpy_s(p_node->book.keyword[3], WORD_LEN_MAX, location[5]);
		strcpy_s(p_node->book.keyword[4], WORD_LEN_MAX, location[6]);
		strcpy_s(p_node->book.author[0], WORD_LEN_MAX, location[7]);
		strcpy_s(p_node->book.author[1], WORD_LEN_MAX, location[8]);
		strcpy_s(p_node->book.author[2], WORD_LEN_MAX, location[9]);
		strcpy_s(p_node->book.press, NAME_LEN_MAX, location[10]);
		p_node->book.publishYear = (int)translate_StrToNum(location[11]);
		p_node->book.totalNum = (int)translate_StrToNum(location[12]);
		p_node->book.currentNum = (int)translate_StrToNum(location[13]);



		p_node->next = NULL;

		if (head == NULL) {
			head = p_node;
			p_node->prev = NULL;
		}
		else {
			tail->next = p_node;
			p_node->prev = tail;
		}
		tail = p_node;
	}
	fclose (p_file_book);
	return head;
}

int write_BookInfo(struct book_node* book_list_head)
{
	FILE* p_file_book = fopen(BookFileSource, "w");
	struct book_node* p_node = book_list_head;
	fprintf(p_file_book,
		"NO./NAME/KEYWORD1/KEYWORD2/KEYWORD3/KEYWORD4/KEYWORD5/AUTHOR1/AUTHOR2/AUTHOR3/PRESS/YEAR/TOTAL/CURRENT\n");
	for ( ; p_node != NULL; )
	{
		fprintf(p_file_book, "%d/%s/%s/%s/%s/%s/%s/%s/%s/%s/%s/%d/%d/%d\n",
			p_node->book.BookNO,     p_node->book.name,        p_node->book.keyword[0], p_node->book.keyword[1],
			p_node->book.keyword[2], p_node->book.keyword[3],  p_node->book.keyword[4], p_node->book.author[0],
			p_node->book.author[1],  p_node->book.author[2],   p_node->book.press,      p_node->book.publishYear,
			p_node->book.totalNum,   p_node->book.currentNum);
		/*struct book_node* q = p_node;*/
		p_node = p_node->next;
	/*	free(q);*/
	}
	fclose(p_file_book);
	return 0;
}

struct lendrecord_node* read_LendRecord(void)
{
	FILE* p_file_record = fopen(LendRecordFileSource, "r");
	
	if (p_file_record == NULL)
	{
		printf("打开record文件失败\n");
		exit(1);
	}
	struct lendrecord_node *head = NULL, *tail = NULL, *p_node = NULL;
	char line[9 * INT_MAX_LEN + WORD_LEN_MAX+20];
	fgets(line, sizeof(line)/sizeof(char), p_file_record);//先读取说明行

	while (!feof(p_file_record)&& fgets(line, sizeof(line) / sizeof(char), p_file_record)!= NULL) {
		p_node = (struct lendrecord_node*)malloc(sizeof(struct lendrecord_node));

		
		if (strlen(line) < 6)		break;

		//将读取的字符串转化为数据
		char * location[6];
		location[0] = line;
		for (int i = 1; i < sizeof(location) / sizeof(char*); i++) {
			location[i] = strchr(location[i - 1], '/');
			if (location[i] == NULL) {
				printf("文件中有格式错误，process: read_LendRecord()\n");
				exit(1);
			}
			*location[i] = '\0';
			location[i]++;
		}
		if (strchr(location[5], '\n') != NULL)
			*strchr(location[5], '\n') = '\0';
		p_node->record.BookNO = (int)translate_StrToNum(location[0]);
		strcpy(p_node->record.reader_account, location[1]);
		p_node->record.lendTime = translate_StrToNum(location[2]);
		p_node->record.ddlTime = translate_StrToNum(location[3]);
		p_node->next = NULL;

		if (head == NULL) {
			head = p_node;
			p_node->prev = NULL;
		}
		else {
			tail->next = p_node;
			p_node->prev = tail;
		}
		tail = p_node;
	}
	fclose(p_file_record);
	return head;
}

int write_LendRecord(struct lendrecord_node* record_list_head)
{
	FILE* p_file_record = fopen(LendRecordFileSource, "w");
	struct lendrecord_node *p_node = record_list_head;
	fprintf(p_file_record, 
		"BOOK_NO/READER_ACCOUNT/INT_TIME_LEND/INT_TIME_DDL/LEND_Y-M-D/DDL_Y-M-D\n");
	while (p_node != NULL)
	{
		struct tm Lend_date = *localtime(&p_node->record.lendTime);
		struct tm DDl_date = *localtime(&p_node->record.ddlTime);

		char l_date[40], d_date[40];
		strftime(l_date, 40,"%F", &Lend_date);
		strftime(d_date, 40, "%F", &DDl_date);

		fprintf(p_file_record, "%d/%s/%lld/%lld/%s/%s\n",
			p_node->record.BookNO,	p_node->record.reader_account,	
			p_node->record.lendTime, p_node->record.ddlTime, l_date,  d_date);
		//struct lendrecord_node* q = p_node;
		p_node = p_node->next;
		//free(q);
	}
	fclose(p_file_record);
	return 0;
}
//struct history_record {
//	int BookNO;
//	char reader_account[WORD_LEN_MAX];
//	time_t lendTime;
//	time_t ddlTime;
//	time_t actual_ReturnTime;
//};
//struct historyrecord_node {
//	struct hostoryrecord_node * prev;
//	struct history_record history;
//	struct hostoryrecord_node * next;
//};

struct historyrecord_node* read_HistoryRecord()
{
	FILE* p_file_history = fopen(HistoryRecordFileSource, "r");
	if (p_file_history == NULL) {
		printf("ERROR, open history record file\n");
		exit(1);
	}
	struct historyrecord_node *head = NULL, *tail = NULL, *p_node = NULL;
	char line[4 * INT_MAX_LEN + WORD_LEN_MAX];
	fgets(line, sizeof(line)/sizeof(char), p_file_history);
	while (!feof(p_file_history)&& fgets(line, sizeof(line) / sizeof(char), p_file_history)!= NULL)
	{
		;
		if (strlen(line) < 7)	break;
		p_node = (struct historyrecord_node*)malloc(sizeof(struct historyrecord_node));
		//数据格式转换
		char * location[8];
		location[0] = line;
		for (int i = 1; i < sizeof(location)/sizeof(char*); i++) {
			location[i] = strchr(location[i - 1], '/');
			if (location[i] == NULL) {
				printf("文件中有格式错误，process: read_HistoryRecord()\n");
				exit(1);
			}
			*location[i] = '\0';
			location[i]++;
		}
		if (strchr(location[7], '\n') != NULL)
			*strchr(location[7], '\n') = '\0';
		p_node->history.BookNO = translate_StrToNum(location[0]);
		strcpy(p_node->history.reader_account, location[1]);
		p_node->history.lendTime = translate_StrToNum(location[2]);
		p_node->history.ddlTime = translate_StrToNum(location[3]);
		p_node->history.actual_ReturnTime = translate_StrToNum(location[4]);
		p_node->next = NULL;

		if (head == NULL) {
			head = p_node;
			p_node->prev = NULL;
		}
		else {
			tail->next = p_node;
			p_node->prev = tail;
		}
		tail = p_node;

	}
	fclose(p_file_history);
	return head;
}

int write_HistoryRecord(struct historyrecord_node* history_list_head)
{
	FILE* p_file_history = fopen(HistoryRecordFileSource, "w");
	struct historyrecord_node *p_node = history_list_head;
	fprintf(p_file_history, 
		"BOOK_NO/RADER_ACCOUNT/INT_TIME_LEND/INT_DDL_TIME/INT_RETURN_TIME/LEND_Y-M-D/DDL_Y-M-D/RETURN_Y-M-D\n");
	while (p_node != NULL) 
	{
		struct tm Lend_date = *localtime(&p_node->history.lendTime);
		struct tm DDl_date = *localtime(&p_node->history.ddlTime);
		struct tm re_date = *localtime(&p_node->history.actual_ReturnTime);

		char l_date[40], d_date[40], r_date[40];
		strftime(l_date, 40, "%F", &Lend_date);
		strftime(d_date, 40, "%F", &DDl_date);
		strftime(r_date, 40, "%F", &re_date);
		fprintf(p_file_history, "%d/%s/%lld/%lld/%lld/%s/%s/%s\n",
			p_node->history.BookNO, p_node->history.reader_account, p_node->history.lendTime, p_node->history.ddlTime,
			p_node->history.actual_ReturnTime,	l_date, d_date,r_date);
		//struct historyrecord_node* q = p_node;
		p_node = p_node->next;
		//free(q);
	}
	fclose(p_file_history);
	return 0;
}
//typedef struct {
//	char account[WORD_LEN_MAX];
//	char password[PASSWORD_LEN_MAX];
//	Gender gender;
//	char WorkPlace[NAME_LEN_MAX];
//	char eMail[WORD_LEN_MAX];
//}Reader;
//struct reader_node {
//	struct reader_node * prev;
//	Reader reader;
//	struct reader_node * next;
//};
struct reader_node* read_ReaderInfo()
{
	FILE* p_file_reader = fopen(ReadersFileSource, "r");
	if (p_file_reader == NULL)
	{
		printf("ERROR, open readers file\n");
		exit(1);
	}
	struct reader_node* head = NULL, *tail = NULL, *p_node = NULL;
	char line[WORD_LEN_MAX + PASSWORD_LEN_MAX + 10 + NAME_LEN_MAX + WORD_LEN_MAX + 10];
	fgets(line, WORD_LEN_MAX + PASSWORD_LEN_MAX + 10 + NAME_LEN_MAX + WORD_LEN_MAX + 10, p_file_reader);

	while (!feof(p_file_reader)&&
		fgets(line, WORD_LEN_MAX + PASSWORD_LEN_MAX + 10 + NAME_LEN_MAX + WORD_LEN_MAX + 10, p_file_reader)!= NULL)
	{
		
		if (strlen(line) < 10)	break;
		p_node = (struct reader_node*)malloc(sizeof(struct reader_node));

		char* location[5];
		location[0] = line;
		for (int i = 1; i < sizeof(location) / sizeof(char*); i++) {
			location[i] = strchr(location[i-1], '/');
			if (location[i] == NULL) {
				printf("格式错误，in readers file");
				exit(1);
			
			}
			*location[i] = '\0';
			location[i]++;
		}
		if (strchr(location[4], '\n') != NULL)
			*strchr(location[4], '\n') = '\0';

		strcpy(p_node->reader.account, location[0]);
		strcpy(p_node->reader.password, location[1]);
		if (strcmp(location[2], "Male") == 0)
			p_node->reader.gender = Male;
		else if (strcmp(location[2], "Female") == 0)
			p_node->reader.gender = Female;
		else {
			printf("格式错误，in readers file");
			exit(1);
		}
		strcpy(p_node->reader.WorkPlace, location[3]);
		strcpy(p_node->reader.eMail, location[4]);

		p_node->next = NULL;

		if (head == NULL) {
			head = p_node;
			p_node->prev = NULL;
		}
		else {
			tail->next = p_node;
			p_node->prev = tail;
		}
		tail = p_node;
	}
	fclose(p_file_reader);
	return head;
}
int write_ReaderInfo(struct reader_node * reader_list_head) 
{
	FILE* p_file_reader = fopen(ReadersFileSource, "w");
	struct reader_node* p_node = reader_list_head;
	fprintf(p_file_reader, "ACCOUNT/PASSWORD/GENDER/WORKPLACE/E-MAIL\n");
	while (p_node != NULL)
	{
		char sex[10];
		if (p_node->reader.gender == Male)
			strcpy(sex, "Male");
		else if (p_node->reader.gender == Female)
			strcpy(sex, "Female");
		fprintf(p_file_reader, "%s/%s/%s/%s/%s\n",
			p_node->reader.account, p_node->reader.password, sex, p_node->reader.WorkPlace, p_node->reader.eMail);
		//struct reader_node* q = p_node;
		p_node = p_node->next;
		//free(q);
	}
	fclose(p_file_reader);
	return 0;
}
//struct administrator_node {
//	struct administrator_node * prev;
//	Administrator administrator;
//	struct administrator_node * next;
//};
//typedef struct {
//	char AD_account[WORD_LEN_MAX];
//	char AD_password[PASSWORD_LEN_MAX];
//}Administrator;

struct AD_node* read_manager_Info() 
{
	FILE* p_file_ad = fopen(AdministratorFileSource, "r");
	if (p_file_ad == NULL) {
		printf("ERROR, OPEN file administrator\n");
		exit(1);
	}
	struct AD_node* head = NULL, *tail = NULL, *p_node = NULL;
	char line[WORD_LEN_MAX + PASSWORD_LEN_MAX + 10];
	fgets(line, WORD_LEN_MAX + PASSWORD_LEN_MAX + 10, p_file_ad);
	while (!feof(p_file_ad)&& fgets(line, WORD_LEN_MAX + PASSWORD_LEN_MAX + 10, p_file_ad)!= NULL)
	{
		
		if (strlen(line) < 4)	break;
		p_node = (struct AD_node*)malloc(sizeof(struct AD_node));

		char* location[2];
		location[0] = line;
		location[1] = strchr(line, '/');
		if (location[1] == NULL) {
			printf("格式错误，in readers file");
			exit(1);
		}
		*location[1] = '\0';
		location[1]++;
		if (strchr(location[1], '\n') != NULL)
			*strchr(location[1], '\n') = '\0';


		strcpy(p_node->administrator.AD_account, location[0]);
		strcpy(p_node->administrator.AD_password, location[1]);
		p_node->next = NULL;

		if (head == NULL) {
			head = p_node;
			p_node->prev = NULL;
		}
		else {
			tail->next = p_node;
			p_node->prev = tail;
		}
		tail = p_node;

	}
	fclose(p_file_ad);
	return head;
}
int write_AdministratorInfo(struct AD_node* administrator_list_head)
{
	FILE* p_file_ad = fopen(AdministratorFileSource, "w");
	struct AD_node *p_node = administrator_list_head;
	fprintf(p_file_ad, "AD_account/AD_password\n");
	while (p_node != NULL)
	{
		fprintf(p_file_ad, "%s/%s\n", 
			p_node->administrator.AD_account, p_node->administrator.AD_password);
		//struct administrator_node *q = p_node;
		p_node = p_node->next;
		//free(q);
	}
	fclose(p_file_ad);
	return 0;
}
void free_all_list()
{
	//free_reader_list();
	//free_AD_list();
	//free_lendrecord_list();
	//free_historyrecord_list();
	//free_book_list();
}
long long int translate_StrToNum(const char* numberString)
{
	long long int ret = 0;
	for (int i = 0; numberString[i] != '\0' && numberString[i] != '\n'; i++) {
		if (numberString[i] >= '0'&&numberString[i] <= '9')
			ret = 10 * ret + numberString[i] - '0';
		else
		{
			return -1;
		}
	}
	return ret;
}