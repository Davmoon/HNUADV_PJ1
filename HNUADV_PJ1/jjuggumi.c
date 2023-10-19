// 2023-2 고급프로그래밍 과제: 쭈꾸미 게임
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "jjuggumi.h"

#define	DATA_FILE	"jjuggumi.dat"

int jjuggumi_init(void);

// low 이상 high 이하 난수를 발생시키는 함수
int randint(int low, int high) {
	int rnum = rand() % (high - low + 1) + low;
	return rnum;
}

int jjuggumi_init(void) {
	srand((unsigned int)time(NULL));

	printf("플레이어 수: ");
	scanf_s("%d", &n_player);

	// 10명 이상일 경우 오류발생. 10명까지로 제한
	while (n_player > 10){
		printf("최대 10명까지 플레이 할 수 있습니다. 다시 입력해주세요 : ");
		scanf_s("%d", &n_player);
	}

	n_alive = n_player;
	for (int i = 0; i < n_player; i++) {
		player[i] = true;
		pass[i] = false;
	}
	return 0;
}

void intro() {
    printf("   _    _                                         _ \n");
    printf("  (_)  (_)                                       (_)\n");
    Sleep(200);
    printf("   _    _  _   _   __ _   __ _  _   _  _ __ ___   _ \n");
    Sleep(200);
    printf("  | |  | || | | | / _` | / _` || | | || '_ ` _ \\ | |\n");
    Sleep(200);
    printf("  | |  | || |_| || (_| || (_| || |_| || | | | | || |\n");
    Sleep(200);
    printf("  | |  | | \\__,_| \\__, | \\__, | \\__,_||_| |_| |_||_|\n");
    Sleep(200);
    printf(" _/ | _/ |         __/ |  __/ |                     \n");
    Sleep(200);
    printf("|__/ |__/         |___/  |___/                      \n");
    Sleep(300);

    system("cls");
}

void ending() {
    // 1명만 남은 경우: 즉시 엔딩으로 넘어가서 쭈꾸미 게임을 종료
    int alive_count = 0;
    int last_alive_player = -1;
    for (int i = 0; i < n_player; i++) {
        if (player[i]) {
            alive_count++;
            last_alive_player = i;
        }
    }

    if (alive_count == 1) {
        system("cls");
        printf("게임이 종료됩니다. 단 한 명만 남았습니다.\n");
        printf("승자: 플레이어 %d\n", last_alive_player);
        exit(0);
    }

    // 모든 (미니)게임에서, 게임 마스터가 게임을 강제로 종료할 수 있음

    if (alive_count > 1) {
        system("cls");
        printf("우승자를 가리지 못했습니다.\n");
        printf("살아 남은 플레이어:\n");
        for (int i = 0; i < n_player; i++) {
            if (player[i]) {
            printf("플레이어 %d\n", i);
            }
        }
        exit(0);
    }
    
}


int main(void) {
    intro();
	jjuggumi_init();//인원 입력 함수
    //sample();
	mugunghwa();
	//nightgame();
	//juldarigi();
	//jebi();
	ending();
	return 0;
}
