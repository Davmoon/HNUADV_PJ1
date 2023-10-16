#define _CRT_SECURE_NO_WARNINGS //sprintf() 사용하기 위함 없으면 사이즈 지정해줘야함..
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
bool mv_m_random(int); // 백분율 부분 https://coding-factory.tistory.com/667 참조
void pass_zone(void);
void yh_no_watch(int yh_period[]);
void mv_ten();
bool catch_mv(int, bool, key_t key);

int px[PLAYER_MAX], py[PLAYER_MAX], period[PLAYER_MAX]; // 각 플레이어 위치, 이동 주기, 패스 여부
int len = 0; //'무궁화꽃이 피었습니다' 출력된 길이 저장
char msg1[50] = { "player",}; //dialog() 메세지 저장 sprintf https://jhnyang.tistory.com/314 참조

void m_init(void) {
	map_init(13, 35);
	yh_print(5, 1, 3, true);
	// 주기가 각자 다르고 tick % period[i]가 정확히 떨어지지 않을 수 있으므로 (Lee 조언 감사)
	int period_set[] = { 250, 260, 270, 280, 290, 300, 310, 320, 330, 340};

	int x, y;
	// 끝 자리에 살아남은 플레이어가 랜덤하게 배치되는 코드
	for (int i = 0; i < n_player; i++) {
		if (player[i] == true) {
			do {
				x = randint(1, 11);
				y = 33;
			} while (!placable(x, y));
			px[i] = x;
			py[i] = y;
			period[i] = period_set[i];

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

bool mv_m_random(int pnum) {
	int nx, ny;
	int percent[10] = { 0,0,0,0,0,0,0,1,2,3 }; // 70%, 10%, 10%, 10% 구현
	int mv_stay = 0; // 제자리에 있는 경우 카운터

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
				back_buf[px[pnum]][py[pnum]] = ' ';//placable 우회 위해 빈칸으로 설정
				mv_stay++;
				break; //제자리에
		}
	} while (!placable(nx, ny));

	// mv_stay가 1일때 
	if (mv_stay == 1) {
		back_buf[px[pnum]][py[pnum]] = '0' + pnum; //placable 우회 작업 복구
		return true;// 제자리인 경우 전달 위해 bool 사용.
	}
	else {
		move_tail(pnum, nx, ny);
		return false; // 굳이 없어도 될듯? 오류방지를 위해 추가
	}
}

void pass_zone(void) {
	//yh 3개이므로 인식부분이 5개(변경가능)
	for (int i = 0; i < n_player; i++) {
		//pass 처리와 false 처리가 운좋게 동시에 처리되는 상황 방지
		if (player[i] == true) {

			if ((px[i] == 4 && py[i] == 1) ||
				(px[i] == 5 && py[i] == 2) ||
				(px[i] == 6 && py[i] == 2) ||
				(px[i] == 7 && py[i] == 2) ||
				(px[i] == 8 && py[i] == 1)) {

				back_buf[px[i]][py[i]] = ' ';
				pass[i] = true;
			}
		}
	}
}

void yh_no_watch(int yh_period[]) {
	char sent[] = "무궁화꽃이피었습니다";
	int pm_time = 30; // 느려지거나 빨라지기 위해 더하는 밀리sec

	if (len <= 9) {
		yh_print(5, 1, 3, false); // 영희 off
		gotoxy(N_ROW, len * 2); // 출력 장소로 이동

		// '무궁화꽃이' 출력
		if (len <= 4 && yh_period[2] % yh_period[0] == 0) {
			printf("%c%c", sent[len * 2], sent[len * 2 + 1]); //한글은 2bit, 따라서 %c%c 사용
			len += 1;
			if (len == 4) {
				yh_period[2] = 0; //무궁화 출력 타이머 초기화
			}
		}
		// '피었습니다' 출력
		else if (len >= 5 && yh_period[2] % yh_period[1] == 0) {
			printf("%c%c", sent[len * 2], sent[len * 2 + 1]); //한글은 2bit, 따라서 %c%c 사용
			len += 1;

			if (len == 10) {
				yh_period[2] = 0; //무궁화 출력 타이머 초기화
				mv_ten(); // len 10일때도 한번만 실행됨(나이스!!!, 3일 날림)
			}
		}
	}
	else if (len == 10) {
		yh_print(5, 1, 3, true);
		yh_period[2] += 10; // 무궁화 출력 이후 3초 카운트 해야 하기 때문

		// 대기시간 카운터 테스트 코드
		gotoxy(N_ROW + 1, 0);
		printf("%d 밀리초 대기", yh_period[2]);

		if (yh_period[2] % 3000 == 0) {
			yh_print(5, 1, 3, true); //영희 on
			len = 0; //무궁화 한 페이즈 종료, 새로운 페이즈 시작 위해 초기화
			yh_period[2] = 0; //무궁화 3초 대기 타이머 초기화

			gotoxy(N_ROW, 0); // 무궁화 출력을 깨끗이 비움 (더 좋은 방법이 생각나지 않음...)
			printf("                    "); // 2bit이므로 20칸

			// 한명도 안죽었으면 dialog() 대기시간 스킵
			if (msg1[6] != '\0') {
				sprintf(msg1, "%s %s", msg1, "dead!");
				dialog(msg1);
			}

			msg1[6] = '\0';//player 외 초기화

			// pm_time을 빼서 음수가 되지 않도록 하기 위함.
			if (yh_period[1] > pm_time) {
				yh_period[0] += pm_time;
				yh_period[1] -= pm_time;
			}
		}
	}
}

void mv_ten() {
	for (int i = 1; i < n_player; i++) {
		if (player[i] == true && pass[i] == false && randint(0, 9) == 1) {
			// catch_mv를 돌리고 제자리인지 확인, 아니면 이동한 자리에서 다시 가려졌는지 검사함. (머리 빡세게 굴렸다...)
			if (!catch_mv(i, true, '\0')) {
				catch_mv(i, false, '\0');
			}
			//player[i] = false; //10% 확률로 죽기만 하면 된다면 이 코드를 사용.
			//back_buf[px[i]][py[i]] = ' ';
		}
	}
}

bool catch_mv(int pnum, bool count, key_t key) {
	int al_chk_count = 0; //플레어어 모두 체크해야 결정할 수 있음.
	for (int i = 0; i < n_player; i++) {

		// if문의 조건 < 에서 자기자신은 걸러짐 (등호 없음)
		if (player[i] == true && pass[i] == false && py[i] < py[pnum] && px[i] == px[pnum]) {

			//테스트 좌표 출력
			gotoxy(N_ROW + 3, 0);
			printf("%d pass 좌표는: %d %d | %d %d", pnum, px[i], py[i], px[pnum], py[pnum]);

			// 0번 앞에 뭔가 있으면 움직일 수 있도록 하는 코드
			if (pnum == 0 && count == true) {
				move_manual(key);
				return true;
			}
			break;
		}
		else { al_chk_count++; }

		if (player[pnum] == true && pass[pnum] == false && al_chk_count == n_player) {
			// 움직인 공간에 다른 플레이어가 존재해 움직이지 못하고 가만히 있는 상태일때
			if (pnum == 0 && count == true && move_manual(key) == false) {
				move_manual(key);
				return false;
			}
			// 제자리에 있는 경우 잡지 않기. true인 경우 한번은 돌리고 제자리인 경우 catch_mv 다시 안돌리도록
			if (count == true && mv_m_random(i) == true) {
				return true;
			}
			//테스트 좌표 출력
			gotoxy(N_ROW + 2, 0);
			printf("%d kill 좌표는: %d %d | %d %d", pnum, px[i], py[i], px[pnum], py[pnum]);

			sprintf(msg1, "%s %d", msg1, pnum); //kill 플레이어 저장
			back_buf[px[pnum]][py[pnum]] = ' ';
			player[pnum] = false;
			n_alive--;
			return false; //오류 방지 위해 설정
		}
	}
}

void mugunghwa(void) {
	m_init();
	system("cls");
	display();

	dialog(" \"무궁화 꽃이 피었습니다\"");
	int yh_period[] = { 250, 250, 0 }; // 무궁화 꽃 t, 피었습니다 t, 무궁화 전용 타이머(tick에 따르면 오차생김)

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
					if (catch_mv(0, true, key) == true) {
						catch_mv(0, false, '\0');
					}
					
				}
			}
		}

		for (int i = 1; i < n_player; i++) {
			if (tick % period[i] == 0 && player[i] == true && pass[i] == false) {
				// 10인 경우, yh_no_watch 안의 ten_mv에서 작동(10% 중복 작동 방지 위해)
				if (len <= 9) {
					mv_m_random(i);
				}
			}
		}
		
		pass_zone();
		display();
		// pass, kill 확인을 위한 테스트 코드
		for (int i = 0; i < n_player; i++) {
			if (pass[i] == true) {
				gotoxy(N_ROW + 15, i * 9);
				printf("P%d(%d,%d)|", i, px[i], py[i]);
			}
			if (player[i] == false) {
				gotoxy(N_ROW + 16, i * 9);
				printf("K%d(%d,%d)|", i, px[i], py[i]);
			}
		}
		Sleep(10);
		tick += 10; // 시스템 시간 (period[i]에서만 사용)
		yh_period[2] += 10; // 무궁화 전용 타이머
	}
}