#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 8
#define MINES 10

typedef struct {
    int isMine;
    int isOpen;
    int isFlagged;
    int adjacentMines;
} Cell;

Cell board[SIZE][SIZE];

// Add global variable for flag count
int flagCount = 0;

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

// Modify flagCell function
void flagCell(int x, int y) {
    if (x < 0 || x >= SIZE || y < 0 || y >= SIZE || board[x][y].isOpen) return;
    if (!board[x][y].isFlagged) {
        board[x][y].isFlagged = 1;
        flagCount++;
    }
}

// Modify unflagCell function
void unflagCell(int x, int y) {
    if (x < 0 || x >= SIZE || y < 0 || y >= SIZE) return;
    if (board[x][y].isFlagged) {
        board[x][y].isFlagged = 0;
        flagCount--;
    }
}

int checkWin() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (!board[i][j].isMine && !board[i][j].isOpen) return 0;
        }
    }
    return 1;
}

void printBoard() {
    // Print top column indices
    printf("\n     ");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", i);
    }
    printf("\n");

    // Print top horizontal line
    printf("    ");
    for (int i = 0; i < SIZE * 2 + 1; i++) {
        printf("-");
    }
    printf("\n");

    // Print board with row indices on both sides
    for (int i = 0; i < SIZE; i++) {
        printf(" %d | ", i);  // Left row index
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
        printf("| %d\n", i);  // Right row index
    }

    // Print bottom horizontal line
    printf("    ");
    for (int i = 0; i < SIZE * 2 + 1; i++) {
        printf("-");
    }
    printf("\n");

    // Print bottom column indices
    printf("     ");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", i);
    }
    printf("\n");
}

// Modify resetGame function
void resetGame() {
    initializeBoard();
    placeMines();
    calculateAdjacentMines();
    flagCount = 0;
}

// Update main loop display
int main() {
    resetGame();  // Initial game setup

    int x, y;
    char action;
    while (1) {
        system("cls");  // Clear screen
        printBoard();
        printf("\nMines: %d | Flags placed: %d", MINES, flagCount);
        printf("\nCommands:");
        printf("\n o x y - open cell");     // Changed order
        printf("\n f x y - flag cell");     // Changed order
        printf("\n u x y - unflag cell");   // Changed order
        printf("\n r - reset game");
        printf("\n E - exit game");
        printf("\nEnter command: ");

        char input[10];
        fgets(input, sizeof(input), stdin);
        
        if (input[0] == 'r' && input[1] == '\n') {
            resetGame();
            continue;
        }
        if (input[0] == 'E' && input[1] == '\n') {
            printf("\nThanks for playing!\n");
            return 0;
        }

        // Handle cell commands - swap x,y order on input
        if (sscanf(input, "%c %d %d", &action, &y, &x) != 3) {  // Swapped x,y
            printf("Invalid input! Try again.\n");
            continue;
        }

        if (x < 0 || x >= SIZE || y < 0 || y >= SIZE) {
            printf("Invalid coordinates! Use 0-%d\n", SIZE-1);
            continue;
        }

        // Rest of the code remains the same since x,y are already swapped
        if (action == 'o') {
            if (board[x][y].isMine) {
                printf("\nGame Over! You hit a mine!\n");
                printf("Press Enter to continue...");
                getchar();
                resetGame();
                continue;
            }
            openCell(x, y);
            if (checkWin()) {
                printf("\nCongratulations! You won!\n");
                printf("Press Enter to continue...");
                getchar();
                resetGame();
                continue;
            }
        } else if (action == 'f') {
            flagCell(x, y);
        } else if (action == 'u') {
            unflagCell(x, y);
        } else {
            printf("Invalid action! Use 'o', 'f', or 'u'\n");
        }
    }

    return 0;
}
