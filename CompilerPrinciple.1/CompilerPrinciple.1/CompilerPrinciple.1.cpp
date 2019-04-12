#include <iostream>
#include <ctype.h>//handle single ch
#include <stdio.h>//input&output
#include <string>
#define MAX 100
using namespace std;
char blank = ' '; char tab = '\t'; char newline = '\n';
//Token��Ԫ���ඨ��
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
	char ch;//��ǰ������ַ�
	int i = 0;//��Ǳ���
	string tempstr;
	ch = *forward++;
	tokeninf m;
	//�����ո�ͻ���
	while (ch == blank || ch == tab || ch == newline) {
		ch = *forward++;
		begining++;
	}
	if (ch == '\0') {
		m.tokendata = -99;
		return(m);
	}
	//��ͷ����ĸ�ҽ�������ĸ������
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
		m.token = "������ĸ���";
		return(m);
		//return(gettoken(token), install_id(token));
	}
	//��ͷ�������ҽ���������
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
		m.token = "���ֿ�ͷ���������";
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
	const char *forward= str.c_str();//��ʼָ�����ǰָ��
	tokeninf i;//���ʶ�����token
	//while (*forward++!='\0'){
		i=token_scan(begining,forward);
		cout << "answer---" << i.token << endl;
	//} //ѭ������scan
	return;
}
