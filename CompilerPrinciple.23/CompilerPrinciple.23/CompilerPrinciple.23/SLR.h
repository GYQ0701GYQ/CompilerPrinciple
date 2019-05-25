
#ifndef SLR_H
#define SLR_H
#include <vector>
#include <stack>
#include <iostream>
#include <string.h>
#include <list>
#include "scanner.h"
using namespace std;
typedef struct genrate {
	bool finish = false;//是否归
	int tag = 3; //圆点位置  
	char * g; //产生式
	bool operator==(genrate &s) {
		if (s.tag == this->tag&&s.g == this->g)
			return true;
		else
			return false;
	}
}genrate;//拓广文法产生式
typedef struct IC {
	int num;//编号
	vector<genrate> I;
	bool operator==(IC &C) {
		if (C.I.size() != this->I.size())
			return false;
		for (int i = 0; i < C.I.size(); i++) {
			if (!(C.I[i] == this->I[i]))
				return false;
		}
		return true;
	}
}IC;//规范集
typedef struct Go {
	int now;
	int next;
	char c;
	bool operator==(Go &h) {
		if (this->now == h.now&&h.next == this->next&&h.c == this->c)
			return true;
		else
			return false;
	}
}Go;//go函数
typedef struct act {
	char ac;
	int state = -2;
}act;//action动作
typedef struct var {
	int num;
	char s;
}var;//存放于slr符号栈中，用来记忆所在识别句子的位置
typedef struct opt {
	int type = 0;
	char op;
	char *a;
	char *b;
	char *c;
}opt;//三地址码操作，有四种类型，运算赋值，赋值，if语句，goto语句，标签 0-5
typedef struct Sc {
	vector<opt> ops;
};//变量.code
typedef struct Pf {
	int start = -1;
	int size = 0;
}pf;//P变量记忆体，记录有多少个S会被规约成一个S
void print();//调整显示函数
#define GLH 23 //产生式个数
char tot[9][10] = { "do","else","id","if","int8","int10","int16","then","while" };
char g[GLH][30] = { "A->P","P->L","P->LP","L->S","S->c=E;","S->dChS","S->iCaS"
				,"S->dChSbS","S->{P}","C->E>E","C->E<E","C->E=E","E->E+T","E->E-T",
				"E->T","T->F","T->T*F","T->T/F","F->(E)","F->c","F->e","F->f","F->g" };//产生式
char togo[6] = "goto";//常用字符串
char tas[3] = ":\n";//常用字符串
vector<char> Vn;//变量
vector<char> Vt;//终结符
vector<char> *first;//first集合
vector<char> *follow;//follow集合
int **got;//goto表
act **actions;//action表
vector<IC> dfa;//规范族
vector<Go> go;//GO函数表
stack<char *> cst;//终结符栈
vector<opt> ss;//T字符数组
list<Sc> ff[3];
list<pf> ps;//p记忆
vector<opt> finalpro;//最终代码
int labcount;//标签数量
void tostring(char *c, int &num, int &index) {
	if (num == 0)
		return;
	int d = num % 10;
	num = num / 10;
	tostring(c, num, index);
	c[index++] = d + '0';
}//序号生成
void findset(char &c, int &index) {
	if (Vn[index] == c)
		return;
	for (int i = 0; i < GLH; i++) {
		if (c == g[i][0]) {
			if (g[i][0] == g[i][3])
				continue;
			if (g[i][3] <= 'Z'&&g[i][3] >= 'A') {
				findset(g[i][3], index);
			}
			else {
				bool flag = true;
				for (int j = 0; j < first[index].size(); j++) {
					if (first[index][j] == g[i][3]) {
						flag = false;
						break;
					}
				}
				if (flag) {
					first[index].push_back(g[i][3]);
				}
			}
		}
	}
}//递归计算first集
void getff() {
	for (int i = 0; i < GLH; i++) {
		bool flag = true;
		for (int k = 0; k < Vn.size(); k++) {
			if (Vn[k] == g[i][0]) {
				flag = false;
				break;
			}
		}
		if (flag)
			Vn.push_back(g[i][0]);
		for (int j = 3; g[i][j] != '\0'; j++) {
			if (g[i][j] <= 'Z'&&g[i][j] >= 'A')
				continue;
			flag = true;
			for (int k = 0; k < Vt.size(); k++) {
				if (Vt[k] == g[i][j]) {
					flag = false;
					break;
				}
			}
			if (flag) {
				Vt.push_back(g[i][j]);
			}
		}
	}
	Vt.push_back('#');
	int length = Vn.size();
	follow = new vector<char>[length];
	first = new vector<char>[length];
	for (int i = 0; i < Vn.size(); i++) {
		for (int j = 0; j < GLH; j++) {
			if (Vn[i] == g[j][0]) {
				if (g[i][0] == g[i][3])
					continue;
				if (g[j][3] <= 'Z'&&g[j][3] >= 'A')
					findset(g[j][3], i);
				else {
					bool flag = true;
					for (int k = 0; k < first[i].size(); k++) {
						if ((first[i][k] == g[j][3])) {
							flag = false;
							break;
						}
					}

					if (flag)
						first[i].push_back(g[j][3]);
				}

			}
		}
	}
	for (int i = 0; i < length; i++)
		follow[i].push_back('#');
	int befor = -1;
	int last = 0;
	while (befor != last) {
		for (int i = 0; i < length; i++) {

			for (int j = 0; j < GLH; j++) {
				for (int k = 3; g[j][k] != '\0'; k++) {
					if (Vn[i] == g[j][k]) {
						if (g[j][k + 1] >= 'A'&&g[j][k + 1] <= 'Z') {
							int index = 0;
							while (g[j][k + 1] != Vn[index])
								index++;
							for (int l = 0; l < first[index].size(); l++) {
								bool flag = true;
								for (int m = 0; m < follow[i].size(); m++) {
									if (follow[i][m] == first[index][l]) {
										flag = false;
										break;
									}
								}
								if (flag) {
									follow[i].push_back(first[index][l]);
								}

							}
						}
						else if (g[j][0] != Vn[i] && g[j][k + 1] == '\0')
						{
							int index = 0;
							while (g[j][0] != Vn[index])
								index++;

							for (int m = 0; m < follow[index].size(); m++) {
								bool flag = true;
								for (int n = 0; n < follow[i].size(); n++) {
									if (follow[i][n] == follow[index][m]) {
										flag = false;
										break;
									}
								}
								if (flag) {
									follow[i].push_back(follow[index][m]);
								}
							}

						}
						else if (g[j][k + 1] != '\0') {
							bool flag = true;
							for (int m = 0; m < follow[i].size(); m++) {
								if (follow[i][m] == g[j][k + 1]) {
									flag = false;
									break;
								}
							}
							if (flag) {
								follow[i].push_back(g[j][k + 1]);
							}

						}
					}
				}
			}
		}
		befor = last;
		last = 0;
		for (int i = 0; i < length; i++) {
			last += follow[i].size();
		}
	}
	/*cout << "变量" << endl;
	for (int i = 0; i < Vn.size(); i++) {
		std::cout << Vn[i] << " ";
	}
	cout << "\n终结符" << endl;
	for (int i = 0; i < Vt.size(); i++) {
		std::cout << Vt[i] << " ";
	}
	cout << "\nfirst集" << endl;
	for (int i = 0; i < Vn.size(); i++) {
		std::cout << Vn[i] << endl;
		for (int j = 0; j < first[i].size(); j++) {
			std::cout << first[i][j] << " ";
		}
		putchar('\n');
	}
	cout << "\nfollow集" << endl;
	for (int i = 0; i < Vn.size(); i++) {
		std::cout << Vn[i] << endl;
		for (int j = 0; j < follow[i].size(); j++) {
			std::cout << follow[i][j] << " ";
		}
		putchar('\n');
	}*/

}//计算first和follow集合
void getIC(IC &ic, genrate &s) {
	if (s.g[s.tag] == '\0') {
		s.finish = true;
		ic.I.push_back(s);
		return;

	}
	ic.I.push_back(s);
	for (int i = 0; i < Vn.size(); i++) {
		if (s.g[s.tag] == Vn[i]) {
			for (int j = 0; j < GLH; j++) {
				if (g[j][0] == Vn[i]) {
					genrate tl;
					tl.g = g[j];
					bool flag = true;
					for (int k = 0; k < ic.I.size(); k++) {
						if (ic.I[k] == tl) {
							flag = false;
							break;
						}
					}
					if (flag)
						getIC(ic, tl);
				}
			}
		}
	}

}//状态转移
int findloc(char &s) {
	if (s <= 'Z'&&s >= 'A') {
		for (int i = 0; i < Vn.size(); i++) {
			if (Vn[i] == s)
				return i;
		}
	}
	else {
		for (int i = 0; i < Vt.size(); i++) {
			if (Vt[i] == s)
				return i;
		}
	}
}//查找终结符或变量所在位置
int findgo(int &in, char &s) {
	for (int i = 0; i < go.size(); i++) {
		if (go[i].now == in && go[i].c == s)
			return go[i].next;
	}
	return -1;
}//查找go表
void getdtable() {
	genrate f;
	f.g = g[0];
	IC tmp;
	tmp.num = 0;
	getIC(tmp, f);
	dfa.push_back(tmp);
	int size = -1;
	while (dfa.size() != size) {
		size = dfa.size();
		for (int i = 0; i < dfa.size(); i++) {
			for (int j = 0; j < Vn.size(); j++) {
				IC tmp;
				Go tg;
				for (int k = 0; k < dfa[i].I.size(); k++) {
					if (dfa[i].I[k].g[dfa[i].I[k].tag] != '\0'&&dfa[i].I[k].g[dfa[i].I[k].tag] == Vn[j]) {
						genrate s;
						s.g = dfa[i].I[k].g;
						s.tag = dfa[i].I[k].tag + 1;
						getIC(tmp, s);
					}
				}
				if (tmp.I.size() == 0)
					continue;
				int flag = true;
				for (int m = 0; m < dfa.size(); m++) {
					if (tmp == dfa[m]) {
						tg.c = Vn[j];
						tg.next = m;
						tg.now = i;
						//go.push_back(tg);
						flag = false;
						break;
					}
				}
				if (flag) {
					tmp.num = dfa.size();
					tg.c = Vn[j];
					tg.next = tmp.num;
					tg.now = i;
					dfa.push_back(tmp);
					//go.push_back(tg);
				}
				flag = true;
				for (int n = 0; n < go.size(); n++) {
					if (go[n] == tg) {
						flag = false;
						break;
					}
				}
				if (flag)
					go.push_back(tg);

			}
			for (int j = 0; j < Vt.size(); j++) {
				IC tmp;
				Go tg;
				for (int k = 0; k < dfa[i].I.size(); k++) {
					if (dfa[i].I[k].g[dfa[i].I[k].tag] != '\0'&&dfa[i].I[k].g[dfa[i].I[k].tag] == Vt[j]) {
						genrate s;
						s.g = dfa[i].I[k].g;
						s.tag = dfa[i].I[k].tag + 1;
						getIC(tmp, s);
					}
				}
				if (tmp.I.size() == 0)
					continue;
				int flag = true;
				for (int m = 0; m < dfa.size(); m++) {
					if (tmp == dfa[m]) {
						tg.c = Vt[j];
						tg.next = m;
						tg.now = i;
						flag = false;
						break;
					}
				}
				if (flag) {
					tmp.num = dfa.size();
					tg.c = Vt[j];
					tg.next = tmp.num;
					tg.now = i;
					dfa.push_back(tmp);
					//go.push_back(tg);
				}
				flag = true;
				for (int n = 0; n < go.size(); n++) {
					if (go[n] == tg) {
						flag = false;
						break;
					}
				}
				if (flag)
					go.push_back(tg);

			}
		}
	}
	actions = (act**)new act*[dfa.size()];
	for (int i = 0; i < dfa.size(); i++) {
		actions[i] = new act[Vt.size()];
	}
	got = (int**)new int*[dfa.size()];
	for (int i = 0; i < dfa.size(); i++) {
		got[i] = new int[Vn.size()];
	}
	for (int i = 0; i < dfa.size(); i++) {
		for (int j = 0; j < dfa[i].I.size(); j++) {
			if (dfa[i].I[j].g == g[0] && dfa[i].I[j].finish) {
				actions[i][Vt.size() - 1] = { 'a',-1 };
			}
			else if (dfa[i].I[j].finish) {
				for (int k = 0; k < GLH; k++) {
					if (dfa[i].I[j].g == g[k]) {
						int c = findloc(g[k][0]);
						for (int l = 0; l < follow[c].size(); l++) {
							char cc = follow[c][l];
							actions[i][findloc(cc)].ac = 'r';
							actions[i][findloc(cc)].state = k;
						}
						break;
					}
				}
			}
			else if (dfa[i].I[j].g[dfa[i].I[j].tag] <= 'Z'&&dfa[i].I[j].g[dfa[i].I[j].tag] >= 'A') {
				char c = dfa[i].I[j].g[dfa[i].I[j].tag];
				got[i][findloc(c)] = findgo(i, c);
			}
			else {
				char c = dfa[i].I[j].g[dfa[i].I[j].tag];
				actions[i][findloc(c)].ac = 's';
				actions[i][findloc(c)].state = findgo(i, c);
			}

		}
	}
	char c = 'b';
	int sta = 34;
	actions[34][findloc(c)].ac = 's';
	actions[34][findloc(c)].state = findgo(sta, c);
	/*cout << "actions" << endl;
	for (int i = 0; i < dfa.size(); i++) {
		for (int j = 0; j < Vt.size(); j++) {
			if (actions[i][j].state >=0)
				printf("action(%d,%c)=%c%d\n",i,Vt[j],actions[i][j].ac, actions[i][j].state);
		}
	}
	/*cout << "goto" << endl;
	for (int i = 0; i < dfa.size(); i++) {
		for (int j = 0; j < Vn.size(); j++) {
			if (got[i][j] > -1)
				printf("goto(%d,%c)=%d\n", i, Vn[j], got[i][j]);

		}
	}*/
	/*for (int i = 0; i < dfa.size(); i++) {
		cout << "I" << dfa[i].num << endl;
		for (int j = 0; j < dfa[i].I.size(); j++) {
			bool flag = true;
			for (int k = 0; dfa[i].I[j].g[k] != '\0'; k++) {
				if (k== dfa[i].I[j].tag) {
					putchar('.');
					flag = false;
				}
					putchar(dfa[i].I[j].g[k]);
			}
			if (flag) {
				putchar('.');
			}
			putchar('\n');
		}
	}
	for (int i = 0; i < go.size(); i++) {
		cout << "GO(" << go[i].now << "," << go[i].c << ")=" << go[i].next << endl;
	}*/
}//建立规范族，并且建立action和goto表
void charstack(int i, std::stack<var> &st, word* wd, std::stack<int> &ist, int &bi) {
	static int index = 1;
	static int tag = 1;
	static pf tmf;
	pf tmpf;
	int dd;
	int length;
	opt tmp;
	Sc vn;
	char *str=NULL;//生成标签1
	char *str1=NULL;//生成标签2
	char *str2=NULL;//生成标签3
	bool flag1;//标签标记1
	bool flag2;//标签标记2
	bool flag3;//标签标记3
	switch (i)
	{
	case 1:
		st.pop();
		ist.pop();
		ps.push_back(tmf);
		tmf.size = 0;
		tmf.start = -1;
		break;
	case 2:
		for (int j = 0; j < 2; j++) {
			st.pop();
			ist.pop();
		}
		break;
	case 3:
		st.pop();
		ist.pop();
		if (tmf.start == -1)
			tmf.start = ff[2].size() - 1;
		tmf.size++;
		break;
	case 4:
		for (int j = 0; j < 4; j++) {
			if (j == 3) {
				tmp.a = wd[st.top().num].w;
			}
			st.pop();
			ist.pop();
		}
		tmp.b = cst.top();
		cst.pop();
		tmp.type = 1;

		for (int i = 0; i < ff[0].back().ops.size(); i++) {
			vn.ops.push_back(ff[0].back().ops[i]);
		}
		ff[0].pop_back();
		vn.ops.push_back(tmp);
		ff[2].push_back(vn);
		break;
	case 5:
		for (int j = 0; j < 4; j++) {
			st.pop();
			ist.pop();
		}
		for (int i = 0; i < (ff[1].back()).ops.size(); i++) {
			vn.ops.push_back((ff[1].back()).ops[i]);
		}
		ff[1].pop_back();
		if (ff[2].back().ops.front().type != 4) {
			dd = tag++;
			str = new char[10];
			str[0] = 'L';
			length = 1;
			tostring(str, dd, length);
			str[length] = '\0';
			tmp.type = 3;
			tmp.b = str;
		}
		else {
			tmp.b = ff[2].back().ops.front().b;
		}
		tmp.type = 3;
		tmp.a = togo;
		vn.ops.push_back(tmp);

		if (ff[2].back().ops.back().type != 4) {
			dd = tag++;
			str1 = new char[10];
			str1[0] = 'L';
			length = 1;
			tostring(str1, dd, length);
			str1[length] = '\0';
			tmp.b = str1;
		}
		else {
			tmp.b = ff[2].back().ops.back().b;
		}
		tmp.type = 3;
		vn.ops.push_back(tmp);

		if (ff[2].back().ops.front().type != 4) {
			tmp.type = 4;
			tmp.b = str;
			tmp.c = tas;
			vn.ops.push_back(tmp);
		}

		for (int i = 0; i < ff[2].back().ops.size(); i++) {
			vn.ops.push_back(ff[2].back().ops[i]);
		}

		if (ff[2].back().ops.back().type != 4) {
			tmp.type = 4;
			tmp.b = str1;
			tmp.c = tas;
			vn.ops.push_back(tmp);
		}
		ff[2].pop_back();
		ff[2].push_back(vn);
		break;
	case 6:
		for (int j = 0; j < 4; j++) {
			st.pop();
			ist.pop();
		}
		flag1 = true;
		if (ff[2].size() > 1) {
			dd = -1;
			for (auto i = ff[2].begin(); i != ff[2].end(); i++) {
				dd++;
				if (dd == ff[2].size() - 2) {
					if ((*i).ops.back().type == 4) {
						str = (*i).ops.back().b;
						flag1 = false;
					}
					break;
				}

			}
		}

		if (flag1) {
			dd = tag++;
			str = new char[10];
			str[0] = 'L';
			length = 1;
			tostring(str, dd, length);
			str[length] = '\0';
			tmp.type = 4;
			tmp.b = str;
			tmp.c = tas;
			vn.ops.push_back(tmp);
		}


		for (int i = 0; i < (ff[1].back()).ops.size(); i++) {
			vn.ops.push_back((ff[1].back()).ops[i]);
		}
		ff[1].pop_back();

		if ((ff[2].back()).ops.front().type != 4) {
			dd = tag++;
			str1 = new char[10];
			str1[0] = 'L';
			length = 1;
			tostring(str1, dd, length);
			str1[length] = '\0';
			tmp.b = str1;
		}
		else {
			tmp.b = (ff[2].back()).ops.front().b;
		}
		tmp.type = 3;
		tmp.a = togo;
		vn.ops.push_back(tmp);



		dd = tag++;
		str2 = new char[10];
		str2[0] = 'L';
		length = 1;
		tostring(str2, dd, length);
		str2[length] = '\0';
		tmp.b = str2;
		vn.ops.push_back(tmp);

		if ((ff[2].back()).ops.front().type != 4) {
			tmp.type = 4;
			tmp.b = str1;
			tmp.c = tas;
			vn.ops.push_back(tmp);
		}

		for (int i = 0; i < (ff[2].back()).ops.size(); i++) {
			vn.ops.push_back((ff[2].back()).ops[i]);
		}

		tmp.type = 3;
		tmp.a = togo;
		tmp.b = str;
		vn.ops.push_back(tmp);
		tmp.type = 4;
		tmp.b = str2;
		tmp.c = tas;
		vn.ops.push_back(tmp);


		ff[2].pop_back();
		ff[2].push_back(vn);
		break;
	case 7:
		flag1 = true;
		flag2 = true;
		flag3 = true;
		for (int j = 0; j < 6; j++) {
			st.pop();
			ist.pop();
		}
		for (int i = 0; i < (ff[1].back()).ops.size(); i++) {
			vn.ops.push_back((ff[1].back()).ops[i]);
		}
		ff[1].pop_back();

		dd = -1;
		for (auto i = ff[2].begin(); i != ff[2].end(); i++) {
			dd++;
			if (dd == ff[2].size() - 2) {
				if ((*i).ops.front().type == 4) {
					flag1 = false;
					str = (*i).ops.front().b;
				}
			}
			if (dd == ff[2].size() - 1) {
				if ((*i).ops.front().type == 4) {
					flag2 = false;
					str1 = (*i).ops.front().b;
				}
				if ((*i).ops.back().type == 4) {
					flag3 = false;
					str2 = (*i).ops.back().b;
				}
			}
		}


		if (flag1) {
			dd = tag++;
			str = new char[10];
			str[0] = 'L';
			length = 1;
			tostring(str, dd, length);
			str[length] = '\0';
		}
		tmp.type = 3;
		tmp.b = str;
		tmp.a = togo;
		vn.ops.push_back(tmp);

		if (flag2) {
			dd = tag++;
			str1 = new char[10];
			str1[0] = 'L';
			length = 1;
			tostring(str1, dd, length);
			str1[length] = '\0';
		}
		tmp.b = str1;
		vn.ops.push_back(tmp);


		if (flag1) {
			tmp.type = 4;
			tmp.b = str;
			tmp.c = tas;
			vn.ops.push_back(tmp);
		}


		dd = -1;
		for (auto i = ff[2].begin(); i != ff[2].end(); i++) {
			dd++;
			if (dd == ff[2].size() - 2) {
				for (int j = 0; j < (*i).ops.size(); j++) {
					vn.ops.push_back((*i).ops[j]);
				}
				break;
			}
		}
		if (flag3) {
			dd = tag++;
			str2 = new char[10];
			str2[0] = 'L';
			length = 1;
			tostring(str2, dd, length);
			str2[length] = '\0';
		}
		tmp.type = 3;
		tmp.b = str2;
		tmp.a = togo;
		vn.ops.push_back(tmp);

		if (flag2) {
			tmp.type = 4;
			tmp.b = str1;
			tmp.c = tas;
			vn.ops.push_back(tmp);
		}

		dd = -1;
		for (auto i = ff[2].begin(); i != ff[2].end(); i++) {
			dd++;
			if (dd == ff[2].size() - 1) {
				for (int j = 0; j < (*i).ops.size(); j++) {
					vn.ops.push_back((*i).ops[j]);
				}
				break;
			}
		}
		ff[2].pop_back();
		ff[2].pop_back();

		if (flag3) {
			tmp.type = 4;
			tmp.b = str2;
			tmp.c = tas;
			vn.ops.push_back(tmp);
		}


		ff[2].push_back(vn);

		break;
	case 8:
		for (int j = 0; j < 3; j++) {
			st.pop();
			ist.pop();
		}

		tmpf = ps.front();
		ps.pop_front();
		dd = -1;

		for (auto it = ff[2].begin(); it != ff[2].end(); it++) {
			dd++;
			if (dd == tmpf.start) {
				for (int j = 0; j < tmpf.size; j++) {
					for (int k = 0; k < (*it).ops.size(); k++) {
						vn.ops.push_back((*it).ops[k]);
					}
					it = ff[2].erase(it);
					if (j == tmpf.size - 1) {
						ff[2].insert(it, vn);
					}
				}
				break;
			}
		}

		break;
	case 9:
	case 10:
	case 11:
		for (int j = 0; j < 3; j++) {
			st.pop();
			ist.pop();
		}
		for (int j = 0; j < 2; j++) {
			for (int i = 0; i < (*ff[0].begin()).ops.size(); i++) {
				vn.ops.push_back((*ff[0].begin()).ops[i]);
			}
			ff[0].pop_front();
		}
		tmp.c = cst.top();
		cst.pop();
		tmp.b = cst.top();
		switch (i)
		{
		case  9:
			tmp.op = '>';
			break;
		case 10:
			tmp.op = '<';
			break;
		case 11:
			tmp.op = '=';
			break;
		default:
			break;
		}
		cst.pop();
		tmp.type = 2;
		vn.ops.push_back(tmp);
		ff[1].push_back(vn);
		break;
	case 12:
	case 13:
		for (int j = 0; j < 3; j++) {
			st.pop();
			ist.pop();
		}
		tmp.op = g[i][4];
		str = new char[10];
		str[0] = 't';
		dd = index++;
		length = 1;
		tostring(str, dd, length);
		str[length] = '\0';
		tmp.c = cst.top();
		cst.pop();
		tmp.b = cst.top();
		cst.pop();
		cst.push(str);
		tmp.a = str;
		tmp.type = 0;
		for (auto i = ff[0].begin(); i != ff[0].end(); i++) {
			for (int j = 0; j < (*i).ops.size(); j++) {
				vn.ops.push_back((*i).ops[j]);
			}
		}
		ff[0].clear();
		for (int i = 0; i < ss.size(); i++) {
			vn.ops.push_back(ss[i]);
		}
		ss.clear();
		vn.ops.push_back(tmp);
		ff[0].push_back(vn);
		break;
	case 14:
		st.pop();
		ist.pop();
		for (int i = 0; i < ss.size(); i++) {
			vn.ops.push_back(ss[i]);
		}
		ss.clear();
		ff[0].push_back(vn);
		break;
	case 15:
		st.pop();
		ist.pop();
		break;
	case 16:
	case 17:
		for (int j = 0; j < 3; j++) {
			st.pop();
			ist.pop();
		}
		tmp.op = g[i][4];
		str = new char[10];
		str[0] = 't';
		dd = index++;
		length = 1;
		tostring(str, dd, length);
		str[length] = '\0';
		tmp.c = cst.top();
		cst.pop();
		tmp.b = cst.top();
		cst.pop();
		cst.push(str);
		tmp.a = str;
		tmp.type = 0;
		ss.push_back(tmp);
		break;
	case 18:
		for (int j = 0; j < 3; j++) {
			st.pop();
			ist.pop();
		}
		break;
	case 19:
	case 20:
	case 21:
	case 22:
		var s = st.top();
		st.pop();
		ist.pop();
		if (i == 19)
			cst.push(wd[s.num].w);
		else
			cst.push(wd[s.num].lexval);
		break;
	default:
		break;
	}
	if (tag - 1 != labcount)
		labcount = tag - 1;
}//语义分析
void slr() {
	word* wd = scaner();
	stack<int> state;
	stack<var> symbol;
	state.push(0);
	symbol.push({ -1,'#' });
	int index = 0;
	for (int i = 0; i < bufindex; ) {
		switch (actions[state.top()][findloc(wd[i].t)].ac) {
		case 'r':
			index = actions[state.top()][findloc(wd[i].t)].state;
			charstack(index, symbol, wd, state, i);
			symbol.push({ -1,g[index][0] });
			state.push(got[state.top()][findloc(symbol.top().s)]);
			//printf("%s\n", &g[index]);
			break;
		case 's':
			symbol.push({ i,wd[i].t });
			state.push(actions[state.top()][findloc(wd[i].t)].state);
			i++;
			break;
		case 'a':
			printf("Accept\n");
			return;
			break;
		default:
			printf("error");
			return;
			break;
		}
	}
}//语法分析
void print() {
	finalpro.clear();
	for (auto i = ff[2].begin(); i != ff[2].end(); i++) {
		for (int j = 0; j < (*i).ops.size(); j++) {
			finalpro.push_back((*i).ops[j]);
		}
	}//拼接
	int *labs = new int[labcount];
	for (int i = 0; i < finalpro.size(); i++) {
		if (finalpro[i].type == 4) {
			int sum = 0;
			for (int j = 1; finalpro[i].b[j] != '\0'; j++) {
				sum = sum * 10 + finalpro[i].b[j] - '0';
			}
			labs[sum - 1] = i;
		}
	}//记录标签位置
	bool flag = true;
	while (flag) {
		flag = false;
		for (int i = 0; i < finalpro.size(); i++) {
			if (finalpro[i].type == 3) {
				int sum = 0;
				for (int j = 1; finalpro[i].b[j] != '\0'; j++) {
					sum = sum * 10 + finalpro[i].b[j] - '0';
				}
				if ((labs[sum - 1] + 1 < finalpro.size()) && finalpro[labs[sum - 1] + 1].type == 3) {
					finalpro[i].b = finalpro[labs[sum - 1] + 1].b;
					flag = true;
				}
			}
		}
	}//修改冗余跳转
	int *labuse = new int[labcount];
	for (int i = 0; i < labcount; i++) {
		labuse[i] = 0;
	}
	for (int i = 0; i < finalpro.size(); i++) {
		if (finalpro[i].type == 3) {
			int sum = 0;
			for (int j = 1; finalpro[i].b[j] != '\0'; j++) {
				sum = sum * 10 + finalpro[i].b[j] - '0';
			}
			labuse[sum - 1]++;
		}
	}//记录标签使用情况
	for (auto i = finalpro.begin(); i != finalpro.end();) {
		if ((*i).type == 4) {
			int sum = 0;
			for (int j = 1; (*i).b[j] != '\0'; j++) {
				sum = sum * 10 + (*i).b[j] - '0';
			}
			if (labuse[sum - 1] == 0) {
				i = finalpro.erase(i);
				continue;
			}
			i++;
		}
		else
			i++;
	}//删除多余标签
	for (auto i = finalpro.begin(); i != finalpro.end();) {
		if ((*i).type == 2) {
			i++;
			i++;
			continue;
		}
		if ((*i).type == 3) {
			auto l = i;
			l++;
			if (l != finalpro.end() && (*l).type == 3) {
				i = finalpro.erase(l);
				i--;
				continue;
			}
		}
		i++;
	}//删除多余goto语句
	for (int j = 0; j < finalpro.size(); j++) {
		switch (finalpro[j].type)
		{
		case 0:
			cout << "  " << finalpro[j].a << " := " << finalpro[j].b << finalpro[j].op << finalpro[j].c << endl;
			break;

		case 1:
			cout << "  " << finalpro[j].a << " := " << finalpro[j].b << endl;
			break;

		case 2:
			cout << "  " << "if " << finalpro[j].b << finalpro[j].op << finalpro[j].c;
			break;
		case 3:
			cout << "  " << finalpro[j].a << " " << finalpro[j].b << endl;
			break;
		case 4:
			cout << finalpro[j].b << finalpro[j].c;
			break;
		default:
			break;
		}
	}
}//调整并显示；

#endif // !SLR_H


