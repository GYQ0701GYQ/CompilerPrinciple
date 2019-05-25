#define _CRT_SECURE_NO_WARNINGS//VS认为fopen不安全，此句可跳过该警告
#ifndef SCANER_H
#define SCANER_H
#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
static int bufindex = 0;
typedef struct word {
	int  type;
	char w[50];
	char  lexval[20];
	char t = '@';
}word;
char Keep[5][8] = {
	"do","else","if","while","then"
};
char ds[6][20] = {
	"标识符","十进制整数","十六进制整数","八进制整数","运算符","保留字"
};
inline bool isLetter(char &c);//字母识别
inline bool isNumber(char &c); //数字
inline bool isCa(char &c);//运算符
inline bool isS(char &c);//空格换行tab
int streq(char *s, int start, int end); //保留字比较
void pstrcpy(char *str1, char *str2, int s, int e); //字符串复制
void itostr(char *s1, int num);
word *scaner() {
	bufindex = 0;
	word *buf = (word*)malloc(sizeof(word) * 1024);
	bufindex = 0;
	FILE* ff;
	ff = fopen("testfile.txt", "r"); //文本路径
	if (ff == NULL)
		return NULL;
	char s[1024];
	int tag = 0;
	char c;
	bool endflag = false;
	while (!endflag) {
		c = fgetc(ff);
		if (c == EOF)
			endflag = true;
		else
			s[tag++] = c;
		if (tag == 8096 || (tag > 0 && endflag)) {
			int start = 0;
			int end = 0;
			bool reset = false;
			while (start < tag) {

				if (isS(s[start])) {
					end++;
					start++;
					continue;
				}
				if (isCa(s[start])) {
					buf[bufindex].type = 4;
					buf[bufindex].w[0] = s[start];
					buf[bufindex].w[1] = '\0';
					buf[bufindex].t = s[start];
					bufindex++;
					end++;
				}
				if (isNumber(s[start])) {
					if (s[start] == '0') {
						if (s[++end] == 'x') {
							while (end < tag - 1 && (isNumber(s[++end]) || (s[end] <= 'f'&&s[end] >= 'a')));
							if (end < tag - 1 || endflag) {
								if (isNumber(s[end]) || (s[end] <= 'f'&&s[end] >= 'a'))
									end++;
								pstrcpy(s, buf[bufindex].w, start, end);
								buf[bufindex].type = 2;
								buf[bufindex].t = 'g';
								bufindex++;
							}
							else {
								tag = 0;
								for (int i = start; i <= end; i++) {
									s[tag++] = s[i];
								}
								reset = true;
							}
						}
						else if (!isNumber(s[end])) {
							buf[bufindex].type = 1;
							buf[bufindex].w[0] = '0';
							buf[bufindex].w[1] = '\0';
							buf[bufindex].t = 'f';
							bufindex++;
						}
						else
						{
							while (end < tag - 1 && isNumber(s[++end]));
							if (end < tag - 1 || endflag) {
								if (isNumber(s[end]))
									end++;
								if (!isLetter(s[end])) {
									pstrcpy(s, buf[bufindex].w, start, end);
									buf[bufindex].type = 3;
									buf[bufindex].t = 'e';
									bufindex++;
								}
							}
							else {
								tag = 0;
								for (int i = start; i <= end; i++) {
									s[tag++] = s[i];
								}
								reset = true;
							}
						}
					}
					else {
						while (end < tag - 1 && isNumber(s[++end]));
						if (end < tag - 1 || endflag) {
							if (isNumber(s[end]))
								end++;
							buf[bufindex].type = 1;
							buf[bufindex].t = 'f';
							pstrcpy(s, buf[bufindex].w, start, end);
							bufindex++;

						}
						else {
							tag = 0;
							for (int i = start; i <= end; i++) {
								s[tag++] = s[i];
							}
							reset = true;
						}
					}
				}

				if (reset)
					break;
				if (isLetter(s[start])) {
					bool nflag = false;
					while ((end < tag - 1) && (isLetter(s[++end]) || isNumber(s[end])))
						if (!nflag&&isNumber(s[end]))
							nflag = true;
					if (end < tag - 1 || endflag) {
						if (isLetter(s[end]) || isNumber(s[end]))
							end++;
						if (nflag) {
							pstrcpy(s, buf[bufindex].w, start, end);
							buf[bufindex].type = 0;
							buf[bufindex].t = 'c';
							bufindex++;
						}
						else {
							int re = streq(s, start, end);
							if (re >= 0) {
								buf[bufindex].type = 5;
								buf[bufindex].w[0] = re;
								switch (re) {
								case 0:
									buf[bufindex].t = 'a';
									break;
								case 1:
									buf[bufindex].t = 'b';
									break;
								case 2:
									buf[bufindex].t = 'd';
									break;
								case 3:
									buf[bufindex].t = 'i';
									break;
								case 4:
									buf[bufindex].t = 'h';
									break;
								default:
									break;
								}
								bufindex++;
							}
							else {
								buf[bufindex].type = 0;
								buf[bufindex].t = 'c';
								pstrcpy(s, buf[bufindex].w, start, end);
								bufindex++;
							}
						}

					}
					else {
						tag = 0;
						for (int i = start; i <= end; i++) {
							s[tag++] = s[i];
						}
						reset = true;
					}
				}
				start = end;
			}
			if (!reset)
				tag = 0;
		}
	}
	fclose(ff);
	int i = 0;
	int sum = 0;
	int j = 0;
	for (; i < bufindex; i++) {

		switch (buf[i].type)
		{
		case 1:
			sum = 0;
			j = 0;
			for (; buf[i].w[j] != '\0'; j++)
				sum = sum * 10 + buf[i].w[j] - '0';
			itostr(buf[i].lexval, sum);
			break;
		case 2:
			sum = 0;
			j = 2;
			for (; buf[i].w[j] != '\0'; j++) {
				if (isLetter(buf[i].w[j]))
					sum = sum * 16 + buf[i].w[j] - 'a' + 10;
				else
					sum = sum * 16 + buf[i].w[j] - '0';
			}
			itostr(buf[i].lexval, sum);
			break;
		case 3:
			sum = 0;
			j = 1;
			for (; buf[i].w[j] != '\0'; j++)
				sum = sum * 8 + buf[i].w[j] - '0';
			itostr(buf[i].lexval, sum);
			break;
		default:
			break;
		}
	}
	buf[bufindex++].t = '#';

	return buf;
}


inline bool isLetter(char &c) {
	if (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z'))
		return true;
	else
		return false;
};
inline bool isNumber(char &c) {
	if ((c >= '0') && (c <= '9'))
		return true;
	else
		return false;
};
inline bool isCa(char &c) {
	switch (c) {
	case '+':
	case '-':
	case '*':
	case '/':
	case '>':
	case '<':
	case '=':
	case '(':
	case ')':
	case '{':
	case '}':
	case ';':
		return true;
		break;
	default:
		break;
	}
	return false;

};
inline bool isS(char &c) {
	if (c == '\t' || c == '\n' || c == ' ')
		return true;
	else
		return false;
};
int streq(char *s, int start, int end) {
	for (int i = 0; i < 5; i++) {
		if (Keep[i][0] == s[start]) {
			int index = 0;
			while (Keep[i][index] != '\0'&&start < end) {
				if (Keep[i][index++] != s[start++]) {
					return -1;
				}
			}
			if (Keep[i][index] == '\0'&&start == end)
				return i;
		}
	}
	return -1;
};
void pstrcpy(char *str1, char *str2, int s, int e) {
	int index = 0;
	for (int i = s; i < e; i++) {
		str2[index++] = str1[i];
	}
	str2[index] = '\0';
}
void itostr(char *s1, int num) {
	char st[30];
	int index = -1;
	if (num == 0) {
		s1[0] = '0';
		s1[1] = '\0';
		return;
	}
	while (num > 0) {
		st[++index] = num % 10 + '0';
		num = num / 10;
	}
	for (int i = 0; index - i >= 0; i++) {
		s1[i] = st[index - i];
	}
	s1[index + 1] = '\0';
}
#endif // !SCANER_H


