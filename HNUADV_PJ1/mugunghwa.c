#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"
#include <stdio.h>

#define DIR_UP		0
#define DIR_DOWN	1
#define DIR_LEFT	2
#define DIR_RIGHT	3

void m_init(void);
void yh_print(int, int, int, bool); //���� ���� �Լ� x, y, ���� ��� ��, �ڵ��ƺ� O(true), X(false)

int px[PLAYER_MAX], py[PLAYER_MAX], period[PLAYER_MAX];  // �� �÷��̾� ��ġ, �̵� �ֱ�

void m_init(void) {
	map_init(11, 35); //����ִ� �÷��̾� 
	yh_print(4, 1, 3, true);
	player[2] = false; player[5] = false; //�׽�Ʈ �ڵ�

	int x, y;
	for (int i = 0; i < n_player; i++) {
		if (player[i] == true) {
			do {
				x = randint(1, 9); //x�� y�� ������ �� �� �����ָ� ���ѷ����� ��¿��� �߻�..
				y = 33;
			} while (!placable(x, y));
			px[i] = x;
			py[i] = y;

			// (0 .. n_player-1) �� 0�� �ٿ��� �ϴ��� �𸣰���; �Ƹ� ������� ������ ��?
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
	dialog("\"����ȭ ���� �Ǿ����ϴ�\"");

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