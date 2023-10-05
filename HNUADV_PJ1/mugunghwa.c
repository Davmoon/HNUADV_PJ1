#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"
#include <stdio.h>

#define DIR_UP		0
#define DIR_DOWN	1
#define DIR_LEFT	2
#define DIR_RIGHT	3

void m_init(void);
void yh_print(int, int, int, bool); //영희 생성 함수 x, y, 영희 블록 수, 뒤돌아봄 O(true), X(false)

int px[PLAYER_MAX], py[PLAYER_MAX], period[PLAYER_MAX];  // 각 플레이어 위치, 이동 주기

void m_init(void) {
	map_init(11, 35); //살아있는 플레이어 
	yh_print(4, 1, 3, true);
	player[2] = false; player[5] = false; //테스트 코드

	int x, y;
	for (int i = 0; i < n_player; i++) {
		if (player[i] == true) {
			do {
				x = randint(1, 9); //x와 y의 범위를 잘 못 정해주면 무한루프로 출력오류 발생..
				y = 33;
			} while (!placable(x, y));
			px[i] = x;
			py[i] = y;

			// (0 .. n_player-1) 왜 0을 붙여야 하는지 모르겠음; 아마 출력형식 때문일 듯?
			back_buf[px[i]][py[i]] = '0' + i;
		}
	}


}

void yh_print(int x, int y, int num, bool look) {
	char icon;

	if (!look) icon = '@';
	else icon = '*';

	for (int i = 0; i < num; i++) {
		back_buf[x + i][y] = icon;
	}
}

void mugunghwa(void) {
	m_init();
	system("cls");
	display();
	dialog("\"무궁화 꽃이 피었습니다\"");

	while (1) {
		key_t key = get_key();
		if (key == K_QUIT) {
			break;
		}
		else if (key != K_UNDEFINED) {

			//move_manual(key);
		}
	}
}