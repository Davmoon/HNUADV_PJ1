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
bool cant_seek_behind(int, int); //�ڿ� �����ϴ� ��� Ȯ�� �Լ�. ������ ��� �ڽ� �������� ���ϸ� �� ��?
void yh_no_watch(int* sent_len, int yh_period[]);
void catch_move(int* sent_len, int, int yh_period[]);

int px[PLAYER_MAX], py[PLAYER_MAX], period[PLAYER_MAX]; // �� �÷��̾� ��ġ, �̵� �ֱ�, �н� ����
int n_pass = 0;// �ӽ� ��������

void m_init(void) {
	map_init(11, 35);
	yh_print(4, 1, 3, true);
	//player[2] = false; player[5] = false; //���� �÷��̾� test �ڵ�

	int x, y;
	// �� �ڸ��� ��Ƴ��� �÷��̾ �����ϰ� ��ġ�Ǵ� �ڵ�
	for (int i = 0; i < n_player; i++) {
		if (player[i] == true) {
			do {
				x = randint(1, 9); //x�� y�� ������ �� �� �����ָ� ���ѷ����� ��¿��� �߻�..
				y = 33; // 0���� �����̱� ����
			} while (!placable(x, y));
			px[i] = x;
			py[i] = y;
			//period[i] = randint(50, 100); //�����Բ� �� phase �� �ֱⰡ �޶����� �ϴ��� �����ؾ���.

			// (0 .. n_player-1) �� 0�� �ٿ��� �ϴ��� �𸣰���; �Ƹ� �ƽ�Ű�ڵ� ������� ������ ��?
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

// ���� ����
bool cant_seek_behind(int row, int col) {
	if (row < 0 || row >= N_ROW ||
		col < 0 || col >= N_COL ||
		back_buf[row][col] != ' ') {
		return false;
	}
	return true;
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

void yh_no_watch(int *sent_len, int yh_period[]) {
	char sent[] = "����ȭ�����Ǿ����ϴ�";
	int len = *sent_len; //�ʹ� �� ����
	int pm_time = 20; // �������ų� �������� ���� ���ϴ� �и�sec

	if (len <= 9) {
		yh_print(4, 1, 3, false); // ���� �ڵ��ƺ�
		gotoxy(N_ROW + 1, len * 2); // ��� ��ҷ� �̵�

		// ��ġ�� �ڵ� ���߿� �����ϸ� ����
		if (len < 5 &&  yh_period[2] % yh_period[0] == 0) {
			printf("%c%c", sent[len * 2], sent[len * 2 + 1]); //�ѱ��� 2bit, ���� %c%c ���
			*sent_len += 1;
		}
		else if (len >= 5 && yh_period[2] % yh_period[1] == 0) {
			printf("%c%c", sent[len * 2], sent[len * 2 + 1]); //�ѱ��� 2bit, ���� %c%c ���
			*sent_len += 1;

			if (*sent_len == 10) {
				yh_period[2] = 0; //����ȭ ��� Ÿ�̸� �ʱ�ȭ
			}
		}
	}
	else if (len == 10){

		yh_print(4, 1, 3, true);
		yh_period[2] += 10; // ����ȭ ��� ���� ī��Ʈ �ؾ� �ϱ� ����

		// ���ð� ī���� �׽�Ʈ �ڵ�
		gotoxy(N_ROW + 2, 0);
		printf("%d �и��� ���", yh_period[2]);

		if (yh_period[2] % 3000 == 0) {
			yh_print(4, 1, 3, true);
			*sent_len = 0;
			yh_period[2] = 0; //����ȭ 3�� ��� Ÿ�̸� �ʱ�ȭ
			yh_period[3]++;


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

void catch_move(int *sent_len, int i, int yh_period[]) {
	int len = *sent_len;
	int mv_user_hy_front[] = { 1,1,1,1,1,1,1,1,1,1 }; //10% Ȯ���� ������ Ż��

	// 3�� ���ð��� ���(len == 10) �Է� �����ų�(0) �����̸�(1~9) ����
	if (len == 10 && player[i] == true) {
		player[i] = false;
		back_buf[px[i]][py[i]] = ' ';
		n_alive--;
	}
}

void mugunghwa(void) {
	m_init();
	system("cls");
	display();
	//dialog("\"����ȭ ���� �Ǿ����ϴ�\"");

	int yh_period[] = { 200, 200, 0, 0 }; // ����ȭ �� t, �Ǿ����ϴ� t, ����ȭ ���� Ÿ�̸�(tick�� ������ ��������), ������ �н� üũ
	int sent_len = 0; // ��µ� char sent[] ���� �� ī��Ʈ

	while (1) {
		yh_no_watch(&sent_len, yh_period);
		
		key_t key = get_key();
		if (key == K_QUIT) {
			break;
		}
		else if (key != K_UNDEFINED) {
			move_manual(key);
			catch_move(&sent_len, 0, yh_period);
		}

		for (int i = 1; i < n_player; i++) {
			period[i] = randint(20, 50); // �ұ�Ģ���� �÷��̾� �ֱ� ����
			if (pass[i] == false && tick % period[i] == 0) {
				if (sent_len <= 9) {
					move_m_random(i);
				}
				else{
					//catch_move(&sent_len, i, yh_period);
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