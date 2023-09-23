#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "jjuggumi.h"
#include "canvas.h"

#define DIALOG_DURATION_SEC		4

void draw(void);
void print_status(void);

// (zero-base) row��, col���� Ŀ�� �̵�
void gotoxy(int row, int col) {
	COORD pos = { col,row };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// row��, col���� ch ���
void printxy(char ch, int row, int col) {
	gotoxy(row, col);
	printf("%c", ch);
}

void map_init(int n_row, int n_col) {
	// �� ���۸��� ������ ���� #�� �ѷ��� ä��
	for (int i = 0; i < ROW_MAX; i++) {
		for (int j = 0; j < COL_MAX; j++) {
			back_buf[i][j] = front_buf[i][j] = ' ';
		}
	}

	N_ROW = n_row;
	N_COL = n_col;
	for (int i = 0; i < N_ROW; i++) {
		// ���Թ� �̷��� �� �� �ִµ� �Ϻη� �� ����������
		back_buf[i][0] = back_buf[i][N_COL - 1] = '#'; //ó���� �� �κ� # �߰�

		for (int j = 1; j < N_COL - 1; j++) {
			back_buf[i][j] = (i == 0 || i == N_ROW - 1) ? '#' : ' ';// ���� ���κп��� ó�� �� ä������ �࿡���� �յ� ���پ��� �� ä��� �ڵ�
		}
	}
}

// back_buf[row][col]�� �̵��� �� �ִ� �ڸ����� Ȯ���ϴ� �Լ�
bool placable(int row, int col) {
	if (row < 0 || row >= N_ROW ||
		col < 0 || col >= N_COL ||
		back_buf[row][col] != ' ') {
		return false;
	}
	return true;
}

// ��ܿ� ����, �ϴܿ��� ���� ���¸� ���
void display(void) {
	draw();
	gotoxy(N_ROW + 4, 0);  // �߰��� ǥ���� ������ ������ �ʰ� ����â ������ �� ������ ���
	print_status();
}


void draw(void) {
	for (int row = 0; row < N_ROW; row++) {
		for (int col = 0; col < N_COL; col++) {
			if (front_buf[row][col] != back_buf[row][col]) {
				front_buf[row][col] = back_buf[row][col];
				printxy(front_buf[row][col], row, col);
			}
		}
	}
}

void print_status(void) {
	printf("no. of players left: %d\n", n_alive);
	for (int p = 0; p < n_player; p++) {
		printf("player %2d: %5s\n", p, player[p] ? "alive" : "DEAD");
	}
}

void dialog(char message[]) {
	int sec = DIALOG_DURATION_SEC + 1;
	char save_buf[ROW_MAX][COL_MAX];

	//���̺� ���� ����, ���ο� �κ� ���� �����ϱ� ����.
	for (int i = 0; i < ROW_MAX; i++) {
		for (int j = 0; j < COL_MAX; j++) {
			save_buf[i][j] = front_buf[i][j];//���� ������� ���۸� ����.
		}
	}
	//for (int i = 3; i < ROW_MAX - 3; i++) {
	//	front_buf[i][3] = front_buf[i][COL_MAX - 3] = '*';
	//	for (int j = 3; j < COL_MAX - 3; j++) {
	//		front_buf[i][j] = (i = 3 || i + ROW_MAX - 3) ? '*': '+';
	//	}
	//}
	
	for (int i = 1; i < N_ROW; i++) {
		back_buf[i][1] = back_buf[i][N_COL - 1] = '*';

		for (int j = 1; j < N_COL - 1; j++) {
			/*back_buf[i][j+1] = (i == 1 || i == N_ROW - 1) ? '*' : back_buf[i][j+1];*/
		}
	}
	display();
	while (sec != 0) {
		Sleep(1000);
		sec--;
	}

	for (int i = 0; i < ROW_MAX; i++) {
		for (int j = 0; j < COL_MAX; j++) {
			back_buf[i][j] = save_buf[i][j];
		}
	}
	display();
}