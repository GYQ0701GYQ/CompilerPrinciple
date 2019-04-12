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

tokeninf token_scan(int *begining, int *forward,char(&INPUTSTR)[MAX])
{
	char ch;//��ǰ������ַ�
	int i = 0;//��Ǳ���
	string tempstr;//��ʱ�ַ�����ƴ���ַ���
	ch=INPUTSTR[*forward];//��ǰָ��ǰ��
	*forward=*forward+1;
	tokeninf m;//��Ԫ�����m
	//�����ո�ͻ���
	while (ch == blank || ch == tab || ch == newline) {
		ch = INPUTSTR[*forward];
		*forward=*forward+1;
		*begining=*begining+1;
	}
	//�����ַ���������
	if (ch == '\0'||ch==NULL) {
		m.tokendata = -99;
		return(m);
	}
	//��ͷ����ĸ�ҽ�������ĸ������
	else if (isalpha(ch)) {
		ch = INPUTSTR[*forward];
		*forward = *forward + 1;
		while (isalnum(ch)) {
			ch = INPUTSTR[*forward];
			*forward = *forward + 1;
		}
		*forward = *forward - 1;
		m.token = "��ĸ��ͷ��������ĸ���";
		return(m);
	}
	//��ͷ�������ҽ���������
	else if (isdigit(ch)) {
		if (ch = '0'){//��0��ͷ������
			ch = INPUTSTR[*forward];
			*forward = *forward + 1;
			//0���x��16����
			if (ch == 'x') {
				ch = INPUTSTR[*forward];
				*forward = *forward + 1;
				while (isxdigit(ch)) {
					ch = INPUTSTR[*forward];
					*forward = *forward + 1;
				}
				*forward = *forward - 1;
				m.token = "16��������";
				return(m);
			}
			//0���0-7���˽�������
			else if(ch >= '0'&&ch <= '7') {
				ch = INPUTSTR[*forward];
				*forward = *forward + 1;
				while (ch >= '0'&&ch <= '7') {
					ch = INPUTSTR[*forward];
					*forward = *forward + 1;
				}
				*forward = *forward - 1;
				m.token = "8��������";
				return(m);
			}
			else {
				*forward = *forward - 1;
				m.token = "10��������";
				return(m);
			}
		}
		else {
			while (ch >= '1'&&ch <= '9') {
				ch = INPUTSTR[*forward];
				*forward = *forward + 1;
			}
			*forward = *forward - 1;
			m.token = "10��������";
			return(m);
		}
		/*ch = INPUTSTR[*forward];
		*forward = *forward + 1;
		while (isdigit(ch)) {
			ch = INPUTSTR[*forward];
			*forward = *forward + 1;
		}
		*forward = *forward - 1;
		m.token = "���ֿ�ͷ���������";
		return(m);*/
	}
	//�жϷ���
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
	tokeninf m("INIT",100);//��ŷ��صĵ�token
	cout << "INPUT INFORMATION: "<<INPUTSTR<< endl<<endl;
	while (m.tokendata>0){
		m=token_scan(&begining,&forward,INPUTSTR);
		if(m.tokendata>0)
			cout << "answer---" << m.token << endl;
	} //ѭ������scan
	return;
}
