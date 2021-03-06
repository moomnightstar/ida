#include<iostream>
#include<stdio.h>
#include<math.h>
#include<time.h>
#include<fstream>
#include<string>
using namespace std;
enum mov_enum
{
	U, D, L, R, F, B  //上下，左右，前后，对应x,z,y加减
};

struct target_location {
	int x, y, z;
}target[25];
void set_target(int tmp, int x, int y, int z) {
	target[tmp].x = x;
	target[tmp].y = y;
	target[tmp].z = z;
}

class cube_numble {
public:
	int numble[3][3][3];
	string path;
	cube_numble *next_node = NULL;
	int h1, h2;
	void output() {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k<3; k++)
					cout << numble[i][j][k] << " ";
				cout << endl;
			}
			cout << endl;
		}
	}
	int cal_h1() {  //错位数码数目
		int sum = 0;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < 3; k++) {
					int tmp = numble[i][j][k];
					if (tmp >= 0 && tmp<25) {
						if (target[tmp].x != i || target[tmp].y != j || target[tmp].z != k) sum++;
					}
				}

			}
		}
		h1 = sum;
		return sum;
	}
	int cal_h2() {//错位数码曼哈顿距离
		int sum = 0;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < 3; k++) {
					int tmp = numble[i][j][k];
					if (tmp != -1) {
						if (target[tmp].x != i || target[tmp].y != j || target[tmp].z != k)
							sum += abs(target[tmp].x - i) + abs(target[tmp].y - j) + abs(target[tmp].z - k);
					}
				}
			}
		}
		h2 = sum;
		return sum;
	}
	struct zero_location {
		int x, y, z;
	}zero;
	bool check_mov(mov_enum e) {
		//判断移动是否合法
		switch (e) {
		case U: if (path.length() != 0) {
			char tmp = path[path.length() - 1];
			if (tmp == 'D') return false;//不允许回到上一个状态
		}
				if (zero.x + 1 < 3) {
					if (numble[zero.x + 1][zero.y][zero.z] != -1)
						return true;
				}
				return false;
		case D: if (path.length() != 0) {
			char tmp = path[path.length() - 1];
			if (tmp == 'U') return false;
		}
				if (zero.x - 1 >= 0) {
					if (numble[zero.x - 1][zero.y][zero.z] != -1)
						return true;
				}
				return false;
		case L:
			if (path.length() != 0) {
				char tmp = path[path.length() - 1];
				if (tmp == 'R') return false;
			}
			if (zero.z - 1 >= 0) {
				if (numble[zero.x][zero.y][zero.z - 1] != -1)
					return true;
			}
			return false;
		case R:
			if (path.length() != 0) {
				char tmp = path[path.length() - 1];
				if (tmp == 'L') return false;
			}
			if (zero.z + 1 < 3) {
				if (numble[zero.x][zero.y][zero.z + 1] != -1)
					return true;
			}
			return false;
		case F:
			if (path.length() != 0) {
				char tmp = path[path.length() - 1];
				if (tmp == 'B') return false;
			}
			if (zero.y + 1 < 3) {
				if (numble[zero.x][zero.y + 1][zero.z] != -1)
					return true;
			}
			return false;
		case B:
			if (path.length() != 0) {
				char tmp = path[path.length() - 1];
				if (tmp == 'F') return false;
			}
			if (zero.y - 1 >= 0) {
				if (numble[zero.x][zero.y - 1][zero.z] != -1)
					return true;
			}
			return false;
		default: return false;
		}
	}
	void print_zero() {
		cout << zero.x << " " << zero.y << " " << zero.z << endl;
	}
	void mov_zero(int x, int y, int z) {
		numble[zero.x][zero.y][zero.z] = numble[x][y][z];
		numble[x][y][z] = 0;
		set_zero(x, y, z);
	}

	void set_zero(int x, int y, int z) {
		zero.x = x;
		zero.y = y;
		zero.z = z;
	}
};

void copy(cube_numble *b, cube_numble *a) {
	b->path = a->path;
	b->zero = a->zero;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++)
				b->numble[i][j][k] = a->numble[i][j][k];
		}
	}
}

bool check_cube(cube_numble *a, cube_numble *b) {
	if (a->h2 != b->h2) return false;
	else {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < 3; k++)
					if (a->numble[i][j][k] != b->numble[i][j][k]) {
						return false;
					}
			}
		}
	}
	return true;
}
bool check_list(cube_numble *p, cube_numble *x) {
	if (p == NULL || x == NULL) {
		return false;
	}
	else {
		while (p != NULL) {
			if (check_cube(p, x) == true) {
				if (p->path.length() > x->path.length()) { //更新结点
					p->path = x->path;
				}
				return true;
			}
			p = p->next_node;
		}
	}
	return false;
}

cube_numble* check_node(cube_numble* open_head, cube_numble *p) {
	cube_numble* open = open_head;
	int tmp = p->cal_h2() + p->path.length();
	if (open == NULL) {
		open = p;
		p->next_node = NULL;
	}
	else {
		if (check_list(open, p)) {
			delete p; //重复状态
		}
		else {//按序插入链表
			cube_numble *q1 = open, *q2;
			while (q1 != NULL) {
				if (tmp < q1->h2 + q1->path.length()) {
					if (q1 == open) {
						p->next_node = q1;
						open = p;
					}
					else {
						q2->next_node = p;
						p->next_node = q1;
					}
					return open;
				}
				else {
					q2 = q1;//q2指向q1的前一个节点；
					q1 = q1->next_node;
				}

			}
			if (q1 == NULL) {
				q2->next_node = p;
				p->next_node = NULL;
			}
		}
	}
	return open;
}

int main() {
	cube_numble *head;
	head = new cube_numble();
	ifstream in1, in2;
	in1.open("input.txt");
	in2.open("target.txt");
	if (!in1 || !in2)//检查文件是否打开
	{
		cerr << "error: unable to open input or target file:" << endl;
		system("pause");
		return -1;
	}
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				in1 >> head->numble[i][j][k];
				if (head->numble[i][j][k] == 0)  head->set_zero(i, j, k);
			}
		}
	}
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				int tmp;
				in2 >> tmp;
				if (tmp == -1) {
					if (head->numble[i][j][k] != -1) {
						cerr << "error:  input.txt can not match target file:" << endl;
						system("pause");
						return -1;
					}
				}
				else  set_target(tmp, i, j, k);
			}
		}
	}

	in1.close();
	in2.close();
	ofstream out;
	out.open("output_IDAh2.txt");
	cube_numble *p, *min_node, *open,*ex_node=NULL;

	clock_t start, finish;
	double totaltime;
	start = clock();
	open = new cube_numble();
	copy(open, head);
	int limit = open->cal_h2();
	while (limit < 999) {
		int next_limit = 999;
		while (open != NULL) {
			if (open->h2 == 0) {
				finish = clock();
				totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
				out << totaltime << endl;
				out << open->path << endl;
				out.close();
				return 0;
			}
			int min_f = open->h2 + open->path.length();
			min_node = open;//每次选取头结点；
			open = min_node->next_node;
						
			if (min_f > limit) {  //大于截断值
				if (min_f < next_limit) next_limit = min_f;
				min_node->next_node=ex_node;
				ex_node = min_node;
			}
			else {
				if (min_node->check_mov(U)) {
					p = new cube_numble();
					copy(p, min_node);
					p->path = p->path + "U";
					p->mov_zero(p->zero.x + 1, p->zero.y, p->zero.z);
					if (p->cal_h2() == 0) {
						finish = clock();
						totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
						out << totaltime << endl;
						out << p->path << endl;
						out.close();
						return 0;
					}
					if (check_list(open, p)) {
						delete p; //重复状态
					}
					else {
						p->next_node = open;
						open = p;
					}
				}
				if (min_node->check_mov(D)) {
					p = new cube_numble();
					copy(p, min_node);
					p->path = p->path + "D";
					p->mov_zero(p->zero.x - 1, p->zero.y, p->zero.z);
					if (p->cal_h2() == 0) {
						finish = clock();
						totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
						out << totaltime << endl;
						out << p->path << endl;
						out.close();
						return 0;
					}
					if (check_list(open, p)) {
						delete p; //重复状态
					}
					else {
						p->next_node = open;
						open = p;
					}
				}
				if (min_node->check_mov(L)) {
					p = new cube_numble();
					copy(p, min_node);
					p->path = p->path + "L";
					p->mov_zero(p->zero.x, p->zero.y, p->zero.z - 1);
					if (p->cal_h2() == 0) {
						finish = clock();
						totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
						out << totaltime << endl;
						out << p->path << endl;
						out.close();
						return 0;
					}
					if (check_list(open, p)) {
						delete p; //重复状态
					}
					else {
						p->next_node = open;
						open = p;
					}
				}
				if (min_node->check_mov(R)) {
					p = new cube_numble();
					copy(p, min_node);
					p->path = p->path + "R";
					p->mov_zero(p->zero.x, p->zero.y, p->zero.z + 1);
					if (p->cal_h2() == 0) {
						finish = clock();
						totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
						out << totaltime << endl;
						out << p->path << endl;
						out.close();
						return 0;
					}
					if (check_list(open, p)) {
						delete p; //重复状态
					}
					else {
						p->next_node = open;
						open = p;
					}
				}
				if (min_node->check_mov(F)) {
					p = new cube_numble();
					copy(p, min_node);
					p->path = p->path + "F";
					p->mov_zero(p->zero.x, p->zero.y + 1, p->zero.z);
					if (p->cal_h2() == 0) {
						finish = clock();
						totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
						out << totaltime << endl;
						out << p->path << endl;
						out.close();
						return 0;
					}
					if (check_list(open, p)) {
						delete p; //重复状态
					}
					else {
						p->next_node = open;
						open = p;
					}
				}
				if (min_node->check_mov(B)) {
					p = new cube_numble();
					copy(p, min_node);
					p->path = p->path + "B";
					p->mov_zero(p->zero.x, p->zero.y - 1, p->zero.z);
					if (p->cal_h2() == 0) {
						finish = clock();
						totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
						out << totaltime << endl;
						out << p->path << endl;
						out.close();
						return 0;
					}
					if (check_list(open, p)) {
						delete p; //重复状态
					}
					else {
						p->next_node = open;
						open = p;
					}
				}
				delete min_node;
			}
		}
		limit = next_limit;
		open = ex_node;
		ex_node = NULL;
	}
	cout << "not found" << endl;
	return 0;
}