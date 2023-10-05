#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"
#include <stdio.h>

#define DIR_UP		0
#define DIR_DOWN	1
#define DIR_LEFT	2
#define DIR_RIGHT	3
#define 

void m_init(void);
void yh_print(int, int, int, bool); //영희 생성 함수 x, y, 영희 블록 수, 뒤돌아봄 O(true), X(false)

int px[PLAYER_MAX], py[PLAYER_MAX], period[PLAYER_MAX];  // 각 플레이어 위치, 이동 주기

void m_init(void) {
	map_init(11, 35); //살아있는 플레이어 
	yh_print(4, 1, 3, true);

	for (int i = 0; i < n_player; i++) {
		if (!player[i]) {
			px[i] = 1;
			py[i] = ;
			period[i] = randint(100, 500);

			back_buf[px[i]][py[i]] = '0' + i;  // (0 .. n_player-1)
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
	system("cls");
	m_init();
	display();

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