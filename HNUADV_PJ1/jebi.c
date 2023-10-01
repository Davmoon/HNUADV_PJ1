#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"
#include <stdio.h>

#define DIR_UP		0
#define DIR_DOWN	1
#define DIR_LEFT	2
#define DIR_RIGHT	3

void jebi_init(void);
void jebi(void);
void move_manual1(key_t key);

int px[PLAYER_MAX], py[PLAYER_MAX];

void jebi_init(void) {
	map_init(9, 15);


}

void move_manual1(key_t key) {
	// 각 방향으로 움직일 때 x, y값 delta
	static int dx[4] = { -1, 1, 0, 0 };
	static int dy[4] = { 0, 0, -1, 1 };

	int dir;  // 움직일 방향(0~3)
	switch (key) {
	case K_LEFT: dir = DIR_LEFT; break;
	case K_RIGHT: dir = DIR_RIGHT; break;
	default: return;
	}

	// 움직여서 놓일 자리
	int nx, ny;
	nx = px[0] + dx[dir];
	ny = py[0] + dy[dir];
	if (!placable(nx, ny)) {
		return;
	}

	move_tail(0, nx, ny);
}

void jebi(void) {
	//jebi_init();
	system("cls");
	display();
	
	while (1) {
		key_t key = get_key();
		if (key == K_QUIT) {
			break;
		}
		else if (key != K_UNDEFINED) {

			move_manual1(key);
		}

	}
}