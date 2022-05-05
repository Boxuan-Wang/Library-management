#include"lendrecord.h"
#include"bookmanagement.h"
#include"filesource.h"
#include"search.h"
#include"users.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


static struct book_node* book_list_head = NULL;
static struct book_node* book_list_tail = NULL;
static struct lendrecord_node *lendrecord_list_head = NULL;
static struct lendrecord_node *lendrecord_list_tail = NULL;
static struct historyrecord_node* history_list_head = NULL;
static struct historyrecord_node* history_list_tail = NULL;
static unsigned int focus_book_no = 0;
static int lend_time_limit_day = 14;//借阅的时间限制
static int lend_number_limit = 5;//借阅数量限制

int ADaddbook(Book* p_newbook, int cnt) {
	if (book_list_head == NULL) {
		book_list_head = read_BookInfo();
	}
	//输入检查
	{
		if (strchr(p_newbook->keyword[0], '/') ||
			strchr(p_newbook->keyword[1], '/') || strchr(p_newbook->keyword[2], '/') ||
			strchr(p_newbook->keyword[3], '/') || strchr(p_newbook->keyword[4], '/') ||
			strchr(p_newbook->author[0], '/') || strchr(p_newbook->author[1], '/') ||
			strchr(p_newbook->author[2], '/') || strchr(p_newbook->press, '/') ||
			strchr(p_newbook->name, '/'))
			return 1;
	}
	int found = 0;
	struct book_node* p_book = book_list_head;
	for (; p_book != NULL; p_book = p_book->next) {
		if (p_book->book.BookNO == p_newbook->BookNO) {
			found = 1;
			break;
		}
		if (p_book->next == NULL) {
			break;
		}
	}
	if (found == 1) {
		p_book->book.totalNum += cnt;
		p_book->book.currentNum += cnt;
	}
	else if (found == 0) {
		p_book->next = (struct book_node*)malloc(sizeof(struct book_node));
		p_book->next->prev = p_book;
		
		p_book->next->next = NULL;
		p_book->next->book = *p_newbook;
		p_book->next->book.totalNum = cnt;
		p_book->next->book.currentNum = cnt;
	}

	write_BookInfo(book_list_head);
	return 0;
}

int ADdeleteBook(int BookNO, int cnt)
{
	if (book_list_head == NULL) {
		book_list_head = read_BookInfo();
	}
	int found = 0;
	struct book_node* p_book = book_list_head;
	for (; p_book != NULL; p_book = p_book->next) {
		if (BookNO == p_book->book.BookNO) {
			found = 1;
			break;
		}
	}
	if (found == 0) {
		return 2;
	}
	if (p_book->book.currentNum < cnt) {
		return 1;
	}
	p_book->book.currentNum -= cnt;
	p_book->book.totalNum -= cnt;
	//若总数减为零，则删除节点
	if (p_book->book.totalNum == 0) {
		if (p_book->prev != NULL)
			p_book->prev->next = p_book->next;
		else
			if (p_book->next == NULL)
				book_list_head = NULL;
		if (p_book->next != NULL) 
			p_book->next->prev = p_book->prev;
		free(p_book);
	}

	write_BookInfo(book_list_head);
	return 0;
}

int ADrecomposeBook(Book * p_newInfofBook)
{
	if (book_list_head == NULL) {
		book_list_head = read_BookInfo();
	}
	{
		if (strchr(p_newInfofBook->keyword[0], '/') ||
			strchr(p_newInfofBook->keyword[1], '/') || strchr(p_newInfofBook->keyword[2], '/') ||
			strchr(p_newInfofBook->keyword[3], '/') || strchr(p_newInfofBook->keyword[4], '/') ||
			strchr(p_newInfofBook->author[0], '/') || strchr(p_newInfofBook->author[1], '/') ||
			strchr(p_newInfofBook->author[2], '/') || strchr(p_newInfofBook->press, '/') ||
			strchr(p_newInfofBook->name, '/'))
			return 2;
	}
	int found = 0;
	struct book_node* p_book = book_list_head;
	for (; p_book != NULL; p_book = p_book->next) {
		if (p_newInfofBook->BookNO == p_book->book.BookNO) {
			found = 1;
			break;
		}
	}
	if (found == 0) {
		return 1;
	}
	strcpy(p_book->book.name, p_newInfofBook->name);
	strcpy(p_book->book.press, p_newInfofBook->press);
	strcpy(p_book->book.keyword[0], p_newInfofBook->keyword[0]);
	strcpy(p_book->book.keyword[1], p_newInfofBook->keyword[1]);
	strcpy(p_book->book.keyword[2], p_newInfofBook->keyword[2]);
	strcpy(p_book->book.keyword[3], p_newInfofBook->keyword[3]);
	strcpy(p_book->book.keyword[4], p_newInfofBook->keyword[4]);
	strcpy(p_book->book.author[0], p_newInfofBook->author[0]);
	strcpy(p_book->book.author[1], p_newInfofBook->author[1]);
	strcpy(p_book->book.author[2], p_newInfofBook->author[2]);
	p_book->book.publishYear = p_newInfofBook->publishYear;

	write_BookInfo(book_list_head);
	return 0;
}

int ReaderLend(int BookNO, char * ReaderAccount)
{
	if (book_list_head == NULL) {
		book_list_head = read_BookInfo();
	}
	for (book_list_tail = book_list_head; book_list_tail != NULL &&
		book_list_tail->next != NULL;
		book_list_tail = book_list_tail->next);
	if (lendrecord_list_head == NULL) {
		lendrecord_list_head = read_LendRecord();
	}
	for (lendrecord_list_tail = lendrecord_list_head; lendrecord_list_tail != NULL &&
		lendrecord_list_tail->next != NULL;
		lendrecord_list_tail = lendrecord_list_tail->next);
	//找到书籍，判断是否有余量
	if (GetBookInf(BookNO) == NULL || GetBookInf(BookNO)->currentNum ==0) {
		return 1;
	}
	//判断读者已经借了这本书
	{
		int lent = 0;
		for (struct lendrecord_node * p_record = lendrecord_list_head;
			p_record != NULL; p_record = p_record->next) {
			if (p_record->record.BookNO == BookNO && 
				strcmp(ReaderAccount, p_record->record.reader_account) == 0) {
				lent = 1;
				break;
			}
		}
		if (lent == 1)
			return 2;
	}
	//判断是否借书数量达到上限
	if (LendNumber(ReaderAccount) >= lend_number_limit) {
		return 3;
	}
	//符合条件
	struct book_node * p_target_book = NULL;
	for (p_target_book = book_list_head; p_target_book != NULL; p_target_book = p_target_book->next) {
		if (p_target_book->book.BookNO == BookNO) {
			p_target_book->book.currentNum--;
			create_record(ReaderAccount, BookNO);
			break;
		}
	}
	write_BookInfo(book_list_head);
	return 0;
}

int ReaderReturn(int BookNO, char* ReaderAccount) {
	if (book_list_head == NULL) {
		book_list_head = read_BookInfo();
	}
	if (lendrecord_list_head == NULL) {
		lendrecord_list_head = read_LendRecord();
	}
	for (lendrecord_list_tail = lendrecord_list_head; lendrecord_list_tail != NULL &&
		lendrecord_list_tail->next != NULL;
		lendrecord_list_tail = lendrecord_list_tail->next);

	struct lendrecord_node * p_record = NULL;
	for (p_record = lendrecord_list_head; p_record != NULL; p_record = p_record->next) {
		if (BookNO == p_record->record.BookNO &&
			strcmp(ReaderAccount, p_record->record.reader_account) == 0) {
			break;
		}
	}
	if (p_record == NULL) {
		return 1;
	}

	finish_record(ReaderAccount, BookNO);
	struct book_node * p_target_book = NULL;
	for (p_target_book = book_list_head; p_target_book != NULL; p_target_book = p_target_book->next) {
		if (p_target_book->book.BookNO == BookNO) {
			p_target_book->book.currentNum++;
		}
	}
	write_BookInfo(book_list_head);
	return 0;

}

Book * GetBookInf(int num)
{
	return find_num(num);
}

void change_focusbook(int BookNO) {
	focus_book_no = BookNO;
}
int get_focusbook(void) {
	return focus_book_no;
}

int get_lendTimeLimit(void)
{
	return lend_time_limit_day;
}

void change_lendTimeLimit(int new_limit)
{
	lend_time_limit_day = new_limit;
}

int get_lendNumLimit(void)
{
	return lend_number_limit;
}

void change_lendNUMLimit(int new_limit)
{
	lend_number_limit = new_limit;
}


int create_record(char* CurrentReaderAccount, int BookNO){
	if (lendrecord_list_head == NULL) {
		lendrecord_list_head = read_LendRecord();
	}
	for (lendrecord_list_tail = lendrecord_list_head; lendrecord_list_tail != NULL && 
		lendrecord_list_tail->next != NULL;		
		lendrecord_list_tail = lendrecord_list_tail->next);
	struct lendrecord_node * p_new_record = (struct lendrecord_node *)malloc(sizeof(struct lendrecord_node));
	p_new_record->record.BookNO = BookNO;
	strcpy(p_new_record->record.reader_account, CurrentReaderAccount);
	time(&p_new_record->record.lendTime);
	p_new_record->record.ddlTime = p_new_record->record.lendTime + time_dayToInt( lend_time_limit_day);
	p_new_record->next = NULL;

	if (lendrecord_list_head == NULL) {
		lendrecord_list_head = p_new_record;
		p_new_record->prev = NULL;
	}
	else {
		lendrecord_list_tail->next = p_new_record;
		p_new_record->prev = lendrecord_list_tail;
	}
	lendrecord_list_tail = p_new_record;

	write_LendRecord(lendrecord_list_head);
	return 0;
}

int finish_record(char* CurrentReaderAccount, int BookNO) {
	if (lendrecord_list_head == NULL) {
		lendrecord_list_head = read_LendRecord();
	}
	for (lendrecord_list_tail = lendrecord_list_head; lendrecord_list_tail != NULL &&
		lendrecord_list_tail->next != NULL;
		lendrecord_list_tail = lendrecord_list_tail->next);
	if (history_list_head == NULL) {
		history_list_head = read_HistoryRecord();
	}
	for (history_list_tail = history_list_head; history_list_tail != NULL &&
		history_list_tail->next != NULL;
		history_list_tail = history_list_tail->next);

	struct lendrecord_node * p_record = NULL;
	for (p_record = lendrecord_list_head; p_record != NULL; p_record = p_record->next) {
		if (BookNO==p_record->record.BookNO &&
			strcmp(CurrentReaderAccount, p_record->record.reader_account) == 0) {
			break;
		}
	}
	if (p_record != NULL) {
		//复制信息
		struct historyrecord_node * p_new_history =
			(struct historyrecord_node *)malloc(sizeof(struct historyrecord_node));
		p_new_history->history.BookNO = p_record->record.BookNO;
		strcpy(p_new_history->history.reader_account, p_record->record.reader_account);
		p_new_history->history.lendTime = p_record->record.lendTime;
		p_new_history->history.ddlTime = p_record->record.ddlTime;
		time(&p_new_history->history.actual_ReturnTime);
		p_new_history->next = NULL;
		//删除record链表里对应的节点
		if (p_record == lendrecord_list_head) {
			lendrecord_list_head = lendrecord_list_head->next;
			free(p_record);
		}
		else if (p_record == lendrecord_list_tail) {
			lendrecord_list_tail = lendrecord_list_tail->prev;
			lendrecord_list_tail->next = NULL;
			free(p_record);
		}
		else {
			p_record->prev->next = p_record->next;
			p_record->next->prev = p_record->prev;
			free(p_record);
		}
		//在history中创建新节点
		if (history_list_head == NULL) {
			history_list_head = p_new_history;
			p_new_history->prev = NULL;
		}
		else {
			history_list_tail->next = p_new_history;
			p_new_history->prev = history_list_tail;
		}
		history_list_tail = p_new_history;

		write_LendRecord(lendrecord_list_head);
		write_HistoryRecord(history_list_head);
		return 0;
	}
}

int LendNumber(char* CurrentReaderAccount) {
	if (lendrecord_list_head == NULL) {
		lendrecord_list_head = read_LendRecord();
	}
	for (lendrecord_list_tail = lendrecord_list_head; lendrecord_list_tail != NULL &&
		lendrecord_list_tail->next != NULL;
		lendrecord_list_tail = lendrecord_list_tail->next);
	int ret = 0;
	struct lendrecord_node * p_record = NULL;
	for (p_record = lendrecord_list_head; p_record != NULL; p_record = p_record->next) {
		if (strcmp(CurrentReaderAccount, p_record->record.reader_account) == 0) {
			ret++;
		}
	}
	return ret;
}

struct lendrecord_node * GetLentBooks(char * CurrentReaderAccount) {
	if (lendrecord_list_head == NULL) {
		lendrecord_list_head = read_LendRecord();
	}
	for (lendrecord_list_tail = lendrecord_list_head; lendrecord_list_tail != NULL &&
		lendrecord_list_tail->next != NULL;
		lendrecord_list_tail = lendrecord_list_tail->next);

	struct lendrecord_node * result_list_head = NULL;
	struct lendrecord_node * result_list_tail = NULL;
	struct lendrecord_node * p_result = NULL;
	struct lendrecord_node * p_record = NULL;
	for (p_record = lendrecord_list_head; p_record != NULL; p_record = p_record->next) {
		if (strcmp(CurrentReaderAccount, p_record->record.reader_account) == 0) {
			p_result = (struct book_node *)malloc(sizeof(struct book_node));
			p_result->record= p_record->record;
			p_result->next = NULL;

			if (result_list_head == NULL) {
				result_list_head = p_result;
				p_result->prev = NULL;
			}
			else {
				result_list_tail->next = p_result;
				p_result->prev = result_list_tail;
			}
			result_list_tail = p_result;

		}
	}
	return result_list_head;
}