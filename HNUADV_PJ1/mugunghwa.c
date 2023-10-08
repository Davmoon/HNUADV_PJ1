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
void move_m_random(int); // ����� �κ� https://coding-factory.tistory.com/667 ����
void pass_zone(void);
void yh_no_watch(int yh_period[]);
void catch_move();
void phase_dialog(char);

int px[PLAYER_MAX], py[PLAYER_MAX], period[PLAYER_MAX]; // �� �÷��̾� ��ġ, �̵� �ֱ�, �н� ����
int len = 0; //'����ȭ���� �Ǿ����ϴ�' ��µ� ���� ����
bool a = true;

void m_init(void) {
	map_init(11, 35);
	yh_print(4, 1, 3, true);

	int x, y;
	// �� �ڸ��� ��Ƴ��� �÷��̾ �����ϰ� ��ġ�Ǵ� �ڵ�
	for (int i = 0; i < n_player; i++) {
		if (player[i] == true) {
			do {
				x = randint(1, 9);
				y = 33;
			} while (!placable(x, y));
			px[i] = x;
			py[i] = y;
			//period[i] = randint(80, 150);

			back_buf[px[i]][py[i]] = '0' + i;
		}
	}

	tick = 0; // �ʱ�ȭ
}

void yh_print(int x, int y, int num, bool look) {
	char icon = '@'; //�÷��̾ ���� �ִ� ��Ȳ

	if (!look) icon = '*'; //�� �ڷ� ���� �ִ� ��Ȳ

	for (int i = 0; i < num; i++) {
		back_buf[x + i][y] = icon;
	}
}

void move_m_random(int pnum) {
	int nx, ny;
	int percent[10] = { 0,0,0,0,0,0,0,1,2,3 }; // 70%, 10%, 10%, 10% ����

	static int dx[4] = { -1, 1, 0, 0 };
	static int dy[4] = { 0, 0, -1, 1 };

	do {
		switch (percent[randint(0, 9)]) {
			case 0:
				nx = px[pnum] + dx[2]; ny = py[pnum] + dy[2];
				break; //��������
			case 1:
				nx = px[pnum] + dx[0]; ny = py[pnum] + dy[0];
				break; //��������
			case 2:
				nx = px[pnum] + dx[1]; ny = py[pnum] + dy[1];
				break; //�Ʒ�������
			case 3:
				nx = px[pnum]; ny = py[pnum];
				break; //���ڸ���
		}
	} while (!placable(nx, ny));

	move_tail(pnum, nx, ny);
}

void pass_zone(void) {

	for (int i = 0; i < n_player; i++) {
		if ((px[i] == 3 && py[i] == 1) ||
			(px[i] == 4 && py[i] == 2) ||
			(px[i] == 5 && py[i] == 2) ||
			(px[i] == 6 && py[i] == 2) ||
			(px[i] == 7 && py[i] == 1)){

			back_buf[px[i]][py[i]] = ' ';
			pass[i] = true;
		}
	}
}

void yh_no_watch(int yh_period[]) {
	char sent[] = "����ȭ�����Ǿ����ϴ�";
	int pm_time = 20; // �������ų� �������� ���� ���ϴ� �и�sec

	if (len <= 9) {
		yh_print(4, 1, 3, false); // ���� �ڵ��ƺ�
		gotoxy(N_ROW + 1, len * 2); // ��� ��ҷ� �̵�

		// ��ġ�� �ڵ� ���߿� �����ϸ� ����
		if (len <= 4 && yh_period[2] % yh_period[0] == 0) {
			printf("%c%c", sent[len * 2], sent[len * 2 + 1]); //�ѱ��� 2bit, ���� %c%c ���
			len += 1;
		}
		else if (len >= 5 && yh_period[2] % yh_period[1] == 0) {
			printf("%c%c", sent[len * 2], sent[len * 2 + 1]); //�ѱ��� 2bit, ���� %c%c ���
			len += 1;

			if (len == 10) {
				yh_period[2] = 0; //����ȭ ��� Ÿ�̸� �ʱ�ȭ
				catch_move(); // len 10�϶��� �ѹ��� �����(���̽�!!!!!)
			}
		}
	}
	else if (len == 10){

		yh_print(4, 1, 3, true);
		yh_period[2] += 10; // ����ȭ ��� ���� ī��Ʈ �ؾ� �ϱ� ����

		// ���ð� ī���� �׽�Ʈ �ڵ�
		//gotoxy(N_ROW + 2, 0);
		//printf("%d �и��� ���", yh_period[2]);

		if (yh_period[2] % 3000 == 0) {
			yh_print(4, 1, 3, true);
			len = 0;
			yh_period[2] = 0; //����ȭ 3�� ��� Ÿ�̸� �ʱ�ȭ
			a = true;

			gotoxy(N_ROW + 1, 0); // ����ȭ ����� ������ ��� (�� ���� ����� �������� ����...)
			printf("                    "); // 2bit�̹Ƿ� 20ĭ

			// pm_time�� ���� ������ ���� �ʵ��� �ϱ� ����.
			if (yh_period[1] > pm_time) {
				yh_period[0] += pm_time;
				yh_period[1] -= pm_time;
			}
		}		
	}
}

void catch_move() {

	//�÷��̾� 1���� 10%Ȯ���� �״� �ڵ� (while ���� ������ 10% * (3000 / 10)�� �õ��Ǿ� �Ұ�)
	for (int i = 1; i < 8; i++) {
		if (player[i] == true && randint(0, 9) == 9) {
			player[i] = false;
			back_buf[px[i]][py[i]] = ' ';
			n_alive--;
			pass[i] = true;
		}
	}
}

void phase_dialog(char passmember) {
	// pass[] �迭�� ���������ϴ�. ������ּ���.
}

void mugunghwa(void) {
	m_init();
	system("cls");
	display();
	//dialog("\"����ȭ ���� �Ǿ����ϴ�\"");

	int yh_period[] = { 300, 300, 0}; // ����ȭ �� t, �Ǿ����ϴ� t, ����ȭ ���� Ÿ�̸�(tick�� ������ ��������), ������ �н� üũ
	

	while (1) {

		if (n_alive == 1) {
			break;
		}

		yh_no_watch(yh_period);
		
		key_t key = get_key();
		if (key == K_QUIT) {
			break;
		}
		else if (key != K_UNDEFINED) {
			move_manual(key);
			if (len == 10 && player[0] == true && pass[0] == false) {
				player[0] = false;
				back_buf[px[0]][py[0]] = ' ';
				n_alive--;
			}
		}

		for (int i = 1; i < n_player; i++) {
			period[i] = randint(100, 200); //�Ź� �ٸ� ����ӵ�
			if (tick % period[i] == 0) {
				if (len <= 9) {
					move_m_random(i);
				}
			}
		}

		display();
		pass_zone();
		Sleep(10);
		tick += 10; // �ý��� �ð�
		yh_period[2] += 10; // ����ȭ ���� Ÿ�̸�
	}
}