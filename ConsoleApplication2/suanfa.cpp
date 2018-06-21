#define _CRT_SECURE_NO_WARNINGS
#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <iomanip>
#include"stdafx.h"
#include<string>


using namespace std;
const int N = 1e3 + 10;

///////// 变量
int pro_num;            // 进程数
int res_num;            // 资源种数
int Available[N];       // 可利用资源向量

int Allocation[N][N];   // 分配矩阵
int Max[N][N];          // 最大需求矩阵
int Need[N][N];         // 需求矩阵
						// Need[i][j] = Max[i][j] - Allocation[i][j];

int Work[N][N];         // 工作向量  表示系统可提供给进程继续运行所需的各类资源数目
						// 为了存储整个过程的 Work 变化所以开了二维的

int Work_add_All[N][N]; // 用来存储安全性检查过程中的Work+Allocation数据

int safe_list[N];       // 安全队列

int safe_count;         // 安全数列中元素个数

bool Finish[N];         // 标记系统是否有足够资源分配给进程并使之运行完成

int re_pro_num;         // 请求进程对应编号
int requset_res[N];     // 请求资源数量

						///////// 函数
void get_res();         // 获取资源相关信息
void get_pro();         // 获取进程相关信息
void request();         // 请求资源
bool safe_check();      // 安全性检测
void menu();            // 菜单栏
void display();         // 展示资源分配状况

int main()
{
	while (true) {
		printf("--------------------银行家算法--------------------\n\n");
		get_res();                  // 获取资源相关信息
		get_pro();                  // 获取进程相关信息
		display();
		printf("\n\n--------------------检查T0时刻的系统安全性--------------------\n\n");
		bool safe = safe_check();   // 检查T0时刻的系统安全性
		bool over = false;          //结束标记
		while (!over) {
			menu();
			int ope;                // 选择操作
			printf("请输入要执行的操作：");
			scanf("%d", &ope);
			if (!safe) printf("\n系统处于不安全状态！\n");
			switch (ope) {
			case 1:
				request();      // 请求资源
				break;
			case 2:
				display();      // 结束本次算法
				break;
			case 3:
				over = true;    // 展示当前资源分配状况
				break;
			default:
				printf("\n***操作无效***\n");
				break;
			}
		}
		printf("--------------------本次算法结束------------------\n\n\n");
	}
	return 0;
}

void get_res() {
	printf("\n请输入资源的种数： ");
	scanf("%d", &res_num);
	for (int i = 1; i <= res_num; i++) {
		printf("\n请输入第%3d 种资源的总数： ", i);
		scanf("%d", &Available[i]);
	}
	printf("------------------------------------------------\n\n");
}

void get_pro() {
	printf("\n\n请输入进程的个数： ");
	scanf("%d", &pro_num);
	for (int i = 1; i <= pro_num; i++) {
		for (int j = 1; j <= res_num; j++) {
			printf("\n请输入第%3d 个进程需要第%3d 种资源的个数： ", i, j);
			scanf("%d", &Max[i][j]);
		}
		for (int j = 1; j <= res_num; j++) {
			printf("\n请输入第%3d 个进程已经分配第%3d 种资源的个数： ", i, j);
			scanf("%d", &Allocation[i][j]);
			Need[i][j] = Max[i][j] - Allocation[i][j];
			Available[j] -= Allocation[i][j];
		}
		printf("\n");
	}

	printf("------------------------------------------------\n\n");
}

void request() {
	printf("\n\n请输入请求资源的进程（第一个进程为P<0>）：");
	scanf("%d", &re_pro_num);
	printf("\n请输入 进程%3d 对%3d 种资源的请求（两种资源之间用空格或换行符隔开）:  ", re_pro_num, res_num);

	bool flag1 = false;     // 标记请求的资源数量是否超过了所需要的资源数量
	bool flag2 = false;     // 标记请求的资源数量是否超过了当前所能利用的资源数量

	for (int i = 1; i <= res_num; i++) {
		scanf("%d", &requset_res[i]);
		if (requset_res[i]>Need[re_pro_num][i]) {
			flag1 = true;
		}
		if (requset_res[i]>Available[i]) {
			flag2 = true;
		}
	}
	if (flag1 || flag2) {
		if (flag1&&flag2) {
			printf("\n请求的资源数量超过了所需要的资源数量和当前所能利用的资源数量!\n\n");
		}
		else if (flag1) {
			printf("\n请求的资源数量超过了所需要的资源数量!\n\n");
		}
		else {
			printf("\n请求的资源数量超过了当前所能利用的资源数量!    等待\n\n");
		}
	}
	else {

		for (int i = 1; i <= res_num; i++) {
			//将资源分配出去
			Available[i] -= requset_res[i];
			//更新进程资源分配状况 即更新 All 和 Need
			Allocation[re_pro_num][i] += requset_res[i];
			Need[re_pro_num][i] -= requset_res[i];
		}

		display();          //展示当前资源分配状况

							//检查系统安全性
		if (safe_check()) {
			//系统安全，分配资源给进程
			//            display();      //展示分配状况
		}
		else {
			//系统不安全，恢复分配资源前可利用资源分配状态

			printf("\n系统不安全，不将资源分配给进程 P<%d> ，令其等待\n", re_pro_num - 1);
			for (int i = 1; i <= res_num; i++) {
				//复原操作
				Available[i] += requset_res[i];
				Allocation[re_pro_num][i] -= requset_res[i];
				Need[re_pro_num][i] += requset_res[i];
			}
		}
	}
	printf("------------------------------------------------\n\n");
}

void show() {
	string str[15] = { "Work|","Need|","All|","Work+All|","Finish|","R","P<>","Pro|","Res","","|" };
	cout << setw(10) << str[7] << setw(5) << str[9];
	for (int i = 1; i<safe_count; i++) cout << setw(7) << "P<" + to_string(safe_list[i]) + ">";
	cout << endl << setw(10) << str[10] << setw(5) << str[8] << endl;

	for (int i = 0; i<5; i++) {
		printf("\n- - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n\n");
		if (i == 4) {
			cout << setw(10) << str[i] << setw(5) << str[9];
			for (int j = 1; j<safe_count; j++) {
				int now = safe_list[j] + 1;
				cout << setw(6) << ((Finish[now] == true) ? "T" : "F") << " ";
			}
			cout << endl;
			break;
		}
		for (int j = 1; j <= res_num; j++) {
			cout << setw(10) << ((j == 1) ? str[i] : str[10]) << setw(5) << str[5] + to_string(j);
			if (i == 0) for (int k = 1; k<safe_count; k++) printf("%6d ", Work[k][j]);
			else if (i == 1) for (int k = 1; k<safe_count; k++) printf("%6d ", Need[safe_list[k] + 1][j]);
			else if (i == 2) for (int k = 1; k<safe_count; k++) printf("%6d ", Allocation[safe_list[k] + 1][j]);
			else for (int k = 1; k<safe_count; k++) printf("%6d ", Work_add_All[k][j]);
			printf("\n");
		}
	}
}

//安全性检测函数
bool safe_check() {

	// 效率 O(n^2)
	safe_count = 1;
	printf("\n--------------------安全性检测--------------------\n\n");
	memset(Finish, false, sizeof Finish);

	for (int i = 1; i <= res_num; i++) {
		Work[1][i] = Available[i];
	}

	for (int i = 1; i <= pro_num; i++) {
		bool flag = false;              // 标记是否还存在 Finish[i] = false && Need[i][j]<=Work[j]
		for (int j = 1; j <= pro_num; j++) {
			if (Finish[j]) continue;
			bool flag2 = true;          // 标记该进程是否满足 Finish[i] = false && Need[i][j]<=Work[j]
			for (int k = 1; k <= res_num; k++) {
				if (Need[j][k] > Work[safe_count][k])
					flag2 = false;
			}
			if (flag2) {
				//若该进程满足条件，则将其加入安全序列，并更新Work向量
				flag = Finish[j] = true;
				safe_list[i] = j - 1;
				for (int k = 1; k <= res_num; k++) {
					Work[safe_count + 1][k] = Work[safe_count][k] + Allocation[j][k];
					Work_add_All[safe_count][k] = Work[safe_count + 1][k];
				}
				safe_count++;
				break;
			}
		}
		if (flag == false && i <= pro_num) {

			show();
			printf("\n--------------------系统不安全--------------------\n\n");
			return false;
		}
	}
	show();
	printf("\n--------------------系统安全--------------------\n\n");
	printf("存在安全序列：{");
	for (int i = 1; i <= pro_num; i++) {
		printf("P<%d>%c", safe_list[i], i == pro_num ? '}' : ',');
	}
	printf("\n");
	return true;
}

void menu() {
	printf("\n    1:请求资源   2:显示当前资源分配状况   3:结束本次银行家算法\n\n");
}


void display() {
	printf("\n-------------------------资源分配情况-----------------------\n\n");
	string str[15] = { "Max|","All|","Need|","Avl|","R","P<>","Pro|","Res","","|" };
	cout << setw(5) << str[6] << setw(5) << str[8];
	for (int i = 0; i<pro_num; i++) cout << setw(6) << "P<" + to_string(i) + ">";
	cout << endl << setw(5) << str[9] << setw(5) << str[7] << endl;

	for (int i = 0; i<4; i++) {
		printf("\n- - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n\n");
		for (int j = 1; j <= res_num; j++) {
			cout << setw(5) << ((j == 1) ? str[i] : str[10]) << setw(5) << str[4] + to_string(j);
			if (i == 0) for (int k = 1; k <= pro_num; k++) printf("%5d ", Max[k][j]);
			else if (i == 1) for (int k = 1; k <= pro_num; k++) printf("%5d ", Allocation[k][j]);
			else if (i == 2) for (int k = 1; k <= pro_num; k++) printf("%5d ", Need[k][j]);
			else printf("%5d ", Available[j]);
			printf("\n");
		}
	}
}
/*
测试样例 3资源 5进程
3 10 5 7 5 7 5 3 0 1 0 3 2 2 2 0 0 9 0 2 3 0 2 2 2 2 2 1 1 4 3 3 0 0 2
*/