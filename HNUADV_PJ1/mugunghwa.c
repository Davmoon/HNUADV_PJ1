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
void yh_no_watch(int yh_period[]);
void catch_move();
void phase_dialog(char);

int px[PLAYER_MAX], py[PLAYER_MAX], period[PLAYER_MAX]; // 각 플레이어 위치, 이동 주기, 패스 여부
int len = 0; //'무궁화꽃이 피었습니다' 출력된 길이 저장
bool a = true;

void m_init(void) {
	map_init(11, 35);
	yh_print(4, 1, 3, true);

	int x, y;
	// 끝 자리에 살아남은 플레이어가 랜덤하게 배치되는 코드
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

	tick = 0; // 초기화
}

void yh_print(int x, int y, int num, bool look) {
	char icon = '@'; //플레이어를 보고 있는 상황

	if (!look) icon = '*'; //등 뒤로 돌고 있는 상황

	for (int i = 0; i < num; i++) {
		back_buf[x + i][y] = icon;
	}
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
	char sent[] = "무궁화꽃이피었습니다";
	int pm_time = 20; // 느려지거나 빨라지기 위해 더하는 밀리sec

	if (len <= 9) {
		yh_print(4, 1, 3, false); // 영희 뒤돌아봄
		gotoxy(N_ROW + 1, len * 2); // 출력 장소로 이동

		// 겹치는 코드 나중에 가능하면 수정
		if (len <= 4 && yh_period[2] % yh_period[0] == 0) {
			printf("%c%c", sent[len * 2], sent[len * 2 + 1]); //한글은 2bit, 따라서 %c%c 사용
			len += 1;
		}
		else if (len >= 5 && yh_period[2] % yh_period[1] == 0) {
			printf("%c%c", sent[len * 2], sent[len * 2 + 1]); //한글은 2bit, 따라서 %c%c 사용
			len += 1;

			if (len == 10) {
				yh_period[2] = 0; //무궁화 출력 타이머 초기화
				catch_move(); // len 10일때도 한번만 실행됨(나이스!!!!!)
			}
		}
	}
	else if (len == 10){

		yh_print(4, 1, 3, true);
		yh_period[2] += 10; // 무궁화 출력 이후 카운트 해야 하기 때문

		// 대기시간 카운터 테스트 코드
		//gotoxy(N_ROW + 2, 0);
		//printf("%d 밀리초 대기", yh_period[2]);

		if (yh_period[2] % 3000 == 0) {
			yh_print(4, 1, 3, true);
			len = 0;
			yh_period[2] = 0; //무궁화 3초 대기 타이머 초기화
			a = true;

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

void catch_move() {

	//플레이어 1명이 10%확율로 죽는 코드 (while 문에 넣으면 10% * (3000 / 10)번 시도되어 불가)
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
	// pass[] 배열을 만들어놨습니다. 사용해주세요.
}

void mugunghwa(void) {
	m_init();
	system("cls");
	display();
	//dialog("\"무궁화 꽃이 피었습니다\"");

	int yh_period[] = { 300, 300, 0}; // 무궁화 꽃 t, 피었습니다 t, 무궁화 전용 타이머(tick에 따르면 오차생김), 페이즈 패스 체크
	

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
			period[i] = randint(100, 200); //매번 다른 진행속도
			if (tick % period[i] == 0) {
				if (len <= 9) {
					move_m_random(i);
				}
			}
		}

		display();
		pass_zone();
		Sleep(10);
		tick += 10; // 시스템 시간
		yh_period[2] += 10; // 무궁화 전용 타이머
	}
}