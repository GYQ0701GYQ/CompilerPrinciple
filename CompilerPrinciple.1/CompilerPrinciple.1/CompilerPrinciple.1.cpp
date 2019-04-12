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

tokeninf token_scan(const char *begining, const char *forward)
{
	char ch;//当前读入的字符
	int i = 0;//标记变量
	string tempstr;
	ch = *forward++;
	tokeninf m;
	//跳过空格和换行
	while (ch == blank || ch == tab || ch == newline) {
		ch = *forward++;
		begining++;
	}
	if (ch == '\0') {
		m.tokendata = -99;
		return(m);
	}
	//开头是字母且紧跟着字母或数字
	else if (isalpha(ch)) {
		ch = *forward++;
		while (isalnum(ch)) {
			ch = *forward;
			forward++;
		}
		forward--;
		/*for (i=0; begining < forward; begining++) {
			tempstr[i] = *begining;
			i++;
		}*/
		//m.token = tempstr;
		m.token = "数字字母组合";
		return(m);
		//return(gettoken(token), install_id(token));
	}
	//开头是数字且紧跟着数字
	else if (isdigit(ch)) {
		ch = *forward;
		forward++;
		while (isdigit(ch)) {
			ch = *forward;
			forward++;
		}
		forward--;
		/*for (; begining < forward; begining++) {
			tempstr[i] = *begining;
			i++;
		}*/
		m.token = "数字开头的数字组合";
		return(m);
		//retract(1);
		//token=copytoken();
		//return(INT, install_num(token));
	}
	else
		switch (ch) {
		case '*':ch = *forward;
			forward++;
			if (ch == '*') {
				m.token = "EXP";
				return(m);
			}
			else {
				forward--;
				//retract(1);
				m.token = "MULTI";
				return(m);
			}break;
		case ':':ch = *forward;
			forward++;
			if (ch == '=') {
				m.token = "ASSIGN";
				return(m);
			}
			else {
				forward--;
				//retract(1);
				m.token = "COLON";
				return(m);
			}break;
		case '<':ch = *forward;
			forward++;
			if (ch == '=') {
				m.token = "LE";
				return(m);
			}
			else if (ch == '>') {
				m.token = "NE";
				return(m);
			}
			else {
				forward--;
				//retract(1);
				m.token = "LT";
				return(m);
			}break;
		case '>':ch = *forward;
			forward++;
			if (ch == '=') {
				m.token = "GE";
				return(m);
			}
			else {
				forward--;
				//retract(1);
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
	string str;
	cin >> str;
	const char *begining= str.c_str();
	const char *forward= str.c_str();//开始指针和向前指针
	tokeninf i;//存放识别出的token
	//while (*forward++!='\0'){
		i=token_scan(begining,forward);
		cout << "answer---" << i.token << endl;
	//} //循环调用scan
	return;
}
