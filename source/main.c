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
#include<time.h>		//ϵͳ�Դ�ʱ�䴦���

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
	Reader_Home,		//������ҳ
	Reader_ChangePassword,
	Reader_ChangePersonalInfo,
	Reader_Return,		//ֻ�л���
	Reader_History,		//��ʷ�����¼
	Book_Detail,		//��Ļֻ��ʾһ�������Ϣ��ͬʱչʾ���顢���顢����İ�ť
	AD_Management,	//����������ɾ���鼮�������鼮��Ϣ,�½�����Ա
	Help,
	About_us

}WindowStatus;

static WindowStatus pageType = LogIn;	//��־��Ҫ����Ǹ�ҳ��
static char status_line[101] = "start";	//ҳ�����½ǵ���Ϣ������ǩ������
static double winwidth, winheight;  //���ڳߴ�
static int exist_clock_blink = 0;	//ʱ����˸���Ƿ����ð��

const char *label_color_style[] = { "blue","black","white","black","black","red","red" };
static int label_color_num = 0;	//setting�и�����ɫʱ����Ӧ�ظı�label����ɫ

void display(void);
void DisplayClear(void);
void startTimer(int id, int timeinterval);



void display_LogIn(void)
{

	double fH = GetFontHeight();
	double h = fH * 2; // �ؼ��߶�
	double w = winwidth / 40; // �ؼ����
	//double x = winwidth / 3;
	//double y = winheight / 2 + h+ h;
	SetPenColor(label_color_style[label_color_num]);
	drawLabel(15, 19, " Account:");
	drawLabel(15, 16, "Password:");

	static char account[WORD_LEN_MAX] = "";

	static char password[PASSWORD_LEN_MAX] = "";
	static char passwordTemp[PASSWORD_LEN_MAX] = "";

	textbox(GenUIID(0), 19, 19, 14, h, account, sizeof(account) / sizeof(char));
	//�������������ʾ
	if (textbox(GenUIID(0), 19, 15.5, 14, h, passwordTemp, sizeof(password) / sizeof(char))) {
		if (strlen(passwordTemp) > strlen(password)) {
			strcat(password, passwordTemp + strlen(passwordTemp) - 1);
			passwordTemp[strlen(passwordTemp) - 1] = '*';
		}
		else if (strlen(passwordTemp) < strlen(password)) {
			strcpy(password + strlen(passwordTemp), "");
		}
	}

	//���ߵ�¼����
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
		/*�������빦�ܽӿ�*/;
}
void display_Register(void)
{
	double fH = GetFontHeight();
	double h = fH * 2; // �ؼ��߶�
	double w = winwidth / 40; // �ؼ����
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
	//���ı������ǵ����������ʾΪ*
	if (textbox(GenUIID(0), 15, 22, 14, h, password_temp, WORD_LEN_MAX)) {

		if (strlen(password_temp) > strlen(newreader.password)) {
			strcat(newreader.password, password_temp + strlen(password_temp) - 1);
			password_temp[strlen(password_temp) - 1] = '*';
		}
		else if (strlen(password_temp) < strlen(newreader.password)) {
			strcpy(newreader.password + strlen(password_temp), "");
		}
	}
	
	//����ȷ��ͬ��������ʾ����
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
	double h = fH * 1.7; // �ؼ��߶�
	double w = winwidth / 40; // �ؼ����
	static struct book_node * search_result = NULL;
	static int search_type = 1;	//������ʽ��1 name��2 keyword, 3 author,4 NO.
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
		//ÿ�ε��search��free֮ǰ�������������
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

	//��ʾ��������Ĳ���
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
		static int cur_page = 1;	//Ŀǰ�ǵڼ�ҳ
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
				//����detailҳ��free�����������
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
	double h = fH * 2; // �ؼ��߶�
	double w = winwidth / 40; // �ؼ����
	SetPenColor(label_color_style[label_color_num]);

	//ÿ�ν���ͳ��ģ��ʱ����������ļ���Ϣ����������
	read_doccument();

	//��ʾ����ͳ����Ϣ
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

	//�����ָ���
	SetPenColor(label_color_style[label_color_num]);
	MovePen(2.5, 19.5);
	DrawLine(10.7, 0);

	//��ͻ���ͳ��ģ��
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

		//��Search��ť��ʾͳ�ƽ��
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

		//��Reset��ť�������
		if (button(GenUIID(0), 5.7, 14.5, 2.6, h / 1.5, "Reset")) {
			flag = 0;
			strcpy(fromY, ""); strcpy(fromM, ""); strcpy(fromD, "");
			strcpy(toY, ""); strcpy(toM, ""); strcpy(toD, "");
		}

		//��Print��ť��ʾͳ��ͼ
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
	double h = fH * 1.7; // �ؼ��߶�
	double w = winwidth / 40; // �ؼ����
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
		//���������Ϣ
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

		//��࣬Ŀǰ�����
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
		//�Ҳ� ��������Ϣ
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
	double h = fH * 2; // �ؼ��߶�
	double w = winwidth / 40; // �ؼ����
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
		double h = fH * 1.7; // �ؼ��߶�
		double w = winwidth / 40; // �ؼ����
		SetPenColor(label_color_style[label_color_num]);
		MovePen(winwidth*0.4, 0.7);
		DrawLine(0, winheight - 2 * h - 0.7);

		MovePen(0, winheight * 0.6);
		DrawLine(winwidth*0.4, 0);
		MovePen(0, winheight * 0.3);
		DrawLine(winwidth*0.4, 0);
		//�Ҳ������Ӻ��޸�ͼ��
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



		//���У�ɾ��ͼ��
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

		//���ϣ��¹���Ա
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
		//���£����Ľ�������
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
	double h = fH * 2; // �ؼ��߶�
	double w = winwidth / 40; // �ؼ����
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
	double h = fH * 2; // �ؼ��߶�
	double w = winwidth / 40; // �ؼ����
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
	double h = fH * 2; // �ؼ��߶�
	double w = winwidth / 40; // �ؼ����
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

		drawLabel(1, 27 - i, "1. ���ߵ�¼��ǳ�"); i++;
		drawLabel(1, 27 - i, "    �򿪳�����ʾ���ǵ�¼���棬�û�����ѡ����߻����Ա��ݵ�¼��ǰ����ӵ����Ӧ���˺ź����롣"); i++;
		drawLabel(1, 27 - i, "    ����Ĭ��ֱ�ӿ��ŵĹ����⣬ֻ�����Զ����˺ŵ�¼�󣬲��ܽ���ͼ��Ľ�ͻ����޸ĸ�����Ϣ�����롣"); i++;
		drawLabel(1, 27 - i, "    ���ߵ�½�󣬻��Զ���ת��������ҳ�������û�������ز�����������ҳ�Ҳ���ʾ�������������б��м���ʾ���߻�����Ϣ���Ҳ���ʾͼ���Ŀǰ��ع涨��"); i++;
		drawLabel(1, 27 - i, "    �û�����ͨ������˵����е�Home-> Home | Ctrl - Hѡ���ֱ�Ӱ��¿�ݼ�Ctrl + H���ص�������ҳ��"); i++;
		drawLabel(1, 27 - i, "2. �����˺�ע��"); i++;
		drawLabel(1, 27 - i, "    �ڷǶ��ߵ�¼״̬�£��ڵ�¼���水��Register��ť����������ע��ҳ�棬����Ҫ����д�����Ϣ��ȷ�ϣ�����ע�ᡣ"); i++;
		drawLabel(1, 27 - i, "3. �����޸ĸ�����Ϣ/�޸�����"); i++;
		drawLabel(1, 27 - i, "    �ڶ��ߵ�¼״̬�£��û��ڸ�����ҳ�а���Change Password/Change Information��ť�����ڲ˵����е��Home->Change Password/Home->Change Informationѡ�"); i++;
		drawLabel(1, 27 - i, "����Ҫ����д�����Ϣ��ȷ�ϣ������޸ĸ�����Ϣ/�޸����롣"); i++;
		drawLabel(1, 27 - i, "4. ����ͼ��"); i++;
		drawLabel(1, 27 - i, "    �û������ڲ˵����е��Book-> Search | Ctrl - Sѡ���ֱ�Ӱ��¿�ݼ�Ctrl + S������������."); i++;
		drawLabel(1, 27 - i, "    �û�����ѡ�����������ߡ��ؼ��ʻ������Ϊ���������������������ߡ��ؼ��ʿ��Խ���ģ������������������ͼ�鰴��ضȴӴ�С������ʾ��"); i++;
		drawLabel(1, 27 - i, "    �������ʾ����һҳʱ������ͨ�����Ҳ�UP��DOWN��ť��ҳ��"); i++;
		drawLabel(1, 27 - i, "	  ��ͼ���Ҳ��Detail��ť�����Խ�����Ӧͼ�������ҳ��"); i++;
		drawLabel(1, 27 - i, "5. ����"); i++;
		drawLabel(1, 27 - i, "    ���ߵ�¼����Ҫ��ʹ�����������ҵ���Ҫ����飬������Ӧͼ�������ҳ�󣬵��Lend��ť���ɡ�"); i++;
		drawLabel(1, 27 - i, "6. ����"); i++;
		drawLabel(1, 27 - i, "    ���ߵ�¼�󣬿���ͨ���ڲ˵����е��Book->Returnѡ����ڸ�����ҳ����Return��ť���뻹����棬������Ӧͼ����Return��ť�󣬼��ɹ黹��Ӧͼ�顣."); i++;
		drawLabel(1, 27 - i, "7. ����Աģʽ"); i++;
		drawLabel(1, 27 - i, "	  ���Թ���Ա��ݵ�¼ʱ���û����Խ���һϵ�й���Ա���ܽ��еĲ�������Ҫע���ʱ�û���û���Զ�����ݵ�¼ʱ��Ȩ�޵ģ���軹��ȡ�"); i++;
		drawLabel(1, 27 - i, "    ����Ա��¼�������¼���棬�����Թ���Ա��ݵ�¼��"); i++;
		drawLabel(1, 27 - i, "    ����Ա�ǳ�������ͨ������˵����е�Administrator->AD-Logoutѡ���ֱ�ӻص���¼���沢��¼�����˺�ʵ�֡�"); i++;
		drawLabel(1, 27 - i, "    ����Ա��Ȩ�����£���1�������¹���Ա��2������ͼ����ɾ��ͼ�飨3������ͼ�������������"); i++;
		drawLabel(1, 27 - i, "                                       ��4������ÿ������ͬʱ�������������ޣ�5�������ͼ�飨6���޸�ͼ����Ϣ");

		if (button(GenUIID(0), 40, 2, 4, 1.5, "Next page"))
			cur_page = 2;
	}

	if (cur_page == 2) {

		SetPenColor(label_color_style[label_color_num]);

		int i = 0.7;

		drawLabel(1, 27 - i, "8. ͳ��ģ��"); i++;
		drawLabel(1, 27 - i, "    ���뷽�����û�����ͨ������˵����е�Statistics-> Statisticsѡ������ģ�顣"); i++;
		drawLabel(1, 27 - i, "    ���Ϸ�ֱ���г�������ͳ����Ϣ���ֱ��ǣ�"); i++;
		drawLabel(1, 27 - i, "    ��1��Ŀǰͼ���ӵ�е�ͼ�������������ظ����������"); i++;
		drawLabel(1, 27 - i, "    ��2��Ŀǰͼ���ӵ�е�ͼ�����ࣨ�������ظ������������"); i++;
		drawLabel(1, 27 - i, "    ��3��Ŀǰͼ��ݵ�ע���û�����"); i++;
		drawLabel(1, 27 - i, "    ָ��ͳ�Ƶ����ͣ��·���ͳ��ģ���У�����ָ��ͳ�Ƶ�������lend��return��lend��ָһ��ʱ���ڽ�������������ѻ���δ���ģ���return��ָһ��ʱ���ڻ����������"); i++;
		drawLabel(1, 27 - i, "    �������������Ϸ������ں󣬰���Search��ť�������·���ʾ��ʱ���ڵĽ�����������Ϸ���������ָ����һ���ʽ�⣬�����Ҫ��2000~2020֮�䡣"); i++;
		drawLabel(1, 27 - i, "    ��ӡͳ��ͼ���ɹ�����֮�󣬿��԰���Print��ť������߻���ʾ��Ӧ��ÿ������ͳ��ͼ��"); i++;
		drawLabel(1, 27 - i, "    ͳ�Ƶ���Ϣ�ǣ���ָ��������֮�䣬ÿ���µ���Ӧ�ĺϼ���Ϣ����Ҫע��ֻ�ܻ��������24�����ڵ�ͳ��ͼ��"); i++;
		drawLabel(1, 27 - i, "    ������룺����Reset��ť������������ʱ�䡣"); i++;
		drawLabel(1, 27 - i, "9. ����ϵͳ��ɫ"); i++;
		drawLabel(1, 27 - i, "    �û�����ͨ������˵����е�Setting-> Change Color | Ctrl - Tѡ���ֱ�Ӱ��¿�ݼ�Ctrl + T��ʹϵͳ��ɫ��6��Ԥ�õ���ɫ������ѭ���л���"); i++;
		drawLabel(1, 27 - i, "10. ������һ���������"); i++;
		drawLabel(1, 27 - i, "     ͨ������˵����е�Back��ť���Խ��л��ˣ�������3��"); i++;

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
	double h = fH * 1.7; // �ؼ��߶�
	double w = winwidth / 40; // �ؼ����
	MovePen(0, winheight);
	DrawLine(winwidth, 0);
	MovePen(0, winheight - h);
	DrawLine(winwidth, 0);
	//��һ��
	//λ��1��pagetype, ��ʾĿǰ��ʲôҳ��
	//λ�����Ҳ࣬ʱ��
	//λ����2��active����account


	//�ڶ��У�
	//λ��һ���˵������ߵĸ�������
	char* menuListHome[] = { "Home",
		"Home | Ctrl-H",
		"Change Password",
		"Personal Information",
		"Reader-Login" };
	{
		//���ö�̬�ɱ�˵���ǩ
		struct LoginStatus cur_status = *GetLogStatus();
		menuListHome[4] = (cur_status.type == Reader_active) ? "Logout" : "Reader-Login"; 
	}

	//λ��2����back����ť��������һ����

	//λ��3���˵�����book�����ܼ���
	char* menuListBook[] = { "Book",
	"Search | Ctrl-S",
	"Return| Ctrl-R",
	};

	//λ��4������Ա�������ܼ���
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

	//λ��5���˵���ͳ�ƹ���ģ��
	char* menuListStatistics[] = { "Statistics",
	"Statistics| Ctrl-A"
	};
	

	//λ��6���˵���setting����ģ��
	char* menuListSitting[] = { "Setting",
	"Change Color | Ctrl-T" };

	//λ��7���˵�������
	char* menuListHelp[] = {
		"Help",
		"Help" ,	//�����ĵ�
		"About us" };//��������Ϣ

	//λ�ã����Ҳ࣬button���ǳ������Ѿ���¼��
	//*****************��ť����ʾ�������ϣ�������ʵ����ع��ܵĴ���************************************************

	//��һ�У�λ��1,��ʾ����Ŀǰ�ı���
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
	//��һ�У�λ��2����ʾ��¼���˺���Ϣ
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
	//��һ�У�λ��3�����Ҳ࣬��ʾʱ��
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
	//�ڶ��У�menulist1��Home
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
	//�ڶ��У�button,back,����
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
	//�ڶ��У�menulist��book
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

	//�ڶ��У�menulist��administrator
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
	//�ڶ��У�menulist��statistics
	{
		int selection;
		selection = menuList(GenUIID(0), 13 * h, winheight - 2 * h, 3 * h, 5.5 * h, h,
			menuListStatistics, sizeof(menuListStatistics) / sizeof(menuListStatistics[0]));
		if (selection == 1) {
			pageType = Statistics;
		}
	}
	//�ڶ��У�menulist��setting
	{
		int selection;
		selection = menuList(GenUIID(0), 16 * h, winheight - 2 * h, 3 * h, 5.5 * h, h,
			menuListSitting, sizeof(menuListSitting) / sizeof(menuListSitting[0]));
		if (selection == 1) {

			static int predefined_color = 0;
			predefined_color = (predefined_color + 1) % 7;
			if (predefined_color == 2)
				predefined_color++;//��ֹlabel��ɫΪ��ɫ

			label_color_num = (label_color_num + 1) % 7;
			if (label_color_num == 2)
				label_color_num++;

			usePredefinedColors(predefined_color);
		}
	}
	//�ڶ��У�menulist��help
	{
		int selection;
		selection = menuList(GenUIID(0), 19 * h, winheight - 2 * h, 3 * h, 3 * h, h,
			menuListHelp, sizeof(menuListHelp) / sizeof(menuListHelp[0]));
		if (selection == 1)
			pageType = Help;
		if (selection == 2)
			pageType = About_us;
	}
	//�ڶ��У����Ҳ࣬�˳���
	{
		if (button(GenUIID(0), winwidth - 4 * h, winheight - 2 * h, 3 * h, h, "EXIT")) {
			exit(-1);
		}
	}
	//�����ָ���
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
	SetWindowTitle("ͼ�����ϵͳ");
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
