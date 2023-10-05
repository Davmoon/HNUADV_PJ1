#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"
#include <stdio.h>

#define DIR_UP		0
#define DIR_DOWN	1
#define DIR_LEFT	2
#define DIR_RIGHT	3

void m_init(void);
bool cant_seek_behind(int, int); //뒤에 존재하는 경우 확인 함수. 움직일 경우 자신 기준으로 비교하면 될 듯?
void yh_print(int, int, int, bool); //영희 생성 함수 x, y, 영희 블록 수, 뒤돌아봄 O(true), X(false)
void move_m_random(int, int); // 백분율 부분 https://coding-factory.tistory.com/667 참조

int px[PLAYER_MAX], py[PLAYER_MAX], period[PLAYER_MAX];  // 각 플레이어 위치, 이동 주기

void m_init(void) {
	map_init(11, 35);
	yh_print(4, 1, 3, true);
	//player[2] = false; player[5] = false; //죽은 플레이어 테스트 코드

	int x, y;
	// 끝 자리에 살아남은 플레이어가 랜덤하게 배치되는 코드
	for (int i = 0; i < n_player; i++) {
		if (player[i] == true) {
			do {
				x = randint(1, 9); //x와 y의 범위를 잘 못 정해주면 무한루프로 출력오류 발생..
				y = 33;
			} while (!placable(x, y));
			px[i] = x;
			py[i] = y;
			period[i] = randint(100, 500);

			// (0 .. n_player-1) 왜 0을 붙여야 하는지 모르겠음; 아마 아스키코드 출력형식 때문일 듯?
			back_buf[px[i]][py[i]] = '0' + i;
		}
	}

	tick = 0; // 초기화
}

void yh_print(int x, int y, int num, bool look) {
	char icon;

	if (!look) icon = '@';
	else icon = '*';

	for (int i = 0; i < num; i++) {
		back_buf[x + i][y] = icon;
	}
}

// 만들 예정
bool cant_seek_behind(int row, int col) {
	if (row < 0 || row >= N_ROW ||
		col < 0 || col >= N_COL ||
		back_buf[row][col] != ' ') {
		return false;
	}
	return true;
}

void move_m_random(int pnum, int dir) {
	int nx, ny;
	int percent[10] = { 0,0,0,0,0,0,0,1,2,3 };

	static int dx[4] = { -1, 1, 0, 0 };
	static int dy[4] = { 0, 0, -1, 1 };

	do {
		switch (percent[randint(0, 9)]) {
			case 0:
				nx = px[pnum] + dx[2];
				ny = py[pnum] + dy[2];
				break; //왼쪽으로 
			case 1:
				nx = px[pnum] + dx[0];
				ny = py[pnum] + dy[0];
				break;
			case 2:
				nx = px[pnum] + dx[1];
				ny = py[pnum] + dy[1];
				break;
			case 3:
				nx = px[pnum];
				ny = py[pnum];
				break;
		}
		//printf("%d",percent[randint(0, 9)]);
	} while (!placable(nx, ny));

	move_tail(pnum, nx, ny);
}

void mugunghwa(void) {
	m_init();
	system("cls");
	display();
	//dialog("\"무궁화 꽃이 피었습니다\"");

	while (1) {
		key_t key = get_key();
		if (key == K_QUIT) {
			break;
		}
		else if (key != K_UNDEFINED) {

			move_manual(key);
		}

		for (int i = 1; i < n_player; i++) {
			if (player[i] == true && tick % period[i]) {
				move_m_random(i, 1);
			}
		}
		
		display();
		Sleep(1000);
		tick += 1000;
	}
}