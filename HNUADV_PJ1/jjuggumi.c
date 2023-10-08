// 2023-2 ������α׷��� ����: �޲ٹ� ����
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "jjuggumi.h"

#define	DATA_FILE	"jjuggumi.dat"

int jjuggumi_init(void);

// low �̻� high ���� ������ �߻���Ű�� �Լ�
int randint(int low, int high) {
	int rnum = rand() % (high - low + 1) + low;
	return rnum;
}

int jjuggumi_init(void) {
	srand((unsigned int)time(NULL));

	printf("�÷��̾� ��: ");
	scanf_s("%d", &n_player);

	// 9�� �̻��� ��� �����߻�. 9������� ����
	while (n_player > 9){
		printf("�ִ� 9����� �÷��� �� �� �ֽ��ϴ�. �ٽ� �Է����ּ��� : ");
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
void ending() {
    // 1�� ���� ���: ��� �������� �Ѿ�� �޲ٹ� ������ ����
    int alive_count = 0;
    int last_alive_player = -1;
    for (int i = 0; i < n_player; i++) {
        if (player[i]) {
            alive_count++;
            last_alive_player = i;
        }
    }

    if (alive_count == 1) {
        printf("������ ����˴ϴ�. �� �� �� ���ҽ��ϴ�.\n");
        printf("����: �÷��̾� %d\n", last_alive_player + 1);
        exit(0);
    }

    system("cls");

    // ��� (�̴�)���ӿ���, ���� �����Ͱ� ������ ������ ������ �� ����
    char choice;
    printf("������ �����Ϸ��� 'q'�� �Է��ϰų� ����Ϸ��� �ٸ� Ű�� ��������: ");
    scanf_s(" %c", &choice);

    if (choice == 'q') {
        // 2�� �̻��� ���� ���
        if (alive_count > 1) {
            printf("����ڸ� ������ ���߽��ϴ�.\n");
            printf("��� ���� �÷��̾�:\n");
            for (int i = 0; i < n_player; i++) {
                if (player[i]) {
                    printf("�÷��̾� %d\n", i + 1);
                }
            }
            exit(0);
        }
    }
}


int main(void) {
	jjuggumi_init();//�ο� �Է� �Լ�
	//sample();
	//intro();
	mugunghwa();
	//nightgame();
	//juldarigi();
	//jebi();
	ending();
	return 0;
}
