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
void move_m_random(int); // 백분율 부분 https://coding-factory.tistory.com/667 참조
void pass_zone(void);
bool cant_seek_behind(int, int); //뒤에 존재하는 경우 확인 함수. 움직일 경우 자신 기준으로 비교하면 될 듯?
void yh_no_watch(int* sent_len, int yh_period[]);
void catch_move(int* sent_len, int);

int px[PLAYER_MAX], py[PLAYER_MAX], period[PLAYER_MAX];  // 각 플레이어 위치, 이동 주기

void m_init(void) {
	map_init(11, 35);
	yh_print(4, 1, 3, true);
	//player[2] = false; player[5] = false; //죽은 플레이어 test 코드

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
			//period[i] = randint(100, 500); //교수님께 한 phase 당 주기가 달라져야 하는지 질문해야함.

			// (0 .. n_player-1) 왜 0을 붙여야 하는지 모르겠음; 아마 아스키코드 출력형식 때문일 듯?
			back_buf[px[i]][py[i]] = '0' + i;
		}
	}

	tick = 0; // 초기화
}

void yh_print(int x, int y, int num, bool look) {
	char icon = '@';

	if (!look) icon = '*';

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

void move_m_random(int pnum) {
	int nx, ny;
	int percent[10] = { 0,0,0,0,0,0,0,1,2,3 }; // 70%, 10%, 10%, 10% 구현

	static int dx[4] = { -1, 1, 0, 0 };
	static int dy[4] = { 0, 0, -1, 1 };

	do {
		switch (percent[randint(0, 9)]) {
			case 0:
				nx = px[pnum] + dx[2]; ny = py[pnum] + dy[2];
				break; //왼쪽으로
			case 1:
				nx = px[pnum] + dx[0]; ny = py[pnum] + dy[0];
				break; //위쪽으로
			case 2:
				nx = px[pnum] + dx[1]; ny = py[pnum] + dy[1];
				break; //아래쪽으로
			case 3:
				nx = px[pnum]; ny = py[pnum];
				break; //제자리에
		}
	} while (!placable(nx, ny));

	move_tail(pnum, nx, ny);
}

void pass_zone(void) {
	for (int i = 0; i < n_player; i++) {
		if ((player[i] == true) &&
			(px[i] >= 3 && px[i] <= 7) &&
			(py[i] == 1 || py[i] == 2) !=
			(px[i] == 3 && py[i] == 2) !=
			(px[i] == 7 && py[i] == 2)) {

			//player[i] = false; // 테스트로 false 처리
			back_buf[px[i]][py[i]] = ' ';
			//char message1[] = {'p','l','a','y','e','r','0'+i,0};
			//dialog(message1);
		}
	}
}

void yh_no_watch(int *sent_len, int yh_period[]) {
	char sent[] = "무궁화꽃이피었습니다";
	int len = *sent_len; //너무 길어서 넣음
	int pm_time = 50; // 느려지거나 빨라지기 위해 더하는 밀리sec

	if (len <= 9) {
		yh_print(4, 1, 3, false); // 영희 뒤돌아봄
		gotoxy(N_ROW + 1, len * 2); // 출력 장소로 이동

		// 겹치는 코드 나중에 가능하면 수정
		if (len < 5 &&  yh_period[2] % yh_period[0] == 0) {
			printf("%c%c", sent[len * 2], sent[len * 2 + 1]); //한글은 2bit, 따라서 %c%c 사용
			*sent_len += 1;
		}
		else if (len >= 5 && yh_period[2] % yh_period[1] == 0) {
			printf("%c%c", sent[len * 2], sent[len * 2 + 1]); //한글은 2bit, 따라서 %c%c 사용
			*sent_len += 1;

			if (*sent_len == 10) {
				yh_period[2] = 0;
			}
		}
	}
	else if (len == 10){

		yh_print(4, 1, 3, true);
		yh_period[2] += 10; // 무궁화 출력 이후 카운트 해야 하기 때문

		// 대기시간 카운터 테스트 코드
		gotoxy(N_ROW + 2, 0);
		printf("%d 밀리초 대기", yh_period[2]);

		if (yh_period[2] % 3000 == 0) {
			yh_print(4, 1, 3, true);
			*sent_len = 0;
			yh_period[2] = 0;

			gotoxy(N_ROW + 1, 0); // 무궁화 출력을 깨끗이 비움 (더 좋은 방법이 생각나지 않음...)
			printf("                    "); // 2bit이므로 20칸

			// pm_time을 빼서 음수가 되지 않도록 하기 위함.
			if (yh_period[1] > pm_time) {
				yh_period[0] += pm_time;
				yh_period[1] -= pm_time;
			}
		}		
	}
}

void catch_move(int *sent_len, int i) {
	int len = *sent_len;

	if (len == 10) {
		player[i] = false;
		back_buf[px[i]][py[i]] = ' ';
		n_alive--;
	}
}

void mugunghwa(void) {
	m_init();
	system("cls");
	display();
	//dialog("\"무궁화 꽃이 피었습니다\"");

	int yh_period[] = { 500, 500, 0 }; // 무궁화 꽃 t, 피었습니다 t, 3초 카운터 t
	int sent_len = 0; // 출력된 char sent[] 글자 수 카운트
	int mv_user_hy_front[] = {0,0,0,0,0,0,0,0,0,1};

	while (1) {
		yh_no_watch(&sent_len, yh_period);
		yh_period[2] += 10;

		key_t key = get_key();
		if (key == K_QUIT) {
			break;
		}
		else if (key != K_UNDEFINED) {
			move_manual(key);

			if (player[0] == true) {
				catch_move(&sent_len, 0);
			}
		}

		for (int i = 1; i < n_player; i++) {
			period[i] = randint(100, 500);
			if (player[i] == true && tick % period[i] == 0) {
				if (sent_len <= 9) {
					move_m_random(i);
				}
				else if (sent_len == 10 && mv_user_hy_front[randint(0, 9)] == 1) {
					catch_move(&sent_len, i);
				}
			}
		}
		
		display();
		pass_zone();
		Sleep(10);
		tick += 10;
	}
}