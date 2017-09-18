#include<stdio.h>			//大二数据结构课程设计
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

typedef struct Infor_provice {			//各个省份的基本信息
	char name_provice[20];				//省份的名称
	char name_captial[20];				//省会的名称
	int population;						//省份的人口
}Infor_provice;

typedef struct Pro_contact {			//各个省份的邻接关系
	Infor_provice infor_provice[34];
	int next_to[34][34];
}Pro_contact;

void InGraph(Pro_contact *pro_contact, Pro_contact *use_contact);		//将邻接表数据构成地图
void Information(Pro_contact *pro_contact);			//将各省份的基本信息读入结构中
status Get_infor_pro(Infor_provice infor_provice[34]);			//显示各省份的基本信息
status Shortestroad(Pro_contact *pro_contact, int *lastplace);			//得出最短路径，并显示出来
status Paintcolor(Pro_contact use_contact, int *p);			//为地图着色
status Showcolor(Pro_contact pro_contact, int *p);			//将颜色用文字的形式变现出来

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
		printf("###################### 中华人民共和国地图 ####################\n\n");
		printf("        1.搜寻省份信息                     2.通信网络构建路径\n\n");
		printf("        3.相邻省份不同颜色的染色           0.退出\n\n");
		printf("##############################################################\n\n");
		printf("   请选择你的操作[0-3]:\n\n");
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
		case 0:printf("谢谢使用此系统，欢迎你的下次使用，再见！\n");
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
status Get_infor_pro(Infor_provice infor_provice[34]) {	//输出所搜索省份的基本信息
	char search[20];
	char c;
	int judge = 0;
	printf("请输入想要搜索信息的省份：\n");
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
			printf("省份：%s		省会：%s		人口：%d		\n", infor_provice[count].name_captial, infor_provice[count].name_provice, infor_provice[count].population);
			printf("##############################################################\n");
			return OK;
		}
	}
	printf("您输入的信息有误，请重新输入！\n");
	printf("##############################################################\n");
	return OK;
}
status Shortestroad(Pro_contact *pro_contact, int *lastplace) {		//运用最小生成树算法，寻求权值最小
	int i = 0;
	int shortest[34], cost[34], place[34];
	int minplace;
	long road = 0;
	place[0] = 1;
	int put_judge = 0;
	printf("##############################################################\n");
	printf("通信网络构建如下:\n%s",pro_contact->infor_provice[0].name_captial);
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
		printf("——%s", pro_contact->infor_provice[j].name_captial);
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
				if (use_contact.next_to[provice][judge] && p[judge] == color)break;		//算法重点 ，检测是否重色
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
		if (color > 4) {			//如果颜色大于4，说明不符合本意，进行回溯着色
			provice--;
			color = p[provice] + 1;
		}
	}
	return OK;
}
status Showcolor(Pro_contact pro_contact, int *p) {
	int count = 0;
	int count_line = 1;
	printf("全国采用四种颜色着色，分别为红色，绿色，蓝色，紫色。具体着色如下：\n");
	printf("##############################################################\n");
	while (count < 34) {
		switch (p[count]) {
		case 1:printf("%s省为红色\t\t", pro_contact.infor_provice[count].name_captial); break;
		case 2:printf("%s省为绿色\t\t", pro_contact.infor_provice[count].name_captial); break;
		case 3:printf("%s省为蓝色\t\t", pro_contact.infor_provice[count].name_captial); break;
		case 4:printf("%s省为紫色\t\t", pro_contact.infor_provice[count].name_captial); break;
		}
		if (!(count_line % 3))printf("\n");
		count++;
		count_line++;
	}
	printf("\n##############################################################\n");
	return OK;
}
