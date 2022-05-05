#include"statics.h"
#include "users.h"
#include"lendrecord.h"
#include"bookmanagement.h"
#include"filesource.h"

#include "imgui.h"
#include "graphics.h"

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

static struct book_node* book_list_head = NULL;
static struct reader_node * reader_list_head = NULL;
static struct lendrecord_node *lendrecord_list_head = NULL;
static struct historyrecord_node* history_list_head = NULL;

static int fy, fm, fd;
static int ty, tm, td;

void read_doccument(void)
{
	free(book_list_head);
	free(reader_list_head);
	free(lendrecord_list_head);
	free(history_list_head);

	book_list_head = read_BookInfo();
	reader_list_head = read_ReaderInfo();
	lendrecord_list_head = read_LendRecord();
	history_list_head = read_HistoryRecord();
}

int GetvolumeofBooks(void)
{
	struct book_node*  head_node, *p_node;
	int volumeofBooks = 0;
	head_node = book_list_head;

	for (p_node = head_node; p_node != NULL; p_node = p_node->next)
		volumeofBooks += p_node->book.totalNum;

	return volumeofBooks;
}

int GetvarietyofBooks(void)
{
	struct book_node*  head_node, *p_node;
	int varietyofBooks = 0;
	head_node = book_list_head;

	for (p_node = head_node; p_node != NULL; p_node = p_node->next)
		varietyofBooks++;

	return varietyofBooks;
}

int GetnumberofUsers(void)
{
	struct reader_node*  head_node = NULL, *p_node;
	int numberofUsers = 0;
	head_node = reader_list_head;
	for (p_node = head_node; p_node != NULL; p_node = p_node->next)
		numberofUsers++;

	return numberofUsers;
}

int check_Date(char* fromY, char* fromM, char* fromD, char* toY, char* toM, char* toD)
{
	fy = check_String(fromY); fm = check_String(fromM); fd = check_String(fromD);
	ty = check_String(toY); tm = check_String(toM); td = check_String(toD);
	if (!(fy && fm && fd && ty && tm && td))
		return 0;
	if (fy < 2000 || fy > 2020 || ty < 2000 || ty > 2020 || fm > 12 || tm > 12 || fd > 31 || td > 31)
		return 0;
	if (fy > ty)
		return 0;
	if (fy == ty && fm > tm)
		return 0;
	if (fy == fy && fm == tm && fd > td)
		return 0;
	return 1;
}

int check_String(char* string)
{
	char* p = string;
	int number = 0;
	while (*p != '\0') {
		if (*p >= '0' && *p <= '9')
			number = *p - '0' + number * 10;
		else {
			return 0;
		}
		p++;
	}
	return number;
}

int Get_numberofLend(void)
{
	struct lendrecord_node * lend_list_head = lendrecord_list_head, *p_lend_node;
	struct historyrecord_node * return_list_head = history_list_head, *p_return_node;
	int number = 0;
	struct tm f_time_ymd = { 0, 0, 0, fd, fm - 1, fy - 1900 };
	struct tm t_time_ymd = { 0, 0, 0, td, tm - 1, ty - 1900 };
	time_t  f_time_sec = mktime(&f_time_ymd);
	time_t  t_time_sec = mktime(&t_time_ymd) + 3600 * 24 - 1;
	for (p_lend_node = lend_list_head; p_lend_node != NULL; p_lend_node = p_lend_node->next) {
		if (p_lend_node->record.lendTime >= f_time_sec && p_lend_node->record.lendTime <= t_time_sec) {
			number++;
		}
	}
	for (p_return_node = return_list_head; p_return_node != NULL; p_return_node = p_return_node->next) {
		if (p_return_node->history.lendTime >= f_time_sec && p_return_node->history.lendTime <= t_time_sec) {
			number++;
		}
	}
	return number;
}

int Get_numberofReturn(void)
{
	struct historyrecord_node * return_list_head = history_list_head, *p_return_node;
	int number = 0;
	struct tm f_time_ymd = { 0, 0, 0, fd, fm - 1, fy - 1900 };
	struct tm t_time_ymd = { 0, 0, 0, td, tm - 1, ty - 1900 };
	time_t  f_time_sec = mktime(&f_time_ymd);
	time_t  t_time_sec = mktime(&t_time_ymd) + 3600 * 24 - 1;
	for (p_return_node = return_list_head; p_return_node != NULL; p_return_node = p_return_node->next) {
		if (p_return_node->history.actual_ReturnTime >= f_time_sec
			&& p_return_node->history.actual_ReturnTime <= t_time_sec) {
			number++;
		}
	}
	return number;
}

int check_Date_interval(void)
{
	if ((ty - fy) * 12 + tm - fm + 1 > 24)
		return 0;
	return 1;
}

void print_statisticalChart(int flag, char * color)
{
	SetPenColor(color);
	//画出横坐标刻度
	int numberofMonth = (ty - fy) * 12 + tm - fm + 1;
	int i, j, k;
	int st_x = 32 - numberofMonth / 2;
	char monthLabel[3];
	for (i = fy, j = fm, k = 0; ; j++, k++) {
		_itoa(j, monthLabel, 10);
		if (j < 10)
			drawLabel(st_x + k + 0.32, 8, monthLabel);
		else
			drawLabel(st_x + k + 0.15, 8, monthLabel);
		if (i == ty && j == tm) {
			break;
		}
		if (j == 12) {
			i++;
			j = j - 12;
		}
	}

	//将每月统计数目写入numberinMonth[numberofMonth]
	int *numberinMonth = (int*)malloc(numberofMonth*(sizeof(int)));
	for (i = 0; i < numberofMonth; i++)
		numberinMonth[i] = 0;
	struct lendrecord_node * lend_list_head = lendrecord_list_head, *p_lend_node;
	struct historyrecord_node * return_list_head = history_list_head, *p_return_node;

	struct tm f_time_ymd = { 0, 0, 0, fd, fm - 1, fy - 1900 };
	struct tm t_time_ymd = { 0, 0, 0, td, tm - 1, ty - 1900 };
	struct tm p_time_ymd;
	time_t f_time_sec = mktime(&f_time_ymd);
	time_t t_time_sec = mktime(&t_time_ymd) + 3600 * 24 - 1;
	if (flag == 2) {
		for (p_lend_node = lend_list_head; p_lend_node != NULL; p_lend_node = p_lend_node->next) {
			if (p_lend_node->record.lendTime >= f_time_sec && p_lend_node->record.lendTime <= t_time_sec) {
				p_time_ymd = *localtime(&p_lend_node->record.lendTime);
				int i = (p_time_ymd.tm_year + 1900 - fy) * 12 + p_time_ymd.tm_mon + 1 - fm;
				numberinMonth[i] ++;
			}
		}
		for (p_return_node = return_list_head; p_return_node != NULL; p_return_node = p_return_node->next) {
			if (p_return_node->history.lendTime >= f_time_sec && p_return_node->history.lendTime <= t_time_sec) {
				p_time_ymd = *localtime(&p_return_node->history.lendTime);
				int i = (p_time_ymd.tm_year + 1900 - fy) * 12 + p_time_ymd.tm_mon + 1 - fm;
				numberinMonth[i] ++;
			}
		}
	}
	if (flag == 3) {
		for (p_return_node = return_list_head; p_return_node != NULL; p_return_node = p_return_node->next) {
			if (p_return_node->history.actual_ReturnTime >= f_time_sec
				&& p_return_node->history.actual_ReturnTime <= t_time_sec) {
				p_time_ymd = *localtime(&p_return_node->history.actual_ReturnTime);
				int i = (p_time_ymd.tm_year + 1900 - fy) * 12 + p_time_ymd.tm_mon + 1 - fm;
				numberinMonth[i] ++;
			}
		}
	}

	for (p_return_node = return_list_head; p_return_node != NULL; p_return_node = p_return_node->next) {
		if (p_return_node->history.lendTime >= f_time_sec && p_return_node->history.lendTime <= t_time_sec) {
			p_time_ymd = *localtime(&p_return_node->history.lendTime);
			int i = (p_time_ymd.tm_year + 1900 - fy) * 12 + p_time_ymd.tm_mon + 1 - fm;
			numberinMonth[i] ++;
		}
	}


	//画出图上的数目标注和柱子
	int max = numberinMonth[0];
	for (i = 0; i < numberofMonth; i++)
		if (numberinMonth[i] > max) {
			max = numberinMonth[i];
		}
	double rate;
	if (max == 0)
		rate = 0;
	else
		rate = 10.0 / max;
	char numberLabel[4];
	for (i = 0, k = 0; i < numberofMonth; i++, k++) {
		_itoa(numberinMonth[i], numberLabel, 10);
		if (numberinMonth[i] < 10)
			drawLabel(st_x + k + 0.32, 8.9 + numberinMonth[i] * rate, numberLabel);
		else
			drawLabel(st_x + k + 0.15, 8.9 + numberinMonth[i] * rate, numberLabel);
		MovePen(st_x + k, 8.7);
		DrawLine(0, numberinMonth[i] * rate);
		DrawLine(1, 0);
		DrawLine(0, -numberinMonth[i] * rate);
	}

	//画横纵轴
	{
		MovePen(st_x - 0.5, 8.7);
		DrawLine(numberofMonth + 1, 0);
		MovePen(st_x + 0.5 + numberofMonth, 8.7);
		DrawLine(-0.3, 0.15);
		MovePen(st_x + 0.5 + numberofMonth, 8.7);
		DrawLine(-0.3, -0.15);

		MovePen(st_x - 0.5, 8.7);
		DrawLine(0, 12);
		MovePen(st_x - 0.5, 20.7);
		DrawLine(-0.15, -0.3);
		MovePen(st_x - 0.5, 20.7);
		DrawLine(0.15, -0.3);

		drawLabel(st_x + numberofMonth, 8, "month");
		drawLabel(st_x - 3, 20, "number");
	}
	//画标题
	{
		char lor_line[7];
		if (flag == 2)
			strcpy(lor_line, "lend");
		else
			strcpy(lor_line, "return");
		char from_line[60], to_line[60];
		sprintf(from_line, "Statistics of %s from %d Y %d M %d D", lor_line, fy, fm, fd);
		drawLabel(st_x + numberofMonth / 2 - 6, 22, from_line);
		sprintf(to_line, "to %d Y %d M %d D", ty, tm, td);
		drawLabel(st_x + numberofMonth / 2 - 0.7, 21, to_line);
	}

	free(numberinMonth);
}