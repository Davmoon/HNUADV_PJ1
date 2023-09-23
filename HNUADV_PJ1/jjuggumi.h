#ifndef _JJUGGUMI_H_ // if no define으로 자세한건 https://blog.naver.com/clown7942/110123052710 참고
#define _JJUGGUMI_H_

#include <Windows.h>
#include <stdbool.h>

#define PLAYER_MAX		10

bool player[PLAYER_MAX];  // 기본값 true, 탈락하면 false
int n_player, n_alive;
int tick;  // 시계

// 미니게임
void sample(void);
//void mugunghwa(void);
//void nightgame(void);
//void juldarigi(void);
//void jebi(void);

int randint(int low, int high);

#endif
