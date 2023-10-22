#include <stdio.h>
#include <stdbool.h>
#include <wchar.h>
#include <locale.h>
#include <unistd.h>
#include <conio.h>

#define HEIGHT 20
#define WIDTH 40

bool board[HEIGHT][WIDTH], paused = false, end = false;
int cursor_i = 0, cursor_j = 0;

void reset_board(bool board[HEIGHT][WIDTH]){
    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            board[i][j] = false;
        }
    }
}

int count_neighbors(bool board[HEIGHT][WIDTH], int i, int j){
    int neighbors = 0;              //wrap-around mode
    for(int m = -1; m < 2; m++){
        for(int n = -1; n < 2; n++){
            if(m != 0 || n != 0){
                if(board[(i + HEIGHT + m) % HEIGHT][(j + WIDTH + n) % WIDTH]){
                    neighbors++;
                }
            }
        }
    }
    return neighbors;
    /*                              no wrap-around
    if(i > 0 && i < HEIGHT - 1 && j > 0 && j < WIDTH - 1){
        int neighbors = 0;
        for(int m = i - 1; m <= i + 1; m++){
            printf("\n%d ", m);
            for(int n = j - 1; n <= j + 1; n++){
                printf("%d ", n);
                if(m != i || n != j){
                    if(board[m][n]){
                        neighbors++;
                    }
                }
            }
        }
        return neighbors;
    }
    else{
        return -1;
    }
    */
}

void print_board(bool board[HEIGHT][WIDTH]){
    system("cls");
    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            if(paused && i == cursor_i && j == cursor_j){
                if(board[i][j]){
                    printf("(O)");
                }
                else{
                    printf("( )");
                }
            }
            else if(board[i][j]){
                printf(" O ");
            }
            else{
                printf(" . ");
            }
        }
        printf("\n");
    }
    printf("\n");
    if(paused){
        printf("paused\n(%d, %d)\n", cursor_i, cursor_j);
        printf("%d\n", count_neighbors(board, cursor_i, cursor_j));
    }
}

void update_board(bool board[HEIGHT][WIDTH]){
    bool board_aux[HEIGHT][WIDTH];
    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            int neighbors = count_neighbors(board, i, j);
            if(board[i][j]){
                if(neighbors < 2 || neighbors > 3 || neighbors == -1){
                    board_aux[i][j] = false;
                }
                else{
                    board_aux[i][j] = true;
                }
            }
            else{
                if(neighbors == 3){
                    board_aux[i][j] = true;
                }
                else{
                    board_aux[i][j] = false;
                }
            }
        }
    }
    memmove(board, board_aux, HEIGHT * WIDTH * sizeof(bool));
}

void pause(){
    paused = true;
    print_board(board);
    while(paused){
        if(kbhit()){
            switch(getch()){
                case 32:                //space
                    paused = false;
                    break;
                case 224:               //arrow keys
                    switch(getch()){
                        case 72:            //up
                            cursor_i = (cursor_i + HEIGHT - 1) % HEIGHT;
                            break;
                        case 80:            //down
                            cursor_i = (cursor_i + 1) % HEIGHT;
                            break;
                        case 75:            //left
                            cursor_j = (cursor_j + WIDTH - 1) % WIDTH;
                            break;
                        case 77:            //right
                            cursor_j = (cursor_j + 1) % WIDTH;
                            break;
                    }
                    break;
                case 'f':
                    board[cursor_i][cursor_j] = !board[cursor_i][cursor_j];
                    break;
                case 'q':
                    end = true;
                    paused = false;
                    break;
                case 's':
                    printf("%d", count_neighbors(board, cursor_i, cursor_j));
                    break;
            }
            print_board(board);
        }
    }
}

void run(){
    while(!end){
        sleep(1);
        if(kbhit()){
            switch(getch()){
                case 32:            //space
                    pause();
                    break;
                case 'r':
                    reset_board(board);
                    pause();
                    break;
                case 'q':
                    end = true;
                    break;
            }
        }
        print_board(board);
        update_board(board);
    }
}

int main(void){
    reset_board(board);
    board[7][7] = true;
    board[8][8] = true;
    board[8][9] = true;
    board[7][9] = true;
    board[6][9] = true;
    printf("%d", count_neighbors(board, 0, 0));
    pause();
}