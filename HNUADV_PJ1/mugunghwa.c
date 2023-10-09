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
void mv_m_random(int); // 백분율 부분 https://coding-factory.tistory.com/667 참조
void pass_zone(void);
void yh_no_watch(int yh_period[]);
void catch_move(int);
void ten_mv();

int px[PLAYER_MAX], py[PLAYER_MAX], period[PLAYER_MAX]; // 각 플레이어 위치, 이동 주기, 패스 여부
int len = 0; //'무궁화꽃이 피었습니다' 출력된 길이 저장

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
			period[i] = randint(100, 200);

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

void mv_m_random(int pnum) {
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

	//9명인 경우에 맞추어 yh 5개로 수정
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
	char sent[] = "무궁화꽃이피었습니다";
	int pm_time = 50; // 느려지거나 빨라지기 위해 더하는 밀리sec

	if (len <= 9) {
		yh_print(4, 1, 3, false); // 영희 off
		gotoxy(N_ROW, len * 2); // 출력 장소로 이동

		// 겹치는 코드 나중에 가능하면 수정
		if (len <= 4 && yh_period[2] % yh_period[0] == 0) {
			printf("%c%c", sent[len * 2], sent[len * 2 + 1]); //한글은 2bit, 따라서 %c%c 사용
			len += 1;
			if (len == 4) {
				yh_period[2] = 0; //무궁화 출력 타이머 초기화
			}
		}
		else if (len >= 5 && yh_period[2] % yh_period[1] == 0) {
			printf("%c%c", sent[len * 2], sent[len * 2 + 1]); //한글은 2bit, 따라서 %c%c 사용
			len += 1;

			if (len == 10) {
				yh_period[2] = 0; //무궁화 출력 타이머 초기화
				ten_mv(); // len 10일때도 한번만 실행됨(나이스!!!!!)
			}
		}
	}
	else if (len == 10) {
		yh_print(4, 1, 3, true);
		yh_period[2] += 10; // 무궁화 출력 이후 카운트 해야 하기 때문

		// 대기시간 카운터 테스트 코드
		gotoxy(N_ROW + 1, 0);
		printf("%d 밀리초 대기", tick);

		if (yh_period[2] % 3000 == 0) {
			yh_print(4, 1, 3, true); //영희 on
			len = 0; //무궁화 한 페이즈 종료, 새로운 페이즈 시작 위해 초기화
			yh_period[2] = 0; //무궁화 3초 대기 타이머 초기화

			gotoxy(N_ROW, 0); // 무궁화 출력을 깨끗이 비움 (더 좋은 방법이 생각나지 않음...)
			printf("                    "); // 2bit이므로 20칸

			// pm_time을 빼서 음수가 되지 않도록 하기 위함.
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
	int al_chk_count = 0; //플레어어 모두 체크해야 결정할 수 있음.
	for (int i = 0; i < n_player; i++) {

		
			// if문의 조건 < 에서 자기자신은 걸러짐 (등호 없음)
			if (player[i] == true && pass[i] == false && py[i] < py[a] && px[i] == px[a]) {
				//테스트 좌표 출력
				gotoxy(N_ROW + 3, 0);
				printf("pass 좌표는: %d %d | %d %d", px[i], py[i], px[a], px[a]);

				break;
			}
			else { al_chk_count++; }

			// 모두 체크를 완료했는데도 앞에 아무도없으면 죽이기
			if (player[a] == true && al_chk_count == n_player) {
				//테스트 좌표 출력
				gotoxy(N_ROW + 2, 0);
				printf("0번 kill 좌표는: %d %d | %d %d", px[i], py[i], px[a], px[a]);

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
	//dialog("\"무궁화 꽃이 피었습니다\"");
	int yh_period[] = { 500, 500, 0 }; // 무궁화 꽃 t, 피었습니다 t, 무궁화 전용 타이머(tick에 따르면 오차생김)
	
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
					// 2번 catch 하여 3초 정지중 들어올 때, 나갈 때 모두 잡도록 인식
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
				printf("|%d패스(%d,%d)|", i, px[i], py[i]);
			}
			if (player[i] == false) {
				gotoxy(N_ROW + 15, i * 13);
				printf("|%d죽음(%d,%d)|", i, px[i], py[i]);
			}

		}
		Sleep(10);
		//tick += 10; // (시스템 시간) 여기선 미사용
		yh_period[2] += 10; // 무궁화 전용 타이머
	}
}