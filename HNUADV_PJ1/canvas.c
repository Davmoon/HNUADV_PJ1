#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "jjuggumi.h"
#include "canvas.h"

#define DIALOG_DURATION_SEC		4

void draw(void);
void print_status(void);

// (zero-base) row행, col열로 커서 이동
void gotoxy(int row, int col) {
	COORD pos = { col,row };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// row행, col열에 ch 출력
void printxy(char ch, int row, int col) {
	gotoxy(row, col);
	printf("%c", ch);
}

void map_init(int n_row, int n_col) {
	// 두 버퍼를를 완전히 비우고 #을 둘레로 채움
	for (int i = 0; i < ROW_MAX; i++) {
		for (int j = 0; j < COL_MAX; j++) {
			back_buf[i][j] = front_buf[i][j] = ' ';
		}
	}

	N_ROW = n_row;
	N_COL = n_col;
	for (int i = 0; i < N_ROW; i++) {
		// 대입문 이렇게 쓸 수 있는데 일부러 안 가르쳐줬음
		back_buf[i][0] = back_buf[i][N_COL - 1] = '#'; //처음과 끝 부분 # 추가

		for (int j = 1; j < N_COL - 1; j++) {
			back_buf[i][j] = (i == 0 || i == N_ROW - 1) ? '#' : ' ';// 앞의 열부분에서 처음 끝 채웠으니 행에서는 앞뒤 한줄씩은 다 채우기 코드
		}
	}
}

// back_buf[row][col]이 이동할 수 있는 자리인지 확인하는 함수
bool placable(int row, int col) {
	if (row < 0 || row >= N_ROW ||
		col < 0 || col >= N_COL ||
		back_buf[row][col] != ' ') {
		return false;
	}
	return true;
}

// 상단에 맵을, 하단에는 현재 상태를 출력
void display(void) {
	draw();
	gotoxy(N_ROW + 4, 0);  // 추가로 표시할 정보가 있으면 맵과 상태창 사이의 빈 공간에 출력
	print_status();
}

// 맵을 넘어가는 dialog 사이즈 구현을 위해 N_ROW, N_COL에서 MAX로 수정, 기본코드 수정 불가하면 추가 함수 필요.
void draw(void) {
	for (int row = 0; row < ROW_MAX; row++) {
		for (int col = 0; col < COL_MAX; col++) {
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
	int sec = DIALOG_DURATION_SEC + 1; // 0으로 끝나므로 1로 끝나도록 + 1
	char save_buf[ROW_MAX][COL_MAX]; //세이브 버퍼 생성, 새로운 부분 띄우고 복구하기 위함

	for (int i = 0; i < ROW_MAX; i++) {
		for (int j = 0; j < COL_MAX; j++) {
			save_buf[i][j] = back_buf[i][j];//현재 출력중인 버퍼를 저장
		}
	}

	int Rmid = N_ROW / 3; //최소한으로 dialog 중앙정렬 위해 맵 세로 3등분해서 처음 + 1
	
	// '*'로 이루어진 dialog 테두리 출력, 상상력 부족으로 한땀한땀 해봄
	for (int i = Rmid; i < Rmid + 5; i++) {
		back_buf[i][2] = back_buf[i][COL_MAX - 45] = '*';
		for (int j = 3; j < COL_MAX - 45; j++) {
			back_buf[i][j] = (i == Rmid || i == Rmid + 4) ? '*' : ' ';
		}
	}
	display();

	int y = Rmid + 2; // message 출력 아래쪽으로 몇칸인지 지정
	int sec_x = 4; // sec초 출력 옆으로 몇칸인지 지정
	int mes_x = 8; // message 출력 옆으로 몇칸인지 지정

	do {

		gotoxy(y, sec_x); // sec초 출력
		printf("%d", sec);

		gotoxy(y, mes_x); // message 출력
		printf("%s", message);

		Sleep(1000); // 1초 지연
		sec--; //초 경과 카운트
		
		if (sec == 0) {
			for (int j = sec_x; j < COL_MAX - 46; j++) {
				gotoxy(y, j); // message 출력부분 빈칸으로 초기화
				printf(" ");
			}
		}
	
	
	} while (sec != 0);

	for (int i = 0; i < ROW_MAX; i++) {
		for (int j = 0; j < COL_MAX; j++) {
			back_buf[i][j] = save_buf[i][j];
		}
	}

	display();

	Sleep(2000); // 정지하는 상태인지 파악하기 위한 TEST CODE
}