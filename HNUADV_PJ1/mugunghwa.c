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
void mv_m_random(int); // ����� �κ� https://coding-factory.tistory.com/667 ����
void pass_zone(void);
void yh_no_watch(int yh_period[]);
void catch_move(int);
void ten_mv();

int px[PLAYER_MAX], py[PLAYER_MAX], period[PLAYER_MAX]; // �� �÷��̾� ��ġ, �̵� �ֱ�, �н� ����
int len = 0; //'����ȭ���� �Ǿ����ϴ�' ��µ� ���� ����

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
			period[i] = randint(100, 200);

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

void mv_m_random(int pnum) {
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

	//9���� ��쿡 ���߾� yh 5���� ����
	for (int i = 0; i < n_player; i++) {
		if ((px[i] == 3 && py[i] == 1) ||
			(px[i] == 4 && py[i] == 2) ||
			(px[i] == 5 && py[i] == 2) ||
			(px[i] == 6 && py[i] == 2) ||
			(px[i] == 7 && py[i] == 1)) {

			back_buf[px[i]][py[i]] = ' ';
			pass[i] = true;
		}	
	}
}

void yh_no_watch(int yh_period[]) {
	char sent[] = "����ȭ�����Ǿ����ϴ�";
	int pm_time = 50; // �������ų� �������� ���� ���ϴ� �и�sec

	if (len <= 9) {
		yh_print(4, 1, 3, false); // ���� off
		gotoxy(N_ROW, len * 2); // ��� ��ҷ� �̵�

		// ��ġ�� �ڵ� ���߿� �����ϸ� ����
		if (len <= 4 && yh_period[2] % yh_period[0] == 0) {
			printf("%c%c", sent[len * 2], sent[len * 2 + 1]); //�ѱ��� 2bit, ���� %c%c ���
			len += 1;
			if (len == 4) {
				yh_period[2] = 0; //����ȭ ��� Ÿ�̸� �ʱ�ȭ
			}
		}
		else if (len >= 5 && yh_period[2] % yh_period[1] == 0) {
			printf("%c%c", sent[len * 2], sent[len * 2 + 1]); //�ѱ��� 2bit, ���� %c%c ���
			len += 1;

			if (len == 10) {
				yh_period[2] = 0; //����ȭ ��� Ÿ�̸� �ʱ�ȭ
				ten_mv(); // len 10�϶��� �ѹ��� �����(���̽�!!!!!)
			}
		}
	}
	else if (len == 10) {
		yh_print(4, 1, 3, true);
		yh_period[2] += 10; // ����ȭ ��� ���� ī��Ʈ �ؾ� �ϱ� ����

		// ���ð� ī���� �׽�Ʈ �ڵ�
		gotoxy(N_ROW + 1, 0);
		printf("%d �и��� ���", tick);

		if (yh_period[2] % 3000 == 0) {
			yh_print(4, 1, 3, true); //���� on
			len = 0; //����ȭ �� ������ ����, ���ο� ������ ���� ���� �ʱ�ȭ
			yh_period[2] = 0; //����ȭ 3�� ��� Ÿ�̸� �ʱ�ȭ

			gotoxy(N_ROW, 0); // ����ȭ ����� ������ ��� (�� ���� ����� �������� ����...)
			printf("                    "); // 2bit�̹Ƿ� 20ĭ

			// pm_time�� ���� ������ ���� �ʵ��� �ϱ� ����.
			if (yh_period[1] > pm_time) {
				yh_period[0] += pm_time;
				yh_period[1] -= pm_time;
			}
		}
	}
}

void ten_mv() {
	for (int i = 1; i < n_player; i++) {
		if (player[i] == true && pass[i] == false && randint(1, 9) == 9) {
			catch_move(i);
			mv_m_random(i);
			catch_move(i);
			//player[i] = false;
		}
	}
}

void catch_move(int a) {
	int al_chk_count = 0; //�÷���� ��� üũ�ؾ� ������ �� ����.
	for (int i = 0; i < n_player; i++) {

		
			// if���� ���� < ���� �ڱ��ڽ��� �ɷ��� (��ȣ ����)
			if (player[i] == true && pass[i] == false && py[i] < py[a] && px[i] == px[a]) {
				//�׽�Ʈ ��ǥ ���
				gotoxy(N_ROW + 3, 0);
				printf("pass ��ǥ��: %d %d | %d %d", px[i], py[i], px[a], px[a]);

				break;
			}
			else { al_chk_count++; }

			// ��� üũ�� �Ϸ��ߴµ��� �տ� �ƹ��������� ���̱�
			if (player[a] == true && al_chk_count == n_player) {
				//�׽�Ʈ ��ǥ ���
				gotoxy(N_ROW + 2, 0);
				printf("0�� kill ��ǥ��: %d %d | %d %d", px[i], py[i], px[a], px[a]);

				player[a] = false;
				back_buf[px[a]][py[a]] = ' ';
				n_alive--;
			}
	}
}

void mugunghwa(void) {
	m_init();
	system("cls");
	display();
	//dialog("\"����ȭ ���� �Ǿ����ϴ�\"");
	int yh_period[] = { 500, 500, 0 }; // ����ȭ �� t, �Ǿ����ϴ� t, ����ȭ ���� Ÿ�̸�(tick�� ������ ��������)
	
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

			if (pass[0] == false && player[0] == true) {
				if (len <= 9) {
					move_manual(key);
				}
				else if (len == 10) {
					// 2�� catch �Ͽ� 3�� ������ ���� ��, ���� �� ��� �⵵�� �ν�
					catch_move(0);
					move_manual(key);
					catch_move(0);
				}
			}
		}

		for (int i = 1; i < n_player; i++) {
			if (yh_period[2] % period[i] == 0 && player[i] == true && pass[i] == false) {
				if (len <= 9) {
					mv_m_random(i);
				}
			}
		}
		
		pass_zone();
		display();
		for (int i = 0; i < n_player; i++) {
			if (pass[i] == true) {
				gotoxy(N_ROW + 14, i*13);
				printf("|%d�н�(%d,%d)|", i, px[i], py[i]);
			}
			if (player[i] == false) {
				gotoxy(N_ROW + 15, i * 13);
				printf("|%d����(%d,%d)|", i, px[i], py[i]);
			}

		}
		Sleep(10);
		//tick += 10; // (�ý��� �ð�) ���⼱ �̻��
		yh_period[2] += 10; // ����ȭ ���� Ÿ�̸�
	}
}