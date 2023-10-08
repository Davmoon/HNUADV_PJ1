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

	// 9명 이상일 경우 오류발생. 9명까지로 제한
	while (n_player > 9){
		printf("최대 9명까지 플레이 할 수 있습니다. 다시 입력해주세요 : ");
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

}

int main(void) {
	jjuggumi_init();//인원 입력 함수
	//sample();
	//intro();
	mugunghwa();
	//nightgame();
	//juldarigi();
	//jebi();
	//ending();
	return 0;
}
