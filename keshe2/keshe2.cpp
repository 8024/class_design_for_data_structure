#include<stdio.h>			//������ݽṹ�γ����
#include<malloc.h>
#include<stdlib.h>
#include<string.h>
#define MAX_NUM 10000
#define TURE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -2

#define RED 1
#define GREEN 2
#define BLUE 3
#define PURPLE 4

typedef int status;
typedef char ElemType;

typedef struct Infor_provice {			//����ʡ�ݵĻ�����Ϣ
	char name_provice[20];				//ʡ�ݵ�����
	char name_captial[20];				//ʡ�������
	int population;						//ʡ�ݵ��˿�
}Infor_provice;

typedef struct Pro_contact {			//����ʡ�ݵ��ڽӹ�ϵ
	Infor_provice infor_provice[34];
	int next_to[34][34];
}Pro_contact;

void InGraph(Pro_contact *pro_contact, Pro_contact *use_contact);		//���ڽӱ����ݹ��ɵ�ͼ
void Information(Pro_contact *pro_contact);			//����ʡ�ݵĻ�����Ϣ����ṹ��
status Get_infor_pro(Infor_provice infor_provice[34]);			//��ʾ��ʡ�ݵĻ�����Ϣ
status Shortestroad(Pro_contact *pro_contact, int *lastplace);			//�ó����·��������ʾ����
status Paintcolor(Pro_contact use_contact, int *p);			//Ϊ��ͼ��ɫ
status Showcolor(Pro_contact pro_contact, int *p);			//����ɫ�����ֵ���ʽ���ֳ���

int main(void) {
	int op = 1;
	Pro_contact pro_contact, use_contact;
	int paint[34];
	int *p = paint;
	int lastplace[34];

	InGraph(&pro_contact, &use_contact);
	Information(&pro_contact);

	while (op) {
		system("cls");
		printf("\n\n");
		printf("###################### �л����񹲺͹���ͼ ####################\n\n");
		printf("        1.��Ѱʡ����Ϣ                     2.ͨ�����繹��·��\n\n");
		printf("        3.����ʡ�ݲ�ͬ��ɫ��Ⱦɫ           0.�˳�\n\n");
		printf("##############################################################\n\n");
		printf("   ��ѡ����Ĳ���[0-3]:\n\n");
		printf("##############################################################\n");
		scanf_s("%d", &op);
		printf("##############################################################\n");
		switch (op) {
		case 1: {
			Get_infor_pro(pro_contact.infor_provice);
			system("pause");
			break;
		}
		case 2: {
			Shortestroad(&pro_contact, lastplace);
			system("pause");
			break;
		}
		case 3: {
			Paintcolor(use_contact, p);
			Showcolor(pro_contact, p);
			system("pause");
			break;
		}
		case 0:printf("ллʹ�ô�ϵͳ����ӭ����´�ʹ�ã��ټ���\n");
			printf("##############################################################\n");
			system("pause");
			break;
		}
	}
	return 0;
}
void InGraph(Pro_contact *pro_contact, Pro_contact *use_contact) {
	FILE *open_D = fopen("Distance.txt", "r"), *open_I = fopen("NextDis.txt", "r");
	int row = 0, column = 0;
	while (row < 34) {
		column = 0;
		while (column < 34 && fscanf(open_D, "%d", &pro_contact->next_to[row][column]) && fscanf(open_I, "%d", &use_contact->next_to[row][column]))column++;
		row++;
	}
	fclose(open_D);
	fclose(open_I);
}
void Information(Pro_contact *pro_contact) {
	FILE *fin = fopen("Information.txt", "r");
	int count = 0;
	if (fin == NULL) {
		exit(-1);
	}
	while (fscanf(fin, "%s %s %d", pro_contact->infor_provice[count].name_captial, pro_contact->infor_provice[count].name_provice, &pro_contact->infor_provice[count].population) != EOF&&fin != NULL) {
		count++;
	}
	fclose(fin);
}
status Get_infor_pro(Infor_provice infor_provice[34]) {	//���������ʡ�ݵĻ�����Ϣ
	char search[20];
	char c;
	int judge = 0;
	printf("��������Ҫ������Ϣ��ʡ�ݣ�\n");
	printf("##############################################################\n");
	getchar();
	while ((c = getchar()) != '\n') {
		search[judge] = c;
		judge++;
	}
	printf("##############################################################\n");
	for (int count = 0; count < 34; count++) {
		int yes = 1;
		for (int i = 0; i < judge; i++) {
			if (yes) {
				if (infor_provice[count].name_captial[i] == search[i]) {
					yes = 1;
				}
				else yes = 0;
			}
		}
		if (yes) {
			printf("ʡ�ݣ�%s		ʡ�᣺%s		�˿ڣ�%d		\n", infor_provice[count].name_captial, infor_provice[count].name_provice, infor_provice[count].population);
			printf("##############################################################\n");
			return OK;
		}
	}
	printf("���������Ϣ�������������룡\n");
	printf("##############################################################\n");
	return OK;
}
status Shortestroad(Pro_contact *pro_contact, int *lastplace) {		//������С�������㷨��Ѱ��Ȩֵ��С
	int i = 0;
	int shortest[34], cost[34], place[34];
	int minplace;
	long road = 0;
	place[0] = 1;
	int put_judge = 0;
	printf("##############################################################\n");
	printf("ͨ�����繹������:\n%s",pro_contact->infor_provice[0].name_captial);
	while (i < 34) {
		shortest[i] = 0;
		cost[i] = pro_contact->next_to[0][i];
		if (i>0)place[i] = 0;
		lastplace[i] = -1;
		i++;
	}
	for (i = 0; i < 33; i++) {
		int j = 0; minplace = MAX_NUM;
		for (int k = 1; k < 34; k++) {
			if (!place[k] && cost[k] < minplace) {
				minplace = cost[k];
				j = k;
			}
		}
		lastplace[j] = cost[j]; place[j] = 1;
		for (int k = 0; k < 34; k++) {
			if (!place[k] && (pro_contact->next_to[k][j] < cost[k])) {
				cost[k] = pro_contact->next_to[k][j];
				shortest[k] = j;
			}
		}
		printf("����%s", pro_contact->infor_provice[j].name_captial);
		if (!((put_judge + 2) % 5))putchar('\n');
		put_judge++;
	}		
	printf("\n##############################################################\n");
	return OK;
}
status Paintcolor(Pro_contact use_contact, int *p) {
	int color, provice;
	int judge;
	p[0] = RED; color = RED, provice = RED;
	while (provice < 34) {
		while (color < 5) {
			judge = 0;
			if (provice > 33)break;
			while (judge < provice) {
				if (use_contact.next_to[provice][judge] && p[judge] == color)break;		//�㷨�ص� ������Ƿ���ɫ
				judge++;
			}
			if (judge < provice) {
				color++;
			}
			else {
				p[provice] = color;
				color = RED;
				provice++;
			}
		}
		if (color > 4) {			//�����ɫ����4��˵�������ϱ��⣬���л�����ɫ
			provice--;
			color = p[provice] + 1;
		}
	}
	return OK;
}
status Showcolor(Pro_contact pro_contact, int *p) {
	int count = 0;
	int count_line = 1;
	printf("ȫ������������ɫ��ɫ���ֱ�Ϊ��ɫ����ɫ����ɫ����ɫ��������ɫ���£�\n");
	printf("##############################################################\n");
	while (count < 34) {
		switch (p[count]) {
		case 1:printf("%sʡΪ��ɫ\t\t", pro_contact.infor_provice[count].name_captial); break;
		case 2:printf("%sʡΪ��ɫ\t\t", pro_contact.infor_provice[count].name_captial); break;
		case 3:printf("%sʡΪ��ɫ\t\t", pro_contact.infor_provice[count].name_captial); break;
		case 4:printf("%sʡΪ��ɫ\t\t", pro_contact.infor_provice[count].name_captial); break;
		}
		if (!(count_line % 3))printf("\n");
		count++;
		count_line++;
	}
	printf("\n##############################################################\n");
	return OK;
}
