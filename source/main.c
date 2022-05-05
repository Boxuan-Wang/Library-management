#include "graphics.h"
#include "imgui.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include<string.h>
#include<time.h>		//系统自带时间处理库

#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>

#include"bookmanagement.h"
#include"filesource.h"
#include"lendrecord.h"
#include"search.h"
#include"users.h"
#include"statics.h"
typedef enum 
{
	LogIn,
	Register,
	Search,

	Statistics,
	Reader_Home,		//个人主页
	Reader_ChangePassword,
	Reader_ChangePersonalInfo,
	Reader_Return,		//只有还书
	Reader_History,		//历史借书记录
	Book_Detail,		//屏幕只显示一本书的信息，同时展示借书、还书、续借的按钮
	AD_Management,	//包括新增、删除书籍，更改书籍信息,新建管理员
	Help,
	About_us

}WindowStatus;

static WindowStatus pageType = LogIn;	//标志了要输出那个页面
static char status_line[101] = "start";	//页面左下角的信息反馈标签的内容
static double winwidth, winheight;  //窗口尺寸
static int exist_clock_blink = 0;	//时钟闪烁，是否输出冒号

const char *label_color_style[] = { "blue","black","white","black","black","red","red" };
static int label_color_num = 0;	//setting中更改颜色时，相应地改变label的颜色

void display(void);
void DisplayClear(void);
void startTimer(int id, int timeinterval);



void display_LogIn(void)
{

	double fH = GetFontHeight();
	double h = fH * 2; // 控件高度
	double w = winwidth / 40; // 控件宽度
	//double x = winwidth / 3;
	//double y = winheight / 2 + h+ h;
	SetPenColor(label_color_style[label_color_num]);
	drawLabel(15, 19, " Account:");
	drawLabel(15, 16, "Password:");

	static char account[WORD_LEN_MAX] = "";

	static char password[PASSWORD_LEN_MAX] = "";
	static char passwordTemp[PASSWORD_LEN_MAX] = "";

	textbox(GenUIID(0), 19, 19, 14, h, account, sizeof(account) / sizeof(char));
	//密码输入加密显示
	if (textbox(GenUIID(0), 19, 15.5, 14, h, passwordTemp, sizeof(password) / sizeof(char))) {
		if (strlen(passwordTemp) > strlen(password)) {
			strcat(password, passwordTemp + strlen(passwordTemp) - 1);
			passwordTemp[strlen(passwordTemp) - 1] = '*';
		}
		else if (strlen(passwordTemp) < strlen(password)) {
			strcpy(password + strlen(passwordTemp), "");
		}
	}

	//读者登录功能
	if (button(GenUIID(0), 16.6, 11.5, 4 * w, 1.5*h, "readers login")) {
		char result[50] = "";
		strcpy(result, ReaderLogin(account, password));
		if (strcmp(result, "__wrong/password__") == 0) {
			strcpy(status_line, "Wrong passowrd");
			strcpy(password, "");
			strcpy(passwordTemp, "");
		}
		else if (strcmp(result, "__no/account__") == 0) {
			strcpy(status_line, "Account not registered");
			strcpy(account, "");
			strcpy(password, "");
			strcpy(passwordTemp, "");
		}
		else {
			char temp[101];
			sprintf(temp, "Welcome %s!", result);
			strcpy(status_line, temp);
			strcpy(account, "");
			strcpy(password, "");
			strcpy(passwordTemp, "");
			pageType = Reader_Home;
		}
	}

	if (button(GenUIID(0), 27, 11.5, 4 * w, 1.5*h, "administrator login")) {
		char result[50] = "";
		strcpy(result, manager_login(account, password));
		if (strcmp(result, "__wrong/password__") == 0) {
			strcpy(status_line, "Wrong passowrd");
			strcpy(password, "");
			strcpy(passwordTemp, "");
		}
		else if (strcmp(result, "__no/account__") == 0) {
			strcpy(status_line, "Account not registered");
			strcpy(account, "");
			strcpy(password, "");
			strcpy(passwordTemp, "");
		}
		else {
			char temp[101];
			sprintf(temp, "Welcome Administrator %s!", result);
			strcpy(status_line, temp);
			strcpy(account, "");
			strcpy(password, "");
			strcpy(passwordTemp, "");
			pageType = AD_Management;
		}
	}



	SetPenColor(label_color_style[label_color_num]);
	drawLabel(17, 8, "If you don't have an account?");
	if (button(GenUIID(0), 25, 7.5, 4 * w, 1.5*h, "register")) {
		pageType = Register;
	}


	SetPenColor(label_color_style[label_color_num]);
	drawLabel(17, 6, "Forget your account / password?");
	if (button(GenUIID(0), 25, 5.5, 4 * w, 1.5*h, "Forget"))
		/*忘记密码功能接口*/;
}
void display_Register(void)
{
	double fH = GetFontHeight();
	double h = fH * 2; // 控件高度
	double w = winwidth / 40; // 控件宽度
	SetPenColor(label_color_style[label_color_num]);
	drawLabel(11, 25, " Account:");
	drawLabel(11, 22, "Password:");
	drawLabel(9.5, 19, "Password Check:");
	drawLabel(11, 16, "Gender:");
	drawLabel(11, 13, "Workplace:");
	drawLabel(11, 10, "E-mail:");

	static Reader newreader;
	static char password_check_temp[WORD_LEN_MAX]="",
		password_check[WORD_LEN_MAX]="",
		password_temp[WORD_LEN_MAX]="";
	static char gender_string[10]="Male";
	
	textbox(GenUIID(0), 15, 25, 14, h, newreader.account, WORD_LEN_MAX);
	//对文本输入是的密码加密显示为*
	if (textbox(GenUIID(0), 15, 22, 14, h, password_temp, WORD_LEN_MAX)) {

		if (strlen(password_temp) > strlen(newreader.password)) {
			strcat(newreader.password, password_temp + strlen(password_temp) - 1);
			password_temp[strlen(password_temp) - 1] = '*';
		}
		else if (strlen(password_temp) < strlen(newreader.password)) {
			strcpy(newreader.password + strlen(password_temp), "");
		}
	}
	
	//密码确认同样进行显示加密
	if (textbox(GenUIID(0), 15, 19, 14, h, password_check_temp, WORD_LEN_MAX)) {
		if (strlen(password_check_temp) > strlen(password_check)) {
			strcat(password_check, password_check_temp + strlen(password_check_temp) - 1);
			password_check_temp[strlen(password_check_temp) - 1] = '*';
		}
		else if (strlen(password_check_temp) < strlen(password_check)) {
			strcpy(password_check + strlen(password_check_temp), "");
		}
	}

	//textbox(GenUIID(0), 15, 16, 14, h, gender_string, sizeof(gender_string) / sizeof(char));
	{
		char* gender_list[] = {
			"Male",
			"Female"
		};
		if (strcmp(gender_string, "Male") == 0) {
			gender_list[0] = "Male";
			gender_list[1] = "Female";
		}
		else if (strcmp(gender_string, "Female") == 0) {
			gender_list[1] = "Male";
			gender_list[0] = "Female";
		}
		int selection = menuList(GenUIID(0), 15, 16, 5, 5, h, gender_list, 2);
		if (selection == 1) {
			if (strcmp(gender_string, "Male") == 0) {
				strcpy(gender_string, "Female");
				gender_list[0] = "Male";
				gender_list[1] = "Female";
			}
			else if (strcmp(gender_string, "Female") == 0) {
				strcpy(gender_string, "Male");
				gender_list[1] = "Male";
				gender_list[0] = "Female";
			}
		}
	}
	textbox(GenUIID(0), 15, 13, 14, h, newreader.WorkPlace, NAME_LEN_MAX);
	textbox(GenUIID(0), 15, 10, 14, h, newreader.eMail, WORD_LEN_MAX);

	SetPenColor(label_color_style[label_color_num]);
	drawLabel(31, 16, "Input \"Male\" or \"Female\"");
	drawLabel(31, 22, "Length over 6 and Must include at least a letter");
	if (button(GenUIID(0), 19, 6, 4 * w, 1.5*h, "Register"))
	{
		int register_result= ReaderRegister(&newreader,password_check,gender_string);
		switch (register_result) {
		case 0: 
			strcpy(status_line, "Register Success!"); 
			strcpy(newreader.password, "");
			strcpy(password_temp, "");
			strcpy(password_check, "");
			strcpy(password_check_temp, "");
			strcpy(newreader.WorkPlace, "");
			strcpy(newreader.eMail, "");
			strcpy(newreader.account, "");

			pageType = LogIn; 
			
			break;
		case 1: 
			strcpy(status_line, "Error, '/' can not be used"); 
			strcpy(newreader.password, "");
			strcpy(password_temp, "");
			strcpy(password_check, "");
			strcpy(password_check_temp, "");
			strcpy(newreader.WorkPlace, "");
			strcpy(newreader.eMail, "");
			strcpy(newreader.account, "");

			break;
		case 2: 
			strcpy(status_line, "Wrong input in Gender"); 

			break;
		case 3: 
			strcpy(status_line, "The account name has been used"); 
			strcpy(newreader.account, "");
			break;
		case 4: 
			strcpy(status_line, "Different input of password"); 
			strcpy(newreader.password, "");
			strcpy(password_temp, "");
			strcpy(password_check, "");
			strcpy(password_check_temp, "");
			break;
		case 5: 
			strcpy(status_line, "Safety problems of the password"); 
			strcpy(newreader.password, "");
			strcpy(password_temp, "");
			strcpy(password_check, "");
			strcpy(password_check_temp, "");
			break;
		}

		
	}
}
void display_Search(void)
{
	double fH = GetFontHeight();
	double h = fH * 1.7; // 控件高度
	double w = winwidth / 40; // 控件宽度
	static struct book_node * search_result = NULL;
	static int search_type = 1;	//搜索方式，1 name，2 keyword, 3 author,4 NO.
	static char keyword[NAME_LEN_MAX]="";
	char search_type_string[20];
	char *menuListSearchtype[] = {
		"Search Type",
		"Caption",
		"Keyword",
		"Author",
		"Book NO."
	};
	switch (search_type) {
	case 1: sprintf(search_type_string, "Caption"); break; 
	case 2: sprintf(search_type_string, "Keyword"); break;
	case 3: sprintf(search_type_string, "Author"); break;
	case 4: sprintf(search_type_string, "Book NO."); break;
	}
	drawLabel(0.1*winwidth, winheight - 5 * h + 0.4, search_type_string);
	textbox(GenUIID(0), 0.2*winwidth, winheight - 5 * h, 16, h, keyword, NAME_LEN_MAX);
	{
		int selection = menuList(GenUIID(0), 0.2*winwidth + 14 + 3 + 3 * w + 3, winheight - 5 * h, 3 * w, 3 * w, h,
			menuListSearchtype, sizeof(menuListSearchtype) / sizeof(menuListSearchtype[0]));
		switch (selection) {
		case 1: search_type = 1; sprintf(search_type_string, "Caption"); break;
		case 2: search_type = 2; sprintf(search_type_string, "Keyword"); break;
		case 3: search_type = 3; sprintf(search_type_string, "Author"); break;
		case 4: search_type = 4; sprintf(search_type_string, "Book NO."); break;
		}
	}
	if (button(GenUIID(0), 0.2*winwidth + 14 + 3, winheight - 5 * h, 3 * w, h, "Search")) {
		//每次点击search都free之前的搜索结果链表
		for (struct book_node* p = search_result; p != NULL; ) {
			if (p->next == NULL) {
				free(p);
				break;
			}
			else {
				p = p->next;
				if (p->prev != NULL)
					free(p->prev);
			}
		}
		search_result = NULL;

		switch (search_type) {
		case 1:search_result = search_name(keyword); break;
		case 2:search_result = search_keyword(keyword); break;
		case 3:search_result = search_author(keyword); break;
		case 4: 

			if (translate_StrToNum(keyword) == -1) {
				strcpy(status_line, "Wrong input");
				strcpy(keyword, "");

			}
			else {
				
				if (find_num((int)translate_StrToNum(keyword)) != NULL) {
					search_result = (struct book_node*)malloc(sizeof(struct book_node));
					search_result->next = NULL;
					search_result->prev = NULL;
					search_result->book = *find_num((int)translate_StrToNum(keyword));
				}
			}
			break;
			
		}
	}

	//显示搜索结果的部分
	SetPenColor(label_color_style[label_color_num]);
	drawLabel(0.05*winwidth, winheight - 6 * h, "NO.");
	drawLabel(0.05*winwidth + 2, winheight - 6 * h, "Caption");
	drawLabel(0.05*winwidth + 14, winheight - 6 * h, "Author");
	drawLabel(0.05*winwidth + 23, winheight - 6 * h, "Keyword");
	drawLabel(0.05*winwidth + 36, winheight - 6 * h, "Number");
	if (search_result != NULL) {
		int result_num = 0;
		for (struct book_node* p = search_result; p != NULL; p = p->next, result_num++);
		const int line_per_page = 12;
		static int cur_page = 1;	//目前是第几页
		{
			char temp[20];
			sprintf(temp, "%d/%d", cur_page, result_num / line_per_page + 1);
			SetPenColor(label_color_style[label_color_num]);
			drawLabel(0.95*winwidth+0.2, 0.5*winheight, temp);
		}
		if (cur_page > 1) {
			if (button(GenUIID(0), 0.95*winwidth, 0.6*winheight, 1 * w, h, "Up"))
				cur_page--;
		}
		if (cur_page < result_num / line_per_page + 1) {
			if (button(GenUIID(0), 0.95*winwidth, 0.4*winheight, 1 * w, h, "Down"))
				cur_page++;
		}

		for (int i = line_per_page * (cur_page - 1);
			i < result_num && i < line_per_page*cur_page - 1;
			i++) 
		{
			struct book_node * p_result_node = search_result;
			for (int j = 0; j < i && p_result_node!=NULL; j++) {
				p_result_node = p_result_node->next;
			}
			{
				char temp[500];
				SetPenColor(label_color_style[label_color_num]);
				sprintf(temp, "%d", p_result_node->book.BookNO);
				drawLabel(0.05*winwidth, winheight - 6 * h - 1.5*(i%line_per_page + 1), temp);
				sprintf(temp, "%s", p_result_node->book.name);
				cut_string(temp, 45);
				drawLabel(0.05*winwidth + 2, winheight - 6 * h - 1.5*(i%line_per_page + 1),temp);
				sprintf(temp, "%s/%s/%s",
					p_result_node->book.author[0], p_result_node->book.author[1], p_result_node->book.author[2]);
				cut_string(temp, 34);
				drawLabel(0.05*winwidth + 14, winheight - 6 * h - 1.5*(i%line_per_page + 1), temp);
				sprintf(temp, "%s/%s/%s/%s/%s", p_result_node->book.keyword[0], p_result_node->book.keyword[1],
					p_result_node->book.keyword[2], p_result_node->book.keyword[3], p_result_node->book.keyword[4]);
				cut_string(temp, 40);
				drawLabel(0.05*winwidth + 23, winheight - 6 * h - 1.5*(i%line_per_page + 1), temp);
				
				sprintf(temp, "%d/%d", p_result_node->book.currentNum, p_result_node->book.totalNum);
				drawLabel(0.05*winwidth + 36, winheight - 6 * h - 1.5*(i%line_per_page + 1), temp);
			}
			if (button(GenUIID(i), 0.05*winwidth + 39, winheight - 6 * h - 1.5*(i%line_per_page + 1), 2 * w, h, "Detail")) {
				change_focusbook(p_result_node->book.BookNO);
				//进入detail页面free搜索结果链表
				for (struct book_node* p = search_result; p != NULL; ) {
					if (p->next == NULL) {
						free(p);
						break;
					}
					else {
						p = p->next;
						if (p->prev != NULL)
							free(p->prev);
					}
				}
				search_result = NULL;
				strcpy(keyword, "");
				pageType = Book_Detail;
				break;
			}
		}
	}
}
void display_Statistics(void)
{
	double fH = GetFontHeight();
	double h = fH * 2; // 控件高度
	double w = winwidth / 40; // 控件宽度
	SetPenColor(label_color_style[label_color_num]);

	//每次进入统计模块时，读入相关文件信息，建立链表
	read_doccument();

	//显示三个统计信息
	{
		static int volumeofBooks, varietyofBooks, numberofUsers;
		static char volumeofBooks_line[50], varietyofBooks_line[50], numberofUsers_line[50];

		volumeofBooks = GetvolumeofBooks();
		varietyofBooks = GetvarietyofBooks();
		numberofUsers = GetnumberofUsers();

		sprintf(volumeofBooks_line, "Total volume of books: %d", volumeofBooks);
		drawLabel(3, 25, volumeofBooks_line);
		sprintf(varietyofBooks_line, "Total vatieties of books: %d", varietyofBooks);
		drawLabel(3, 23, varietyofBooks_line);
		sprintf(numberofUsers_line, "Number of registered users: %d", numberofUsers);
		drawLabel(3, 21, numberofUsers_line);
	}

	//画出分割线
	SetPenColor(label_color_style[label_color_num]);
	MovePen(2.5, 19.5);
	DrawLine(10.7, 0);

	//借和还的统计模块
	{
		static StatisticsStatus statistics_Status = lend;
		char statistisclor[7];
		static int numberoflor = 0;
		static int flag = 0;

		drawLabel(3, 18.5, "Statistics for                     :");
		drawLabel(3, 17, "from                   Y                 M                 D");
		drawLabel(3, 16, "    to                   Y                  M                 D");

		if (statistics_Status == lend) {
			char* menuListlor[] = { "lend", "return" };
			strcpy(statistisclor, "lend");
			if (menuList(GenUIID(0), 6.5, 18.4, 2, 2, h / 2,
				menuListlor, sizeof(menuListlor) / sizeof(menuListlor[0]))) {
				flag = 0;
				statistics_Status = 1 - statistics_Status;
			}
		}
		else {
			char* menuListlor[] = { "return", "lend" };
			if (menuList(GenUIID(0), 6.5, 18.4, 2, 2, h / 2,
				menuListlor, sizeof(menuListlor) / sizeof(menuListlor[0]))) {
				flag = 0;
				statistics_Status = 1 - statistics_Status;
			}
		}

		if (statistics_Status == lend)
			strcpy(statistisclor, "lend");
		else
			strcpy(statistisclor, "return");

		static char fromY[5] = "", fromM[3] = "", fromD[3] = "";
		static char toY[5] = "", toM[3] = "", toD[3] = "";
		textbox(GenUIID(0), 4.7, 16.9, 1.8, h / 2, fromY, sizeof(fromY) / sizeof(char));
		textbox(GenUIID(0), 7.5, 16.9, 1.8, h / 2, fromM, sizeof(fromM) / sizeof(char));
		textbox(GenUIID(0), 10.3, 16.9, 1.8, h / 2, fromD, sizeof(fromD) / sizeof(char));
		textbox(GenUIID(0), 4.7, 15.9, 1.8, h / 2, toY, sizeof(toY) / sizeof(char));
		textbox(GenUIID(0), 7.5, 15.9, 1.8, h / 2, toM, sizeof(toM) / sizeof(char));
		textbox(GenUIID(0), 10.3, 15.9, 1.8, h / 2, toD, sizeof(toD) / sizeof(char));

		//按Search按钮显示统计结果
		if (button(GenUIID(0), 8.6, 14.5, 2.6, h / 1.5, "Search")) {
			if (check_Date(fromY, fromM, fromD, toY, toM, toD) == 0) {
				strcpy(status_line, "wrong input in date");
			}
			else {
				flag = 1;
				strcpy(status_line, "start");
				if (statistics_Status == lend) {
					numberoflor = Get_numberofLend();
				}
				else {
					numberoflor = Get_numberofReturn();
				}
			}
		}
		SetPenColor(label_color_style[label_color_num]);
		char statistisclor_line[100];
		if (flag == 0)
			sprintf(statistisclor_line, "Total %s during this time:", statistisclor);
		else
			sprintf(statistisclor_line, "Total %s during this time: %d", statistisclor, numberoflor);
		drawLabel(3, 13.7, statistisclor_line);

		//按Reset按钮清空输入
		if (button(GenUIID(0), 5.7, 14.5, 2.6, h / 1.5, "Reset")) {
			flag = 0;
			strcpy(fromY, ""); strcpy(fromM, ""); strcpy(fromD, "");
			strcpy(toY, ""); strcpy(toM, ""); strcpy(toD, "");
		}

		//按Print按钮显示统计图
		if (button(GenUIID(0), 11.5, 14.5, 2.6, h / 1.5, "Print")) {
			if (flag == 0) {
				strcpy(status_line, "Please search first");
			}
			else if (!check_Date_interval()) {
				strcpy(status_line, "Date interval is too large");
			}
			else {
				if (statistics_Status == lend) {
					flag = 2;
					print_statisticalChart(flag, label_color_style[label_color_num]);
				}
				else {
					flag = 3;
					print_statisticalChart(flag, label_color_style[label_color_num]);
				}
			}
		}
		if (flag == 2 || flag == 3)
			print_statisticalChart(flag, label_color_style[label_color_num]);
	}
}
void display_ReaderHome(void)
{
	double fH = GetFontHeight();
	double h = fH * 1.7; // 控件高度
	double w = winwidth / 40; // 控件宽度
	SetPenColor(label_color_style[label_color_num]);
	MovePen(winwidth*0.3, 0.7);
	DrawLine(0, winheight - 2 * h - 0.7);
	MovePen(winwidth*0.7, 0.7);
	DrawLine(0, winheight - h - 0.7);

	struct LoginStatus active_account = *GetLogStatus();
	if (active_account.type != Reader_active) {
		strcpy(status_line, "Fail! Have not logged in");
		/*static int flag = 0;
		if (flag == 0) {
			flag = 1;
			MessageBoxA(NULL, "Have not logged in", "Fail", MB_OK);
			
		}*/
	}
	else if (active_account.type == Reader_active) {
		//中央读者信息
		Reader active_readerInfo = *GetReaderInf(active_account.account);
		SetPenColor(label_color_style[label_color_num]);
		char temp[400];
		sprintf(temp, "Account:   %s", active_readerInfo.account);
		drawLabel(winwidth*0.3 + w, 0.75*winheight, temp);
		if (active_readerInfo.gender == Male)
			sprintf(temp, "Gender:    Male");
		else if (active_readerInfo.gender == Female)
			sprintf(temp, "Gender:    Female");
		drawLabel(winwidth*0.3 + w, 0.75*winheight - 3, temp);
		sprintf(temp, "Workplace: %s", active_readerInfo.WorkPlace);
		drawLabel(winwidth*0.3 + w, 0.75*winheight - 6, temp);
		sprintf(temp, "Email:     %s", active_readerInfo.eMail);
		drawLabel(winwidth*0.3 + w, 0.75*winheight - 9, temp);

		if (button(GenUIID(0), 0.4*winwidth, 0.75*winheight - 15, 4 * w, 1.5*h, "Change Password")) {
			pageType = Reader_ChangePassword;
		}
		if (button(GenUIID(0), 0.52*winwidth, 0.75*winheight - 15, 4 * w, 1.5*h, "Change Information")) {
			pageType = Reader_ChangePersonalInfo;
		}

		//左侧，目前借的书
		drawLabel(0.05*winwidth, winheight - 3 * h, "Lend List");
		struct lendrecord_node * lend_record_list_head = GetLentBooks(active_account.account);
		struct lendrecord_node * p_record = lend_record_list_head;
		for (int i = 0; p_record != NULL; p_record = p_record->next, i++) {
			SetPenColor(label_color_style[label_color_num]);
			char temp[NAME_LEN_MAX];
			sprintf(temp,"%s", GetBookInf(p_record->record.BookNO)->name);
			drawLabel(0.6, winheight - 5 * h - i * 2.5, temp);

			struct tm DDL_time = *localtime(&p_record->record.ddlTime);
			strftime(temp, NAME_LEN_MAX, "%F", &DDL_time);
			drawLabel(3.7, winheight - 5 * h -0.7- i * 2.5, temp);

			struct tm lend_time = *localtime(&p_record->record.lendTime);
			strftime(temp, NAME_LEN_MAX, "%F", &lend_time);
			drawLabel(0.7, winheight - 5 * h - 0.7 - i * 2.5, temp);
		}
		if (button(GenUIID(0), 0.1*winwidth, 3 * h, 4 * w, 1.5*h, "Return")) {
			pageType = Reader_Return;
		}
		//右侧 ，其他信息
		{
			char temp[50];
			SetPenColor(label_color_style[label_color_num]);
			sprintf(temp, "You can keep a book for %d days.", get_lendTimeLimit());
			drawLabel(0.75*winwidth, winheight - 5 * h, temp);
			sprintf(temp, "You can borrow %d books at a time.", get_lendNumLimit());
			drawLabel(0.75*winwidth, winheight - 6 * h, temp);
		}
	}

}

void display_BookDetail(void)
{
	double fH = GetFontHeight();
	double h = fH * 2; // 控件高度
	double w = winwidth / 40; // 控件宽度
	int display_book_NO = get_focusbook();
	Book display_book = *GetBookInf(display_book_NO);
	SetPenColor(label_color_style[label_color_num]);
	drawLabel(11, 25-3, " Book NO.");
	drawLabel(11, 23-3, "Caption:");
	drawLabel(11, 21-3, "Keyword:");
	drawLabel(11, 19-3, "Author:");
	drawLabel(11, 17-3, "Press:");
	drawLabel(11, 15-3, "Publish year:");
	drawLabel(11, 13-3, "Number:");

	char temp[NAME_LEN_MAX];
	sprintf(temp, "%d", display_book.BookNO);
	drawLabel(15, 22, temp);
	drawLabel(15, 20, display_book.name);
	sprintf(temp, "%s / %s / %s / %s / %s", display_book.keyword[0],
		display_book.keyword[1], display_book.keyword[2],
		display_book.keyword[3], display_book.keyword[4]);
	drawLabel(15, 18, temp);
	sprintf(temp, "%s / %s / %s", display_book.author[0],
		display_book.author[1], display_book.author[2]);
	drawLabel(15, 16, temp);
	drawLabel(15, 14, display_book.press);
	sprintf(temp, "%d", display_book.publishYear);
	drawLabel(15, 12, temp);
	sprintf(temp, "%d / %d", display_book.currentNum, display_book.totalNum);
	drawLabel(15, 10, temp);

	if (button(GenUIID(0), 19, 8, 4 * w, 1.5*h, "Lend")) {
		/*int ReaderLend(int BookNO, char* ReaderAccount);*/

		if (GetLogStatus()->type != Reader_active) {
			strcpy(status_line, "Please login first");
			goto end_book_detail;
		}
		char reader_account[WORD_LEN_MAX];
		strcpy(reader_account, GetLogStatus()->account);
		int result = ReaderLend(display_book.BookNO, reader_account);
		if (result == 1) {
			strcpy(status_line, "No book in store");
			goto end_book_detail;
		}
		if (result == 2) {
			strcpy(status_line, "Can not borrow two same books.");
			goto end_book_detail;
		}
		if (result == 3) {
			strcpy(status_line, "Can not borrow more books.");
			goto end_book_detail;
		}
		if (result == 0) {
			strcpy(status_line, "Lend success!");
			//change_focusbook(0);
			pageType = Reader_Home;
		}
	}
end_book_detail:
	;
}
void display_AD_Management(void)
{
	if (GetLogStatus()->type == AD_acitve) {
		double fH = GetFontHeight();
		double h = fH * 1.7; // 控件高度
		double w = winwidth / 40; // 控件宽度
		SetPenColor(label_color_style[label_color_num]);
		MovePen(winwidth*0.4, 0.7);
		DrawLine(0, winheight - 2 * h - 0.7);

		MovePen(0, winheight * 0.6);
		DrawLine(winwidth*0.4, 0);
		MovePen(0, winheight * 0.3);
		DrawLine(winwidth*0.4, 0);
		//右部，增加和修改图书
		SetPenColor(label_color_style[label_color_num]);
		drawLabel(winwidth*0.4 + w, winheight - 4 * h, "NO.");
		drawLabel(winwidth*0.4 + w, winheight - 4 * h - 2, "Caption:");
		drawLabel(winwidth*0.4 + w, winheight - 4 * h - 4, "Author 1:");
		drawLabel(winwidth*0.4 + w, winheight - 4 * h - 6, "Author 2:");
		drawLabel(winwidth*0.4 + w, winheight - 4 * h - 8, "Author 3:");
		drawLabel(winwidth*0.4 + w, winheight - 4 * h - 10, "Keyword 1:");
		drawLabel(winwidth*0.4 + w, winheight - 4 * h - 12, "Keyword 2:");
		drawLabel(winwidth*0.4 + w, winheight - 4 * h - 14, "Keyword 3:");
		drawLabel(winwidth*0.4 + w, winheight - 4 * h - 16, "Keyword 4:");
		drawLabel(winwidth*0.4 + w, winheight - 4 * h - 18, "Keyword 5:");
		drawLabel(winwidth*0.4 + w, winheight - 4 * h - 20, "Press:");
		drawLabel(winwidth*0.4 + w, winheight - 4 * h - 22, "Publish year:");

		static Book new_book;
		static char add_book_no_str[10];
		static char add_publish_year_str[10];
		if (textbox(GenUIID(0), winwidth*0.4 + 6 * w, winheight - 4 * h, 13, h, add_book_no_str,
			sizeof(add_book_no_str) / sizeof(char))) {
			if (translate_StrToNum(add_book_no_str) == -1) {
				strcpy(status_line, "Wrong input");
				strcpy(add_book_no_str, "");
			}
			else
				new_book.BookNO = (int)translate_StrToNum(add_book_no_str);
		}
		textbox(GenUIID(0), winwidth*0.4 + 6 * w, winheight - 4 * h - 2, 13, h, new_book.name, NAME_LEN_MAX);
		textbox(GenUIID(0), winwidth*0.4 + 6 * w, winheight - 4 * h - 4, 13, h, new_book.author[0], WORD_LEN_MAX);
		textbox(GenUIID(0), winwidth*0.4 + 6 * w, winheight - 4 * h - 6, 13, h, new_book.author[1], WORD_LEN_MAX);
		textbox(GenUIID(0), winwidth*0.4 + 6 * w, winheight - 4 * h - 8, 13, h, new_book.author[2], WORD_LEN_MAX);
		textbox(GenUIID(0), winwidth*0.4 + 6 * w, winheight - 4 * h - 10, 13, h, new_book.keyword[0], WORD_LEN_MAX);
		textbox(GenUIID(0), winwidth*0.4 + 6 * w, winheight - 4 * h - 12, 13, h, new_book.keyword[1], WORD_LEN_MAX);
		textbox(GenUIID(0), winwidth*0.4 + 6 * w, winheight - 4 * h - 14, 13, h, new_book.keyword[2], WORD_LEN_MAX);
		textbox(GenUIID(0), winwidth*0.4 + 6 * w, winheight - 4 * h - 16, 13, h, new_book.keyword[3], WORD_LEN_MAX);
		textbox(GenUIID(0), winwidth*0.4 + 6 * w, winheight - 4 * h - 18, 13, h, new_book.keyword[4], WORD_LEN_MAX);
		textbox(GenUIID(0), winwidth*0.4 + 6 * w, winheight - 4 * h - 20, 13, h, new_book.press, NAME_LEN_MAX);
		if (textbox(GenUIID(0), winwidth*0.4 + 6 * w, winheight - 4 * h - 22, 13, h, add_publish_year_str,
			sizeof(add_publish_year_str) / sizeof(char))) {
			if (translate_StrToNum(add_publish_year_str) == -1) {
				strcpy(status_line, "Wrong input");
				strcpy(add_publish_year_str, "");
			}
			else
				new_book.publishYear = (int)translate_StrToNum(add_publish_year_str);
		}

		if (button(GenUIID(0), winwidth*0.4 + 5 * w, winheight - 4 * h - 25, 3 * w, 1.5*h, "Add")) {
			int result = ADaddbook(&new_book, 1);
			if (result == 0) {
				strcpy(status_line, "Add success!");
			}
			else if (result == 1) {
				strcpy(status_line, "Illegal input!");
			}
		}
		if (button(GenUIID(0), winwidth*0.4 + 11 * w, winheight - 4 * h - 25, 3 * w, 1.5*h, "Change")) {
			int result = ADrecomposeBook(&new_book);
			if (result == 1) {
				strcpy(status_line, "Not found the book.");
			}
			else if (result == 2) {
				strcpy(status_line, "Illegal input!");
			}
			else if (result == 0) {
				strcpy(status_line, "Change success!");
			}
		}



		//左中，删除图书
		SetPenColor(label_color_style[label_color_num]);
		drawLabel(0.02*winwidth, winheight*0.6 - 3 * h, "Book NO.");
		static char delete_book_no_str[10];
		static int delete_no = 0;
		if (textbox(GenUIID(0), winwidth*0.12, winheight*0.6 - 3 * h, 13, h, delete_book_no_str,
			sizeof(delete_book_no_str) / sizeof(char))) {
			if (translate_StrToNum(delete_book_no_str) == -1) {
				strcpy(status_line, "Wrong input");
				strcpy(delete_book_no_str, "");
			}
			else
				delete_no = (int)translate_StrToNum(delete_book_no_str);
		}
		if (button(GenUIID(0), winwidth*0.12, winheight*0.3 + 1 * h, 4 * w, 1.5*h, "Delete")) {
			int result = ADdeleteBook(delete_no, 1);
			if (result == 1) {
				strcpy(status_line, "Not found the book.");

			}
			else if (result == 2) {
				strcpy(status_line, "Not found the book.");

			}
			else if (result == 0) {
				strcpy(status_line, "Delete success!");
			}
		}

		//左上，新管理员
		SetPenColor(label_color_style[label_color_num]);
		drawLabel(0.02*winwidth, winheight - 3 * h, "Account:");
		drawLabel(0.02*winwidth, winheight - 3 * h - 3, "Pawssword:");
		drawLabel(0.02*winwidth, winheight - 3 * h - 6, "Check:");
		static char ad_password_check[PASSWORD_LEN_MAX];
		static char ad_password_check_temp[PASSWORD_LEN_MAX];
		static char ad_password_temp[PASSWORD_LEN_MAX];
		static Administrator new_ad;
		textbox(GenUIID(0), 0.12*winwidth, winheight - 3.5 * h, 13, h, new_ad.AD_account, WORD_LEN_MAX);
		if (textbox(GenUIID(0), 0.12*winwidth, winheight - 3 * h - 3, 13, h, ad_password_temp, WORD_LEN_MAX)) {
			if (strlen(ad_password_temp) > strlen(new_ad.AD_password)) {
				strcat(new_ad.AD_password, ad_password_temp + strlen(ad_password_temp) - 1);
				ad_password_temp[strlen(ad_password_temp) - 1] = '*';
			}
			else if (strlen(ad_password_temp) < strlen(new_ad.AD_password)) {
				strcpy(new_ad.AD_password + strlen(ad_password_temp), "");
			}
		}
		if (textbox(GenUIID(0), 0.12*winwidth, winheight - 3 * h - 6, 13, h, ad_password_check_temp, WORD_LEN_MAX)) {
			if (strlen(ad_password_check_temp) > strlen(ad_password_check)) {
				strcat(ad_password_check, ad_password_check_temp + strlen(ad_password_check_temp) - 1);
				ad_password_check_temp[strlen(ad_password_check_temp) - 1] = '*';
			}
			else if (strlen(ad_password_check_temp) < strlen(ad_password_check)) {
				strcpy(ad_password_check + strlen(ad_password_check_temp), "");
			}
		}

		if (button(GenUIID(0), 0.12*winwidth, 0.6*winheight + h, 4 * w, 1.5*h, "New AD")) {

			int result = GenNewAdimistrator(&new_ad, ad_password_check);
			switch (result) {
			case 0:
				strcpy(status_line, "New administrator create success!");
				strcpy(ad_password_check, "");
				strcpy(ad_password_check_temp, "");
				strcpy(new_ad.AD_password, "");
				strcpy(ad_password_temp, "");
				strcpy(new_ad.AD_account, "");
				break;
			case 1:
				strcpy(status_line, "Illegal input.");
				strcpy(ad_password_check, "");
				strcpy(ad_password_check_temp, "");
				strcpy(new_ad.AD_password, "");
				strcpy(ad_password_temp, "");
				strcpy(new_ad.AD_account, "");
				break;
			case 2:
				strcpy(status_line, "The account has been used.");
				strcpy(new_ad.AD_account, "");
				break;
			case 3:
				strcpy(status_line, "Password is not safe.");
				strcpy(ad_password_check, "");
				strcpy(ad_password_check_temp, "");
				strcpy(new_ad.AD_password, "");
				strcpy(ad_password_temp, "");
				break;
			case 4:
				strcpy(status_line, "Different password.");
				strcpy(ad_password_check, "");
				strcpy(ad_password_check_temp, "");
				strcpy(new_ad.AD_password, "");
				strcpy(ad_password_temp, "");
				break;
			}
		}
		//左下，更改借书期限
		static int lend_time_limit;
		static int lend_num_limit;
		static char lend_time_limit_str[10];
		static char lend_num_limit_str[10];
		static int read_limit_first = 1;
		if (read_limit_first == 1) {
			lend_time_limit = get_lendTimeLimit();
			lend_num_limit = get_lendNumLimit();
			sprintf(lend_num_limit_str, "%d", lend_num_limit);
			sprintf(lend_time_limit_str, "%d", lend_time_limit);
			read_limit_first = 0;
		}
		SetPenColor(label_color_style[label_color_num]);
		drawLabel(0.02*winwidth, 0.3*winheight - 2 * h, "Lend time limit:");
		drawLabel(0.02*winwidth, 0.3*winheight - 2 * h - 3, "Lend number limit:");
		if (textbox(GenUIID(0), 0.12*winwidth, 0.3*winheight - 2 * h, 13, h, lend_time_limit_str,
			sizeof(lend_time_limit_str) / sizeof(char))) {
			if (translate_StrToNum(lend_time_limit_str) == -1) {
				strcpy(status_line, "Wrong input");
				strcpy(lend_time_limit_str, "");
			}
			else
				lend_time_limit = (int)translate_StrToNum(lend_time_limit_str);
		}
		if (textbox(GenUIID(0), 0.12*winwidth, 0.3*winheight - 2 * h - 3, 13, h, lend_num_limit_str,
			sizeof(lend_num_limit_str) / sizeof(char))) {
			if (translate_StrToNum(lend_num_limit_str) == -1) {
				strcpy(status_line, "Wrong input");
				strcpy(lend_num_limit_str, "");
			}
			else
				lend_num_limit = (int)translate_StrToNum(lend_num_limit_str);
		}
		if (button(GenUIID(0), winwidth*0.12, 2 * h, 4 * w, 1.5*h, "Confirm")) {
			read_limit_first = 0;
			change_lendNUMLimit(lend_num_limit);
			change_lendTimeLimit(lend_time_limit);
			strcpy(status_line, "Change success!");
		}
	}
	else {
	strcpy(status_line, "Please login first.");
}
}
void display_readerChangePassword(void)
{
	double fH = GetFontHeight();
	double h = fH * 2; // 控件高度
	double w = winwidth / 40; // 控件宽度
	static char password_prev[PASSWORD_LEN_MAX] = "";
	static char password_new[PASSWORD_LEN_MAX] = "";
	static char password_check[PASSWORD_LEN_MAX] = "";
	static char password_prev_temp[PASSWORD_LEN_MAX] = "";
	static char password_new_temp[PASSWORD_LEN_MAX] = "";
	static char password_check_temp[PASSWORD_LEN_MAX] = "";

	SetPenColor(label_color_style[label_color_num]);
	drawLabel(9.5, 22, "Previous password");
	drawLabel(9.5, 19, "New password");
	drawLabel(9.5, 16, "Check password");
	drawLabel(31, 19, "Length over 6 and Must include at least a letter");
	if (textbox(GenUIID(0), 15, 22, 14, h, password_prev_temp, PASSWORD_LEN_MAX)) {
		if (strlen(password_prev_temp) > strlen(password_prev)) {
			strcat(password_prev, password_prev_temp + strlen(password_prev_temp) - 1);
			password_prev_temp[strlen(password_prev_temp) - 1] = '*';
		}
		else if (strlen(password_prev_temp) < strlen(password_prev)) {
			strcpy(password_prev, password_prev_temp);
		}
	}

	if (textbox(GenUIID(0), 15, 19, 14, h, password_new_temp, PASSWORD_LEN_MAX)) {
		if (strlen(password_new_temp) > strlen(password_new)) {
			strcat(password_new, password_new_temp + strlen(password_new_temp) - 1);
			password_new_temp[strlen(password_new_temp) - 1] = '*';
		}
		else if (strlen(password_new_temp) < strlen(password_new)) {
			strcpy(password_new, password_new_temp);
		}
	}
	if (textbox(GenUIID(0), 15, 16, 14, h, password_check_temp, PASSWORD_LEN_MAX)) {
		if (strlen(password_check_temp) > strlen(password_check)) {
			strcat(password_check, password_check_temp + strlen(password_check_temp) - 1);
			password_check_temp[strlen(password_check_temp) - 1] = '*';
		}
		else if (strlen(password_check_temp) < strlen(password_check)) {
			strcpy(password_check, password_check_temp);
		}
	}
	if (button(GenUIID(0), 19, 10, 4 * w, 1.5*h, "Change")) {
		int result = recomposeReaderPassword(password_prev, password_new, password_check);
		switch (result) {
		case 0:
			strcpy(status_line, "Change Password Success");
			strcpy(password_check_temp, "");
			strcpy(password_check, "");
			strcpy(password_new_temp, "");
			strcpy(password_new, "");
			strcpy(password_prev, "");
			strcpy(password_prev_temp, "");
			pageType = Reader_Home;
			break;
		case 1:
			strcpy(status_line, "Please login first");
			strcpy(password_check_temp, "");
			strcpy(password_check, "");
			strcpy(password_new_temp, "");
			strcpy(password_new, "");
			strcpy(password_prev, "");
			strcpy(password_new_temp, "");
			pageType = LogIn;
			break;

		case 2:
			strcpy(status_line, "Previous Password Wrong");
			strcpy(password_prev, "");
			strcpy(password_prev_temp, "");
			break;
		case 3:
			strcpy(status_line, "Error, '/' can not be used");
			strcpy(password_check_temp, "");
			strcpy(password_check, "");
			strcpy(password_new_temp, "");
			strcpy(password_new, "");
			break;
		case 4:
			strcpy(status_line, "Different of password");
			strcpy(password_check_temp, "");
			strcpy(password_check, "");
			strcpy(password_new_temp, "");
			strcpy(password_new, "");
			break;
		case 5:
			strcpy(status_line, "Safety problem of password");
			strcpy(password_check_temp, "");
			strcpy(password_check, "");
			strcpy(password_new_temp, "");
			strcpy(password_new, "");
			break;
		}
	}
}
void display_readerReturn(void){
	double fH = GetFontHeight();
	double h = fH * 2; // 控件高度
	double w = winwidth / 40; // 控件宽度
	struct LoginStatus active_account = *GetLogStatus();

	if (active_account.type != Reader_active) {
		strcpy(status_line, "Fail! Have not logged in");
	}
	else {


		SetPenColor(label_color_style[label_color_num]);
		drawLabel(0.05*winwidth, winheight - 5 * h, "NO.");
		drawLabel(0.05*winwidth + 2, winheight - 5 * h, "Caption");
		drawLabel(0.05*winwidth + 14, winheight - 5 * h, "Author");
		drawLabel(0.05*winwidth + 28, winheight - 5 * h, "Lend time");
		drawLabel(0.05*winwidth + 33, winheight - 5 * h, "Return before");

		struct lendrecord_node * lend_record_list_head = GetLentBooks(active_account.account);
		struct lendrecord_node * p_record = lend_record_list_head;
		for (int i = 0; p_record != NULL; p_record = p_record->next, i++) {
			Book lent_book = *GetBookInf(p_record->record.BookNO);
			SetPenColor(label_color_style[label_color_num]);
			char temp[NAME_LEN_MAX];
			sprintf(temp, "%d", lent_book.BookNO);
			drawLabel(0.05*winwidth, winheight - 6 * h - 2 * i, temp);
			sprintf(temp, "%s", lent_book.name);
			cut_string(temp, 50);
			drawLabel(0.05*winwidth + 2, winheight - 6 * h - 2 * i, temp);
			sprintf(temp, "%s/%s/%s", lent_book.author[0],
				lent_book.author[1], lent_book.author[2]);
			cut_string(temp, 45);
			drawLabel(0.05*winwidth + 14, winheight - 6 * h - 2 * i, temp);
			struct tm lend_time = *localtime(&p_record->record.lendTime);
			strftime(temp, NAME_LEN_MAX, "%F", &lend_time);
			drawLabel(0.05*winwidth + 28, winheight - 6 * h - 2 * i, temp);
			struct tm DDL_time = *localtime(&p_record->record.ddlTime);			
			strftime(temp, NAME_LEN_MAX, "%F", &DDL_time);
			drawLabel(0.05*winwidth + 33, winheight - 6 * h - 2 * i, temp);

			if (button(GenUIID(i), 0.05*winwidth + 38, winheight - 6 * h - 2 * i, 2 * w, h, "Return")) {
				int result = ReaderReturn(lent_book.BookNO, active_account.account);
				if (result == 0) {
					strcpy(status_line, "Return success!");
				}
				else {
					strcpy(status_line, "Return fail!");
				}
			}
		}
	}
}
void display_readerHistory(void) {}
void display_readerChangePersonalInfo(void) {
	double fH = GetFontHeight();
	double h = fH * 2; // 控件高度
	double w = winwidth / 40; // 控件宽度
	if (GetLogStatus()->type != Reader_active) {
		strcpy(status_line, "Please login first!");

	}
	else {
		static Reader newReaderInfo;
		static int first_read_reader = 1;
		static char gender_string[10] = "Male";
		SetPenColor(label_color_style[label_color_num]);
		drawLabel(11, 25, "Previous account:");
		drawLabel(11, 22, "New account:");
		drawLabel(11, 19, "Gender:");
		drawLabel(11, 16, "Workplace:");
		drawLabel(11, 13, "E-mail:");
		if (first_read_reader == 1) {
			newReaderInfo = *GetReaderInf(GetLogStatus()->account);
			if (newReaderInfo.gender == Male)
				sprintf(gender_string, "Male");
			else if (newReaderInfo.gender == Female)
				sprintf(gender_string, "Female");
			first_read_reader = 0;
		}
		drawLabel(17, 25, GetLogStatus()->account);
		textbox(GenUIID(0), 15, 22, 14, h, newReaderInfo.account, sizeof(newReaderInfo.account) / sizeof(char));
		//textbox(GenUIID(0), 15, 19, 14, h, gender_string, sizeof(gender_string) / sizeof(char));

		{
			char* gender_list[] = {
				"Male",
				"Female"
			};
			if (strcmp(gender_string, "Male") == 0) {
				gender_list[0] = "Male";
				gender_list[1] = "Female";
			}
			else if (strcmp(gender_string, "Female") == 0) {
				gender_list[1] = "Male";
				gender_list[0] = "Female";
			}
			int selection = menuList(GenUIID(0), 15, 19, 5, 5, h, gender_list, 2);
			if (selection == 1) {
				if (strcmp(gender_string, "Male") == 0) {
					strcpy(gender_string, "Female");
					gender_list[0] = "Male";
					gender_list[1] = "Female";
				}
				else if (strcmp(gender_string, "Female") == 0) {
					strcpy(gender_string, "Male");
					gender_list[1] = "Male";
					gender_list[0] = "Female";
				}
			}
		}
		textbox(GenUIID(0), 15, 16, 14, h, newReaderInfo.WorkPlace, sizeof(newReaderInfo.WorkPlace) / sizeof(char));
		textbox(GenUIID(0), 15, 13, 14, h, newReaderInfo.eMail, sizeof(newReaderInfo.eMail) / sizeof(char));

		if (button(GenUIID(0), 19, 9, 4 * w, 1.5*h, "Confirm")) {
			first_read_reader = 1;
			int result = recomposeReaderInf(&newReaderInfo, gender_string);
			switch (result) {
			case 0:
				strcpy(status_line, "Change success!");
				newReaderInfo = *GetReaderInf(GetLogStatus()->account);

				break;
			case 1:
				strcpy(newReaderInfo.account, "");

				strcpy(newReaderInfo.WorkPlace, "");
				strcpy(newReaderInfo.eMail, "");
				strcpy(status_line, "Error, '/' can not be used");
				break;
			case 2:

				strcpy(status_line, "Wrong input in Gender");
				break;
			case 3:
				strcpy(newReaderInfo.account, "");
				strcpy(status_line, "The account name has been used");
				break;

			}
		}
	}
}
void display_Help(void) {

	SetPenColor(label_color_style[label_color_num]);

	static cur_page = 1;

	if (cur_page == 1) {

		SetPenColor(label_color_style[label_color_num]);

		int i = 0.7;

		drawLabel(1, 27 - i, "1. 读者登录与登出"); i++;
		drawLabel(1, 27 - i, "    打开程序，显示的是登录界面，用户可以选择读者或管理员身份登录，前提是拥有相应的账号和密码。"); i++;
		drawLabel(1, 27 - i, "    除了默认直接开放的功能外，只有在以读者账号登录后，才能进行图书的借和还、修改个人信息和密码。"); i++;
		drawLabel(1, 27 - i, "    读者登陆后，会自动跳转到个人主页，方便用户进行相关操作。个人主页右侧显示读者所借的书的列表，中间显示读者基本信息，右侧显示图书馆目前相关规定。"); i++;
		drawLabel(1, 27 - i, "    用户可以通过点击菜单栏中的Home-> Home | Ctrl - H选项，或直接按下快捷键Ctrl + H，回到个人主页。"); i++;
		drawLabel(1, 27 - i, "2. 读者账号注册"); i++;
		drawLabel(1, 27 - i, "    在非读者登录状态下，在登录界面按下Register按钮，会进入读者注册页面，按照要求填写相关信息并确认，即可注册。"); i++;
		drawLabel(1, 27 - i, "3. 读者修改个人信息/修改密码"); i++;
		drawLabel(1, 27 - i, "    在读者登录状态下，用户在个人主页中按下Change Password/Change Information按钮，或在菜单栏中点击Home->Change Password/Home->Change Information选项，"); i++;
		drawLabel(1, 27 - i, "按照要求填写相关信息并确认，即可修改个人信息/修改密码。"); i++;
		drawLabel(1, 27 - i, "4. 搜索图书"); i++;
		drawLabel(1, 27 - i, "    用户可以在菜单栏中点击Book-> Search | Ctrl - S选项，或直接按下快捷键Ctrl + S进入搜索功能."); i++;
		drawLabel(1, 27 - i, "    用户可以选择书名、作者、关键词或书号作为搜索条件。对书名、作者、关键词可以进行模糊搜索，将搜索到的图书按相关度从大到小排序显示。"); i++;
		drawLabel(1, 27 - i, "    当结果显示超过一页时，可以通过按右侧UP和DOWN按钮切页。"); i++;
		drawLabel(1, 27 - i, "	  按图书右侧的Detail按钮，可以进入相应图书的详情页。"); i++;
		drawLabel(1, 27 - i, "5. 借书"); i++;
		drawLabel(1, 27 - i, "    读者登录后，需要先使用搜索功能找到需要借的书，进入相应图书的详情页后，点击Lend按钮即可。"); i++;
		drawLabel(1, 27 - i, "6. 还书"); i++;
		drawLabel(1, 27 - i, "    读者登录后，可以通过在菜单栏中点击Book->Return选项，或在个人主页按下Return按钮进入还书界面，按下相应图书后的Return按钮后，即可归还相应图书。."); i++;
		drawLabel(1, 27 - i, "7. 管理员模式"); i++;
		drawLabel(1, 27 - i, "	  当以管理员身份登录时，用户可以进行一系列管理员才能进行的操作。需要注意此时用户是没有以读者身份登录时的权限的，如借还书等。"); i++;
		drawLabel(1, 27 - i, "    管理员登录：进入登录界面，请求以管理员身份登录。"); i++;
		drawLabel(1, 27 - i, "    管理员登出：可以通过点击菜单栏中的Administrator->AD-Logout选项，或直接回到登录界面并登录其它账号实现。"); i++;
		drawLabel(1, 27 - i, "    管理员的权限如下：（1）创建新管理员（2）根据图书编号删除图书（3）设置图书借阅天数期限"); i++;
		drawLabel(1, 27 - i, "                                       （4）设置每个读者同时最多借书数量上限（5）添加新图书（6）修改图书信息");

		if (button(GenUIID(0), 40, 2, 4, 1.5, "Next page"))
			cur_page = 2;
	}

	if (cur_page == 2) {

		SetPenColor(label_color_style[label_color_num]);

		int i = 0.7;

		drawLabel(1, 27 - i, "8. 统计模块"); i++;
		drawLabel(1, 27 - i, "    进入方法：用户可以通过点击菜单栏中的Statistics-> Statistics选项进入该模块。"); i++;
		drawLabel(1, 27 - i, "    右上方直接列出的三条统计信息，分别是："); i++;
		drawLabel(1, 27 - i, "    （1）目前图书馆拥有的图书总量（即含重复，含借出）"); i++;
		drawLabel(1, 27 - i, "    （2）目前图书馆拥有的图书总类（即不含重复，含借出），"); i++;
		drawLabel(1, 27 - i, "    （3）目前图书馆的注册用户数。"); i++;
		drawLabel(1, 27 - i, "    指定统计的类型：下方的统计模块中，可以指定统计的类型是lend或return，lend是指一定时间内借书的总量（含已还和未还的），return是指一定时间内还书的总量。"); i++;
		drawLabel(1, 27 - i, "    搜索结果：输入合法的日期后，按下Search按钮，会在下方显示该时期内的借或还书总量。合法的日期是指符合一般格式外，年份需要在2000~2020之间。"); i++;
		drawLabel(1, 27 - i, "    打印统计图：成功搜索之后，可以按下Print按钮，在左边会显示相应的每月柱形统计图。"); i++;
		drawLabel(1, 27 - i, "    统计的信息是：在指定的日期之间，每个月的相应的合计信息。需要注意只能画出跨度在24个月内的统计图。"); i++;
		drawLabel(1, 27 - i, "    清空输入：按下Reset按钮，会清空输入的时间。"); i++;
		drawLabel(1, 27 - i, "9. 设置系统配色"); i++;
		drawLabel(1, 27 - i, "    用户可以通过点击菜单栏中的Setting-> Change Color | Ctrl - T选项，或直接按下快捷键Ctrl + T，使系统配色在6种预置的配色方案中循环切换。"); i++;
		drawLabel(1, 27 - i, "10. 返回上一个浏览界面"); i++;
		drawLabel(1, 27 - i, "     通过点击菜单栏中的Back按钮可以进行回退，最多后退3步"); i++;

		if (button(GenUIID(0), 40, 2, 4, 1.5, "Prve page"))
			cur_page = 1;
	}
}
void display_About_us(void) {
	drawLabel(20, 20, "Developer: Boxuan Wang");
	drawLabel(23.2, 19, "Binlei Li");
	drawLabel(23.2, 16, "2020-6");
}
void draw_StatusLabel(void)
{
	MovePen(0, 0.7);
	SetPenColor(label_color_style[label_color_num]);
	DrawLine(winwidth, 0);
	drawLabel(0, 0.2, status_line);

}
void drawMenuList(void)
{
	double fH = GetFontHeight();
	double h = fH * 1.7; // 控件高度
	double w = winwidth / 40; // 控件宽度
	MovePen(0, winheight);
	DrawLine(winwidth, 0);
	MovePen(0, winheight - h);
	DrawLine(winwidth, 0);
	//第一列
	//位置1，pagetype, 显示目前在什么页面
	//位置最右侧，时间
	//位置右2，active——account


	//第二列，
	//位置一：菜单，读者的个人中心
	char* menuListHome[] = { "Home",
		"Home | Ctrl-H",
		"Change Password",
		"Personal Information",
		"Reader-Login" };
	{
		//设置动态可变菜单标签
		struct LoginStatus cur_status = *GetLogStatus();
		menuListHome[4] = (cur_status.type == Reader_active) ? "Logout" : "Reader-Login"; 
	}

	//位置2：“back”按钮，返回上一界面

	//位置3，菜单，“book”功能集合
	char* menuListBook[] = { "Book",
	"Search | Ctrl-S",
	"Return| Ctrl-R",
	};

	//位置4，管理员操作功能集合
	char* menuListAdministrator[] = {
		"Administrator",
		"AD-Logout",
		"New Book",
		"Recompose Book",
		"Delete Book",
		"New Administrator",
		"Change lend limit"
	};
	{
		struct LoginStatus cur_status = *GetLogStatus();
		if (cur_status.type != AD_acitve) {
			menuListAdministrator[1] = "AD-Login";
			menuListAdministrator[2] = "--";
			menuListAdministrator[3] = "--";
			menuListAdministrator[4] = "--";
			menuListAdministrator[5] = "--";
			menuListAdministrator[6] = "--";
		}
	}

	//位置5，菜单，统计功能模块
	char* menuListStatistics[] = { "Statistics",
	"Statistics| Ctrl-A"
	};
	

	//位置6，菜单，setting设置模块
	char* menuListSitting[] = { "Setting",
	"Change Color | Ctrl-T" };

	//位置7，菜单，帮助
	char* menuListHelp[] = {
		"Help",
		"Help" ,	//帮助文档
		"About us" };//开发者信息

	//位置，最右侧，button，登出（若已经登录）
	//*****************按钮和显示的设计完毕，以下是实现相关功能的代码************************************************

	//第一列，位置1,显示窗口目前的标题
	{
		char pageinfo[50];
		switch (pageType) {
		case LogIn: strcpy(pageinfo, "     Login"); break;
		case Register: strcpy(pageinfo, "      New Reader Register"); break;
		case Search: strcpy(pageinfo, "     Search"); break;
		case Statistics: strcpy(pageinfo, "     Statistics"); break;
		case Reader_Home: strcpy(pageinfo, "     Home"); break;
		case Reader_ChangePassword: strcpy(pageinfo, "     Change passowrd"); break;
		case Reader_ChangePersonalInfo: strcpy(pageinfo, "    Change Personal Information"); break;
		case Reader_Return: strcpy(pageinfo, "     Return Books"); break;
		case Reader_History: strcpy(pageinfo, "    Lend & Return History"); break;
		case Book_Detail: strcpy(pageinfo, "     Book Detail"); break;
		case AD_Management: strcpy(pageinfo, "    Administrator Management"); break;
		case Help: strcpy(pageinfo, "     Help"); break;
		case About_us:strcpy(pageinfo, "   About us "); break;
		}
		MovePen(0, winheight - fH);
		SetPenColor(label_color_style[label_color_num]);
		drawLabel(0, winheight - fH, pageinfo);
	}
	//第一列，位置2，显示登录的账号信息
	{
		struct LoginStatus logstatus = *GetLogStatus();
		char logstatus_line[50] = "";
		if (logstatus.type == Not_Active) {
			sprintf(logstatus_line, "Not Log In");
		}
		else if (logstatus.type == Reader_active) {
			sprintf(logstatus_line, "Reader %s", logstatus.account);
		}
		else if(logstatus.type==AD_acitve){
			sprintf(logstatus_line, "Administrator %s", logstatus.account);
		}
		SetPenColor(label_color_style[label_color_num]);
		drawLabel(winwidth - 5 - strlen(logstatus_line)/5, winheight - fH, logstatus_line);
	}
	//第一行，位置3，最右侧，显示时间
	{
		time_t cur_time;
		time(&cur_time);
		struct tm time_tm;
		time_tm = *localtime(&cur_time);
		char time_s[10];
		if (exist_clock_blink)
			sprintf(time_s, "%2d:%02d", time_tm.tm_hour, time_tm.tm_min);
		else
			sprintf(time_s, "%2d %02d", time_tm.tm_hour, time_tm.tm_min);
		SetPenColor(label_color_style[label_color_num]);
		drawLabel(winwidth - 2, winheight - fH, time_s);
	}
	//第二行，menulist1，Home
	{
		int selection;
		selection = menuList(GenUIID(0), 0, winheight - 2 * h, 3 * h, 5.5 * h, h,
			menuListHome, sizeof(menuListHome) / sizeof(menuListHome[0]));
		if (selection == 1) {
			if (GetLogStatus()->type != Reader_active) {
				strcpy(status_line, "Please log in first");
			}
			else if (GetLogStatus()->type == Reader_active) {
				pageType = Reader_Home;
			}
			
		}
		else if (selection == 2) {
			if (GetLogStatus()->type != Reader_active) {
				strcpy(status_line, "Please log in first");
			}
			else if (GetLogStatus()->type == Reader_active) {
				pageType = Reader_ChangePassword;
			}
		}
		else if (selection == 3){
			if (GetLogStatus()->type != Reader_active) {
				strcpy(status_line, "Please log in first");
			}
			else if (GetLogStatus()->type == Reader_active) {
				pageType = Reader_ChangePersonalInfo;
			}
		}
		else if (selection == 4) {
			if (GetLogStatus()->type != Reader_active) {
				pageType = LogIn;
			}
			else if (GetLogStatus()->type == Reader_active) {
				pageType = LogIn;
				reader_logout();

			}
		}
	}
	//第二行，button,back,返回
	{
		static WindowStatus prev_page[4] = 
			{ Reader_History ,Reader_History ,Reader_History ,Reader_History };
		if (prev_page[0] != pageType) {
			prev_page[3] = prev_page[2];
			prev_page[2] = prev_page[1];
			prev_page[1] = prev_page[0];
			prev_page[0] = pageType;
		}

		if (button(GenUIID(0), 3 * h, winheight - 2 * h, 3 * h, h, "back")) {
			if (prev_page[1] != Reader_History) {
				pageType = prev_page[1];
				prev_page[0] = prev_page[1];
				prev_page[1] = prev_page[2];
				prev_page[2] = prev_page[3];
				prev_page[3] = Reader_History;
				strcpy(status_line, "back");
			}
			else {
				strcpy(status_line, "Can not go back.");
			}
		}
		

	}
	//第二行，menulist，book
	{
		int selection;
		selection = menuList(GenUIID(0), 6 * h, winheight - 2 * h, 3 * h, 5.5 * h, h,
			menuListBook, sizeof(menuListBook) / sizeof(menuListBook[0]));
		if (selection == 1) {
			pageType = Search;
		}
		if (selection == 2) {
			if (GetLogStatus()->type != Reader_active) {
				strcpy(status_line, "Please log in first");
			}
			else if (GetLogStatus()->type == Reader_active) {
				pageType = Reader_Return;
			}
		}
	}

	//第二行，menulist，administrator
	{
		int selection;
		selection = menuList(GenUIID(0), 9 * h, winheight - 2 * h, 4 * h, 5.5 * h, h,
			menuListAdministrator, sizeof(menuListAdministrator) / sizeof(menuListAdministrator[0]));
		if (GetLogStatus()->type != AD_acitve) {
			if (selection == 1) {
				pageType = LogIn;
			}
		}
		else if (GetLogStatus()->type == AD_acitve) {
			if (selection == 1) {
				AD_logout();
				pageType = LogIn;
			}
			else if (selection == 2) {
				pageType = AD_Management;
			}
			else if (selection == 3) {
				pageType = AD_Management;
			}
			else if (selection == 4) {
				pageType = AD_Management;
			}
			else if (selection == 5) {
				pageType = AD_Management;
			}
			else if (selection == 6) {
				pageType = AD_Management;
			}
		}
	}
	//第二行，menulist，statistics
	{
		int selection;
		selection = menuList(GenUIID(0), 13 * h, winheight - 2 * h, 3 * h, 5.5 * h, h,
			menuListStatistics, sizeof(menuListStatistics) / sizeof(menuListStatistics[0]));
		if (selection == 1) {
			pageType = Statistics;
		}
	}
	//第二行，menulist，setting
	{
		int selection;
		selection = menuList(GenUIID(0), 16 * h, winheight - 2 * h, 3 * h, 5.5 * h, h,
			menuListSitting, sizeof(menuListSitting) / sizeof(menuListSitting[0]));
		if (selection == 1) {

			static int predefined_color = 0;
			predefined_color = (predefined_color + 1) % 7;
			if (predefined_color == 2)
				predefined_color++;//防止label颜色为白色

			label_color_num = (label_color_num + 1) % 7;
			if (label_color_num == 2)
				label_color_num++;

			usePredefinedColors(predefined_color);
		}
	}
	//第二行，menulist，help
	{
		int selection;
		selection = menuList(GenUIID(0), 19 * h, winheight - 2 * h, 3 * h, 3 * h, h,
			menuListHelp, sizeof(menuListHelp) / sizeof(menuListHelp[0]));
		if (selection == 1)
			pageType = Help;
		if (selection == 2)
			pageType = About_us;
	}
	//第二行，最右侧，退出键
	{
		if (button(GenUIID(0), winwidth - 4 * h, winheight - 2 * h, 3 * h, h, "EXIT")) {
			exit(-1);
		}
	}
	//画出分割线
	SetPenColor(label_color_style[label_color_num]);
	MovePen(0, winheight);
	DrawLine(winwidth, 0);
	MovePen(0, winheight - h);
	DrawLine(winwidth, 0);
}

static void(*displayPage[13])(void) = { 
	display_LogIn,
	display_Register,
	display_Search,
	display_Statistics,
	display_ReaderHome,
	display_readerChangePassword,
	display_readerChangePersonalInfo,
	display_readerReturn,
	display_readerHistory,
	display_BookDetail,
	display_AD_Management ,
	display_Help,
	display_About_us
};





void MouseEventProcess(int x, int y, int button, int event)
{
	uiGetMouse(x,y,button,event);
	display();
}
void KeyboardEventProcess(int key, int event)
{
	uiGetKeyboard(key, event);
	display();
}
void CharEventProcess(char c)
{
	uiGetChar(c);
	display();
}
void TimerEventProcess(int id) {
	exist_clock_blink = !exist_clock_blink;
	display();
}


void Main()
{
	SetWindowSize(50, 30);
	SetWindowTitle("图书管理系统");
	InitGraphics();
	//InitConsole();

	winwidth = GetWindowWidth();
	winheight = GetWindowHeight();

	startTimer(0, 1000);
	registerMouseEvent(MouseEventProcess);
	registerKeyboardEvent(KeyboardEventProcess);
	registerCharEvent(CharEventProcess);
	registerTimerEvent(TimerEventProcess);

}




void display()
{
	DisplayClear();
	drawMenuList();
	draw_StatusLabel();
	(*displayPage[pageType])();
}
