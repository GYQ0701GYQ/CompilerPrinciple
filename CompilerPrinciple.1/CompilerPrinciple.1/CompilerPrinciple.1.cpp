#include <iostream>
#include <ctype.h>//handle single ch
#include <stdio.h>//input&output
#include <string>
#define MAX 100
#define KEYWARD {"if","then","else","while","do"}
using namespace std;
char blank = ' '; char tab = '\t'; char newline = '\n';
//Token二元组类定义
class tokeninf{
public:
	string token;//token的种别
	string tokendata;//token的属性值
	tokeninf() {
		token = "NULL";
	}
	tokeninf(string t_token, string t_data) {
		token = t_token;
		tokendata = t_data;
	}
};
//检测关键字
inline tokeninf isKeyword(tokeninf &m) {
	string Keyword[5] = { "if","then","else","while","do" };
	for (int i = 0; i < 5; i++) {
		if (m.tokendata == Keyword[i]) {
			m.token = Keyword[i];
			break;
		}
	}
	int q = 0;
	while (m.token[q] != '\0') {
		m.token[q] = toupper(m.token[q]);
		q++;
	}
	return m;
}
//16进制转10进制
inline tokeninf HEXtoDEC(tokeninf &m) {
	//字符串中提取数字转换进制
	int i = 2, ans=0;
	while (m.tokendata[i] != '\0') {
		if (isdigit(m.tokendata[i])) {
			ans = ans*16+(m.tokendata[i++]- 48);
		}
		else {
			ans = ans*16+(m.tokendata[i++] - 87);
		}
	}
	//转换后的int值再换成string形式
	string str;
	int cs = ans / 10, ys = ans % 10;
	for (; (cs != 0)||(ys!=0); ys = cs % 10,cs = cs / 10) {
		char ch = ys + 48;
		str = ch + str;
	}
	m.tokendata = str;
	return m;
}
//八进制转10进制
inline tokeninf OCTtoDEC(tokeninf &m) {
	int i = 0, ans = 0;
	while (m.tokendata[i] != '\0') {
		ans = ans * 8 + (m.tokendata[i++] - 48);
	}
	//转换后的int值再换成string形式
	string str;
	int cs = ans / 10, ys = ans % 10;
	for (; (cs != 0) || (ys != 0); ys = cs % 10, cs = cs / 10) {
		char ch = ys + 48;
		str = ch + str;
	}
	m.tokendata = str;
	return m;
}

tokeninf token_scan(int *begining, int *forward,char(&INPUTSTR)[MAX])
{
	char ch;//当前读入的字符
	int i = 0;//标记变量
	ch=INPUTSTR[*forward];
	*forward=*forward+1;//向前指针前移
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
		m.token = "ID";
		for (string s1; (*begining) != (*forward); *begining = *begining + 1) {
			s1 = INPUTSTR[*begining];
			m.tokendata = m.tokendata + s1;
		}
		m=isKeyword(m);
		return(m);
	}
	//开头是数字且紧跟着数字
	else if (isdigit(ch)) {
		if (ch == '0'){//以0开头的数字
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
				for (string s1; (*begining) != (*forward); *begining = *begining + 1) {
					s1 = INPUTSTR[*begining];
					m.tokendata = m.tokendata + s1;
				}
				m = HEXtoDEC(m);
				m.token = "INT16";
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
				for (string s1; (*begining) != (*forward); *begining = *begining + 1) {
					s1 = INPUTSTR[*begining];
					m.tokendata = m.tokendata + s1;
				}
				m = OCTtoDEC(m);
				m.token = "INT8";
				return(m);
			}
			else {
				*forward = *forward - 1;
				for (string s1; (*begining) != (*forward); *begining = *begining + 1) {
					s1 = INPUTSTR[*begining];
					m.tokendata = m.tokendata + s1;
				}
				m.token = "INT10";
				return(m);
			}
		}
		else {
			while (ch >= '1'&&ch <= '9') {
				ch = INPUTSTR[*forward];
				*forward = *forward + 1;
			}
			*forward = *forward - 1;
			for (string s1; (*begining) != (*forward); *begining = *begining + 1) {
				s1 = INPUTSTR[*begining];
				m.tokendata = m.tokendata + s1;
			}
			m.token = "INT10";
			return(m);
		}
	}
	//判断符号
	else
		switch (ch) {
		case '*':
			m.tokendata = ch;
			ch = INPUTSTR[*forward];
			*forward = *forward + 1;
			if (ch == '*') {
				m.token = "EXP";
				m.tokendata += ch;
				return(m);
			}
			else {
				*forward = *forward - 1;
				m.token = "MULTI";
				return(m);
			}break;
		case ':':
			m.tokendata += ch;
			ch = INPUTSTR[*forward];
			*forward = *forward + 1;
			if (ch == '=') {
				m.token = "ASSIGN";
				m.tokendata += ch;
				return(m);
			}
			else {
				*forward = *forward - 1;
				m.token = "COLON";
				return(m);
			}break;
		case '<':
			m.tokendata += ch;
			ch = INPUTSTR[*forward];
			*forward = *forward + 1;
			if (ch == '=') {
				m.token = "LE";
				m.tokendata += ch;
				return(m);
			}
			else if (ch == '>') {
				m.token = "NE";
				m.tokendata += ch;
				return(m);
			}
			else {
				*forward = *forward - 1;
				m.token = "LT";
				return(m);
			}break;
		case '>':
			m.tokendata += ch;
			ch = INPUTSTR[*forward];
			*forward = *forward + 1;
			if (ch == '=') {
				m.token = "GE";
				m.tokendata += ch;
				return(m);
			}
			else {
				*forward = *forward - 1;
				m.token = "GT";
				return(m);
			}break;
		case '=':
			m.tokendata += ch;
			m.token = "EQ";
			return(m);
			break;
		case '+':
			m.tokendata += ch;
			m.token = "PLUS";
			return(m);
			break;
		case '-':
			m.tokendata += ch;
			m.token = "MINUS";
			return(m);
			break;
		case '/':
			m.tokendata += ch;
			m.token = "RDIV";
			return(m);
			break;
		case ',':
			m.tokendata += ch;
			m.token = "COMMA";
			return(m);
			break;
		case ';':
			m.tokendata += ch;
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
	tokeninf m("INIT","INITDATA");//存放返回的的token
	while (m.token !="NULL"){
		m=token_scan(&begining,&forward,INPUTSTR);
		if(m.token !="NULL")
			cout << "answer---" << m.token <<"  "<<m.tokendata<< endl;
	} //循环调用scan
	return;
}
