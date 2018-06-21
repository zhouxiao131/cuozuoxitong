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

///////// ����
int pro_num;            // ������
int res_num;            // ��Դ����
int Available[N];       // ��������Դ����

int Allocation[N][N];   // �������
int Max[N][N];          // ����������
int Need[N][N];         // �������
						// Need[i][j] = Max[i][j] - Allocation[i][j];

int Work[N][N];         // ��������  ��ʾϵͳ���ṩ�����̼�����������ĸ�����Դ��Ŀ
						// Ϊ�˴洢�������̵� Work �仯���Կ��˶�ά��

int Work_add_All[N][N]; // �����洢��ȫ�Լ������е�Work+Allocation����

int safe_list[N];       // ��ȫ����

int safe_count;         // ��ȫ������Ԫ�ظ���

bool Finish[N];         // ���ϵͳ�Ƿ����㹻��Դ��������̲�ʹ֮�������

int re_pro_num;         // ������̶�Ӧ���
int requset_res[N];     // ������Դ����

						///////// ����
void get_res();         // ��ȡ��Դ�����Ϣ
void get_pro();         // ��ȡ���������Ϣ
void request();         // ������Դ
bool safe_check();      // ��ȫ�Լ��
void menu();            // �˵���
void display();         // չʾ��Դ����״��

int main()
{
	while (true) {
		printf("--------------------���м��㷨--------------------\n\n");
		get_res();                  // ��ȡ��Դ�����Ϣ
		get_pro();                  // ��ȡ���������Ϣ
		display();
		printf("\n\n--------------------���T0ʱ�̵�ϵͳ��ȫ��--------------------\n\n");
		bool safe = safe_check();   // ���T0ʱ�̵�ϵͳ��ȫ��
		bool over = false;          //�������
		while (!over) {
			menu();
			int ope;                // ѡ�����
			printf("������Ҫִ�еĲ�����");
			scanf("%d", &ope);
			if (!safe) printf("\nϵͳ���ڲ���ȫ״̬��\n");
			switch (ope) {
			case 1:
				request();      // ������Դ
				break;
			case 2:
				display();      // ���������㷨
				break;
			case 3:
				over = true;    // չʾ��ǰ��Դ����״��
				break;
			default:
				printf("\n***������Ч***\n");
				break;
			}
		}
		printf("--------------------�����㷨����------------------\n\n\n");
	}
	return 0;
}

void get_res() {
	printf("\n��������Դ�������� ");
	scanf("%d", &res_num);
	for (int i = 1; i <= res_num; i++) {
		printf("\n�������%3d ����Դ�������� ", i);
		scanf("%d", &Available[i]);
	}
	printf("------------------------------------------------\n\n");
}

void get_pro() {
	printf("\n\n��������̵ĸ����� ");
	scanf("%d", &pro_num);
	for (int i = 1; i <= pro_num; i++) {
		for (int j = 1; j <= res_num; j++) {
			printf("\n�������%3d ��������Ҫ��%3d ����Դ�ĸ����� ", i, j);
			scanf("%d", &Max[i][j]);
		}
		for (int j = 1; j <= res_num; j++) {
			printf("\n�������%3d �������Ѿ������%3d ����Դ�ĸ����� ", i, j);
			scanf("%d", &Allocation[i][j]);
			Need[i][j] = Max[i][j] - Allocation[i][j];
			Available[j] -= Allocation[i][j];
		}
		printf("\n");
	}

	printf("------------------------------------------------\n\n");
}

void request() {
	printf("\n\n������������Դ�Ľ��̣���һ������ΪP<0>����");
	scanf("%d", &re_pro_num);
	printf("\n������ ����%3d ��%3d ����Դ������������Դ֮���ÿո���з�������:  ", re_pro_num, res_num);

	bool flag1 = false;     // ����������Դ�����Ƿ񳬹�������Ҫ����Դ����
	bool flag2 = false;     // ����������Դ�����Ƿ񳬹��˵�ǰ�������õ���Դ����

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
			printf("\n�������Դ��������������Ҫ����Դ�����͵�ǰ�������õ���Դ����!\n\n");
		}
		else if (flag1) {
			printf("\n�������Դ��������������Ҫ����Դ����!\n\n");
		}
		else {
			printf("\n�������Դ���������˵�ǰ�������õ���Դ����!    �ȴ�\n\n");
		}
	}
	else {

		for (int i = 1; i <= res_num; i++) {
			//����Դ�����ȥ
			Available[i] -= requset_res[i];
			//���½�����Դ����״�� ������ All �� Need
			Allocation[re_pro_num][i] += requset_res[i];
			Need[re_pro_num][i] -= requset_res[i];
		}

		display();          //չʾ��ǰ��Դ����״��

							//���ϵͳ��ȫ��
		if (safe_check()) {
			//ϵͳ��ȫ��������Դ������
			//            display();      //չʾ����״��
		}
		else {
			//ϵͳ����ȫ���ָ�������Դǰ��������Դ����״̬

			printf("\nϵͳ����ȫ��������Դ��������� P<%d> ������ȴ�\n", re_pro_num - 1);
			for (int i = 1; i <= res_num; i++) {
				//��ԭ����
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

//��ȫ�Լ�⺯��
bool safe_check() {

	// Ч�� O(n^2)
	safe_count = 1;
	printf("\n--------------------��ȫ�Լ��--------------------\n\n");
	memset(Finish, false, sizeof Finish);

	for (int i = 1; i <= res_num; i++) {
		Work[1][i] = Available[i];
	}

	for (int i = 1; i <= pro_num; i++) {
		bool flag = false;              // ����Ƿ񻹴��� Finish[i] = false && Need[i][j]<=Work[j]
		for (int j = 1; j <= pro_num; j++) {
			if (Finish[j]) continue;
			bool flag2 = true;          // ��Ǹý����Ƿ����� Finish[i] = false && Need[i][j]<=Work[j]
			for (int k = 1; k <= res_num; k++) {
				if (Need[j][k] > Work[safe_count][k])
					flag2 = false;
			}
			if (flag2) {
				//���ý�������������������밲ȫ���У�������Work����
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
			printf("\n--------------------ϵͳ����ȫ--------------------\n\n");
			return false;
		}
	}
	show();
	printf("\n--------------------ϵͳ��ȫ--------------------\n\n");
	printf("���ڰ�ȫ���У�{");
	for (int i = 1; i <= pro_num; i++) {
		printf("P<%d>%c", safe_list[i], i == pro_num ? '}' : ',');
	}
	printf("\n");
	return true;
}

void menu() {
	printf("\n    1:������Դ   2:��ʾ��ǰ��Դ����״��   3:�����������м��㷨\n\n");
}


void display() {
	printf("\n-------------------------��Դ�������-----------------------\n\n");
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
�������� 3��Դ 5����
3 10 5 7 5 7 5 3 0 1 0 3 2 2 2 0 0 9 0 2 3 0 2 2 2 2 2 1 1 4 3 3 0 0 2
*/