#include <ncurses.h>
#include <stdio.h>

#define LEFT_BORDER 0
#define TOP_BORDER 0
#define RIGHT_BORDER 80
#define BOT_BORDER 25
#define DEFAULT_SPEED 15
#define SPEED_DELTA 1

int i_border_check(int i);
int j_border_check(int j);
int check_gen(const int gen1[BOT_BORDER][RIGHT_BORDER], const int gen2[BOT_BORDER][RIGHT_BORDER]);
int count_neighbours(const int gen1[BOT_BORDER][RIGHT_BORDER], int i, int j);
int alive(int cell, int neighboors);
void next_gen(int gen1[BOT_BORDER][RIGHT_BORDER], int gen2[BOT_BORDER][RIGHT_BORDER]);
void copy(int gen1[BOT_BORDER][RIGHT_BORDER], int gen2[BOT_BORDER][RIGHT_BORDER]);
int input(int gen1[BOT_BORDER][RIGHT_BORDER]);
void output(const int gen1[BOT_BORDER][RIGHT_BORDER]);
void output_stat(const int gen1[BOT_BORDER][RIGHT_BORDER]);
void change_speed(char button, int *speed);
void game(int gen1[BOT_BORDER][RIGHT_BORDER]);

int main() {
    int gen1[BOT_BORDER][RIGHT_BORDER];

    if (input(gen1) == 0) {
        printf("n/a");
    } else {
        if (freopen("/dev/tty", "r", stdin) == NULL)
            printf("n/a");
        else {
            game(gen1);
            output_stat(gen1);
        }
    }

    return 0;
}

void game(int gen1[BOT_BORDER][RIGHT_BORDER]) {
    initscr();
    noecho();
    int gen2[BOT_BORDER][RIGHT_BORDER];
    int speed = DEFAULT_SPEED;
    halfdelay(DEFAULT_SPEED);

    while (1) {
        clear();
        output(gen1);
        next_gen(gen1, gen2);
        char button = getch();
        change_speed(button, &speed);
        if (check_gen(gen1, gen2) == 1 || button == 'q' || button == 'Q') break;
        copy(gen1, gen2);
    }

    endwin();
}

int input(int gen1[BOT_BORDER][RIGHT_BORDER]) {
    int correct_input = 1;
    for (int i = 0; i < BOT_BORDER && correct_input == 1; i++) {
        for (int j = 0; j < RIGHT_BORDER && correct_input == 1; j++) {
            if (scanf("%1d", &gen1[i][j]) != 1 || (gen1[i][j] != 1 && gen1[i][j] != 0)) {
                correct_input = 0;
            }
        }
    }
    return correct_input;
}

void output(const int gen1[BOT_BORDER][RIGHT_BORDER]) {
    move(0, 0);
    for (int i = 0; i < BOT_BORDER; i++) {
        for (int j = 0; j < RIGHT_BORDER; j++) {
            if (gen1[i][j] == 1) {
                mvprintw(i, j, "*");
            } else {
                mvprintw(i, j, " ");
            }
        }
    }
    refresh();
}

void output_stat(const int gen1[BOT_BORDER][RIGHT_BORDER]) {
    for (int i = 0; i < BOT_BORDER; i++) {
        for (int j = 0; j < RIGHT_BORDER; j++) {
            if (gen1[i][j] == 1) {
                printf("*");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}

int i_border_check(int i) {
    int new_i = i;
    if (i < TOP_BORDER) new_i = BOT_BORDER - 1;
    if (i > BOT_BORDER - 1) new_i = TOP_BORDER;
    return new_i;
}

int j_border_check(int j) {
    int new_j = j;
    if (j > RIGHT_BORDER - 1) new_j = LEFT_BORDER;
    if (j < LEFT_BORDER) new_j = RIGHT_BORDER - 1;
    return new_j;
}

int count_neighbours(const int gen1[BOT_BORDER][RIGHT_BORDER], int i, int j) {
    int counter = 0;
    if (gen1[i_border_check(i - 1)][j_border_check(j - 1)] == 1) counter++;  // left top
    if (gen1[i_border_check(i + 1)][j_border_check(j + 1)] == 1) counter++;  // right bot
    if (gen1[i_border_check(i - 1)][j_border_check(j + 1)] == 1) counter++;  // right top
    if (gen1[i_border_check(i + 1)][j_border_check(j - 1)] == 1) counter++;  // left bot
    if (gen1[i_border_check(i - 1)][j_border_check(j)] == 1) counter++;      // top
    if (gen1[i_border_check(i + 1)][j_border_check(j)] == 1) counter++;      // bot
    if (gen1[i_border_check(i)][j_border_check(j - 1)] == 1) counter++;      // left
    if (gen1[i_border_check(i)][j_border_check(j + 1)] == 1) counter++;      // right
    return counter;
}

int alive(int cell, int neighboors) {
    int live = 0;

    if (neighboors == 3 && cell == 0) live = 1;
    if ((neighboors == 2 || neighboors == 3) && cell == 1) live = 1;

    return live;
}

void next_gen(int gen1[BOT_BORDER][RIGHT_BORDER], int gen2[BOT_BORDER][RIGHT_BORDER]) {
    for (int i = 0; i < BOT_BORDER; i++) {
        for (int j = 0; j < RIGHT_BORDER; j++) {
            gen2[i][j] = alive(gen1[i][j], count_neighbours(gen1, i, j));
        }
    }
}

int check_gen(const int gen1[BOT_BORDER][RIGHT_BORDER], const int gen2[BOT_BORDER][RIGHT_BORDER]) {
    int same = 1;
    for (int i = 0; i < BOT_BORDER && same == 1; i++) {
        for (int j = 0; j < RIGHT_BORDER && same == 1; j++) {
            if (gen1[i][j] != gen2[i][j]) same = 0;
        }
    }
    return same;
}

void copy(int gen1[BOT_BORDER][RIGHT_BORDER], int gen2[BOT_BORDER][RIGHT_BORDER]) {
    for (int i = 0; i < BOT_BORDER; i++) {
        for (int j = 0; j < RIGHT_BORDER; j++) {
            gen1[i][j] = gen2[i][j];
        }
    }
}

void change_speed(char button, int *speed) {
    if (button == 'p' || button == 'P') {  // быстрее
        halfdelay(*speed -= SPEED_DELTA);
    } else if (button == 'm' || button == 'M') {  // медленнее
        halfdelay(*speed += SPEED_DELTA);
    }
}
