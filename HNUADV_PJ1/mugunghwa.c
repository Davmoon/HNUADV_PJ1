#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"
#include <stdio.h>

#define DIR_UP		0
#define DIR_DOWN	1
#define DIR_LEFT	2
#define DIR_RIGHT	3

void m_init(void);
bool cant_seek_behind(int, int); //�ڿ� �����ϴ� ��� Ȯ�� �Լ�. ������ ��� �ڽ� �������� ���ϸ� �� ��?
void yh_print(int, int, int, bool); //���� ���� �Լ� x, y, ���� ��� ��, �ڵ��ƺ� O(true), X(false)
void move_m_random(int, int); // ����� �κ� https://coding-factory.tistory.com/667 ����

int px[PLAYER_MAX], py[PLAYER_MAX], period[PLAYER_MAX];  // �� �÷��̾� ��ġ, �̵� �ֱ�

void m_init(void) {
	map_init(11, 35);
	yh_print(4, 1, 3, true);
	//player[2] = false; player[5] = false; //���� �÷��̾� �׽�Ʈ �ڵ�

	int x, y;
	// �� �ڸ��� ��Ƴ��� �÷��̾ �����ϰ� ��ġ�Ǵ� �ڵ�
	for (int i = 0; i < n_player; i++) {
		if (player[i] == true) {
			do {
				x = randint(1, 9); //x�� y�� ������ �� �� �����ָ� ���ѷ����� ��¿��� �߻�..
				y = 33;
			} while (!placable(x, y));
			px[i] = x;
			py[i] = y;
			period[i] = randint(100, 500);

			// (0 .. n_player-1) �� 0�� �ٿ��� �ϴ��� �𸣰���; �Ƹ� �ƽ�Ű�ڵ� ������� ������ ��?
			back_buf[px[i]][py[i]] = '0' + i;
		}
	}

	tick = 0; // �ʱ�ȭ
}

void yh_print(int x, int y, int num, bool look) {
	char icon;

	if (!look) icon = '@';
	else icon = '*';

	for (int i = 0; i < num; i++) {
		back_buf[x + i][y] = icon;
	}
}

// ���� ����
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
				break; //�������� 
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
	//dialog("\"����ȭ ���� �Ǿ����ϴ�\"");

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