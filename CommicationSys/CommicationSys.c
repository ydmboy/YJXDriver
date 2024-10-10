#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

#define ROAD_WIDTH 20
#define ROAD_LENGTH 50

void drawRoad(int playerPosition) {
	system("cls");
	for (int i = 0; i < ROAD_LENGTH; i++) {
		for (int j = 0; j < ROAD_WIDTH; j++) {
			if (j == 0 || j == ROAD_WIDTH - 1) {
				printf("|");
			}
			else if (j == playerPosition && i == ROAD_LENGTH - 2) {
				printf("A");  // Player's car
			}
			else {
				printf(" ");
			}
		}
		printf("\n");
	}
}

int main() {
	int playerPosition = ROAD_WIDTH / 2;
	char input;

	printf("Welcome to the simple racing game!\n");
	printf("Use 'a' to move left and 'd' to move right. Press 'q' to quit.\n");
	printf("Press any key to start...\n");
	_getch();

	while (1) {
		if (_kbhit()) {
			input = _getch();
			if (input == 'a' && playerPosition > 1) {
				playerPosition--;
			}
			else if (input == 'd' && playerPosition < ROAD_WIDTH - 2) {
				playerPosition++;
			}
			else if (input == 'q') {
				break;
			}
		}
		drawRoad(playerPosition);
		printf("\nUse 'a' to move left, 'd' to move right, 'q' to quit.\n");
		Sleep(100);  // Delay to simulate movement
	}

	printf("Game Over!\n");
	return 0;
}