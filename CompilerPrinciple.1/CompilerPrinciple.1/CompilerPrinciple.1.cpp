#include <iostream>
#include <ctype.h>//handle single ch
#include <stdio.h>//input&output
#include <string>
#define MAX 100
using namespace std;
char blank = ' '; char tab = '\t'; char newline = '\n';
//Token二元组类定义
class tokeninf{
public:
	string token;
	int tokendata;
	tokeninf() {
		token = "NULL";
		tokendata = 100;
	}
	tokeninf(string t_token, int t_data) {
		token = t_token;
		tokendata = t_data;
	}
};

tokeninf token_scan(int *begining, int *forward,char(&INPUTSTR)[MAX])
{
	char ch;//当前读入的字符
	int i = 0;//标记变量
	string tempstr;//临时字符串，拼接字符用
	ch=INPUTSTR[*forward];//向前指针前移
	*forward=*forward+1;
	tokeninf m;//二元组变量m
	//跳过空格和换行
	while (ch == blank || ch == tab || ch == newline) {
		ch = INPUTSTR[*forward];
		*forward=*forward+1;
		*begining=*begining+1;
	}
	//遇到字符串结束符
	if (ch == '\0'||ch==NULL) {
		m.tokendata = -99;
		return(m);
	}
	//开头是字母且紧跟着字母或数字
	else if (isalpha(ch)) {
		ch = INPUTSTR[*forward];
		*forward = *forward + 1;
		while (isalnum(ch)) {
			ch = INPUTSTR[*forward];
			*forward = *forward + 1;
		}
		*forward = *forward - 1;
		m.token = "字母开头的数字字母组合";
		return(m);
	}
	//开头是数字且紧跟着数字
	else if (isdigit(ch)) {
		if (ch = '0'){//以0开头的数字
			ch = INPUTSTR[*forward];
			*forward = *forward + 1;
			//0后跟x，16进制
			if (ch == 'x') {
				ch = INPUTSTR[*forward];
				*forward = *forward + 1;
				while (isxdigit(ch)) {
					ch = INPUTSTR[*forward];
					*forward = *forward + 1;
				}
				*forward = *forward - 1;
				m.token = "16进制数字";
				return(m);
			}
			//0后跟0-7，八进制数字
			else if(ch >= '0'&&ch <= '7') {
				ch = INPUTSTR[*forward];
				*forward = *forward + 1;
				while (ch >= '0'&&ch <= '7') {
					ch = INPUTSTR[*forward];
					*forward = *forward + 1;
				}
				*forward = *forward - 1;
				m.token = "8进制数字";
				return(m);
			}
			else {
				*forward = *forward - 1;
				m.token = "10进制数字";
				return(m);
			}
		}
		else {
			while (ch >= '1'&&ch <= '9') {
				ch = INPUTSTR[*forward];
				*forward = *forward + 1;
			}
			*forward = *forward - 1;
			m.token = "10进制数字";
			return(m);
		}
		/*ch = INPUTSTR[*forward];
		*forward = *forward + 1;
		while (isdigit(ch)) {
			ch = INPUTSTR[*forward];
			*forward = *forward + 1;
		}
		*forward = *forward - 1;
		m.token = "数字开头的数字组合";
		return(m);*/
	}
	//判断符号
	else
		switch (ch) {
		case '*':ch = INPUTSTR[*forward];
			*forward = *forward + 1;
			if (ch == '*') {
				m.token = "EXP";
				return(m);
			}
			else {
				*forward = *forward - 1;
				m.token = "MULTI";
				return(m);
			}break;
		case ':':ch = INPUTSTR[*forward];
			*forward = *forward + 1;
			if (ch == '=') {
				m.token = "ASSIGN";
				return(m);
			}
			else {
				*forward = *forward - 1;
				m.token = "COLON";
				return(m);
			}break;
		case '<':ch = INPUTSTR[*forward];
			*forward = *forward + 1;
			if (ch == '=') {
				m.token = "LE";
				return(m);
			}
			else if (ch == '>') {
				m.token = "NE";
				return(m);
			}
			else {
				*forward = *forward - 1;
				m.token = "LT";
				return(m);
			}break;
		case '>':ch = INPUTSTR[*forward];
			*forward = *forward + 1;
			if (ch == '=') {
				m.token = "GE";
				return(m);
			}
			else {
				*forward = *forward - 1;
				m.token = "GT";
				return(m);
			}break;
		case '=':
			m.token = "EQ";
			return(m);
			break;
		case '+':
			m.token = "PLUS";
			return(m);
			break;
		case '-':
			m.token = "MINUS";
			return(m);
			break;
		case '/':
			m.token = "RDIV";
			return(m);
			break;
		case ',':
			m.token = "COMMA";
			return(m);
			break;
		case ';':
			m.token = "SEMIC";
			return(m);
			break;
		default://error_handle();
			break;
		}
	return(m);
}

void main() {
	char INPUTSTR[MAX];
	int imm = 0,begining = 0, forward = 0;
	gets_s(INPUTSTR);
	tokeninf m("INIT",100);//存放返回的的token
	cout << "INPUT INFORMATION: "<<INPUTSTR<< endl<<endl;
	while (m.tokendata>0){
		m=token_scan(&begining,&forward,INPUTSTR);
		if(m.tokendata>0)
			cout << "answer---" << m.token << endl;
	} //循环调用scan
	return;
}
