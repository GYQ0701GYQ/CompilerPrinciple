#include <iostream>
#include <ctype.h>//handle single ch
#include <stdio.h>//input&output
#include <string>
#define MAX 100
#define KEYWARD {"if","then","else","while","do"}
using namespace std;
char blank = ' '; char tab = '\t'; char newline = '\n';
//Token��Ԫ���ඨ��
class tokeninf{
public:
	string token;//token���ֱ�
	string tokendata;//token������ֵ
	tokeninf() {
		token = "NULL";
	}
	tokeninf(string t_token, string t_data) {
		token = t_token;
		tokendata = t_data;
	}
};
//���ؼ���
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
//16����ת10����
inline tokeninf HEXtoDEC(tokeninf &m) {
	//�ַ�������ȡ����ת������
	int i = 2, ans=0;
	while (m.tokendata[i] != '\0') {
		if (isdigit(m.tokendata[i])) {
			ans = ans*16+(m.tokendata[i++]- 48);
		}
		else {
			ans = ans*16+(m.tokendata[i++] - 87);
		}
	}
	//ת�����intֵ�ٻ���string��ʽ
	string str;
	int cs = ans / 10, ys = ans % 10;
	for (; (cs != 0)||(ys!=0); ys = cs % 10,cs = cs / 10) {
		char ch = ys + 48;
		str = ch + str;
	}
	m.tokendata = str;
	return m;
}
//�˽���ת10����
inline tokeninf OCTtoDEC(tokeninf &m) {
	int i = 0, ans = 0;
	while (m.tokendata[i] != '\0') {
		ans = ans * 8 + (m.tokendata[i++] - 48);
	}
	//ת�����intֵ�ٻ���string��ʽ
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
	char ch;//��ǰ������ַ�
	int i = 0;//��Ǳ���
	ch=INPUTSTR[*forward];
	*forward=*forward+1;//��ǰָ��ǰ��
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
		m.token = "ID";
		for (string s1; (*begining) != (*forward); *begining = *begining + 1) {
			s1 = INPUTSTR[*begining];
			m.tokendata = m.tokendata + s1;
		}
		m=isKeyword(m);
		return(m);
	}
	//��ͷ�������ҽ���������
	else if (isdigit(ch)) {
		if (ch == '0'){//��0��ͷ������
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
				for (string s1; (*begining) != (*forward); *begining = *begining + 1) {
					s1 = INPUTSTR[*begining];
					m.tokendata = m.tokendata + s1;
				}
				m = HEXtoDEC(m);
				m.token = "INT16";
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
	//�жϷ���
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
	tokeninf m("INIT","INITDATA");//��ŷ��صĵ�token
	while (m.token !="NULL"){
		m=token_scan(&begining,&forward,INPUTSTR);
		if(m.token !="NULL")
			cout << "answer---" << m.token <<"  "<<m.tokendata<< endl;
	} //ѭ������scan
	return;
}
