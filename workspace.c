#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 10
#define MINES 10

typedef struct {
    int isMine;
    int isOpen;
    int isFlagged;
    int adjacentMines;
} Cell;

Cell board[SIZE][SIZE];

void initializeBoard() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board[i][j].isMine = 0;
            board[i][j].isOpen = 0;
            board[i][j].isFlagged = 0;
            board[i][j].adjacentMines = 0;
        }
    }
}

void placeMines() {
    srand(time(NULL));
    int minesPlaced = 0;
    while (minesPlaced < MINES) {
        int x = rand() % SIZE;
        int y = rand() % SIZE;
        if (!board[x][y].isMine) {
            board[x][y].isMine = 1;
            minesPlaced++;
        }
    }
}

void calculateAdjacentMines() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j].isMine) continue;
            int count = 0;
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    int nx = i + dx;
                    int ny = j + dy;
                    if (nx >= 0 && nx < SIZE && ny >= 0 && ny < SIZE && board[nx][ny].isMine) {
                        count++;
                    }
                }
            }
            board[i][j].adjacentMines = count;
        }
    }
}

void openCell(int x, int y) {
    if (x < 0 || x >= SIZE || y < 0 || y >= SIZE || board[x][y].isOpen || board[x][y].isFlagged) return;
    board[x][y].isOpen = 1;
    if (board[x][y].adjacentMines == 0) {
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                openCell(x + dx, y + dy);
            }
        }
    }
}

void flagCell(int x, int y) {
    if (x < 0 || x >= SIZE || y < 0 || y >= SIZE || board[x][y].isOpen) return;
    board[x][y].isFlagged = !board[x][y].isFlagged;
}

void printBoard() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j].isFlagged) {
                printf("F ");
            } else if (!board[i][j].isOpen) {
                printf(". ");
            } else if (board[i][j].isMine) {
                printf("* ");
            } else {
                printf("%d ", board[i][j].adjacentMines);
            }
        }
        printf("\n");
    }
}

int main() {
    initializeBoard();
    placeMines();
    calculateAdjacentMines();

    int x, y;
    char action;
    while (1) {
        printBoard();
        printf("Enter action (o for open, f for flag) and coordinates (x y): ");
        if (scanf(" %c %d %d", &action, &x, &y) != 3) {
            printf("Invalid input. Please enter action and coordinates in the format: o 3 4 or f 2 5\n");
            while (getchar() != '\n'); // Clear the input buffer
            continue;
        }
        if (x < 0 || x >= SIZE || y < 0 || y >= SIZE) {
            printf("Invalid coordinates. Please enter values between 0 and %d.\n", SIZE - 1);
            continue;
        }
        if (action == 'o') {
            if (board[x][y].isMine) {
                printf("Game Over! You hit a mine.\n");
                break;
            }
            openCell(x, y);
        } else if (action == 'f') {
            flagCell(x, y);
        } else {
            printf("Invalid action. Use 'o' for open and 'f' for flag.\n");
        }
    }

    return 0;
}
