// Snake developed by Thanos Dimitrakopoulos

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define MAX_X 60
#define MAX_Y 20

int score, hscore, key , prev_key;
double speed = 0.05;

char base[MAX_Y][MAX_X];

typedef struct snake_body { // Snake's body
       int y;
       int x;
       struct snake_body *prev;
       struct snake_body *next;
} body;

body *body_head = NULL; // Snake's head
body *body_tail = NULL; // Snake's tail

typedef struct obstacles { // Position of obstacles
        int y;
        int x;
        struct obstacles *next;
} obstacle;

obstacle *obstacle_head = NULL;
obstacle *obstacle_tail = NULL;

struct point {
       int y;
       int x;
} food_pos;

void welcome();
void end();
void wait();
void gotoxy(unsigned short, unsigned short);
void init();
void add_body(int, int);
void draw_snake();
void display();
void input();
void direction();
void move(int, int);
void food();
void check();
void self_eat();
void deallocate();
int main();
void welcome()
{
     gotoxy (35, 6);
     printf ("Food: \" %c \"", 254);
     gotoxy (28,8);
     printf("Press ENTER to strat");
     key = getch();
     while (key != 13) {
           key = getch();
     }
     system("cls");
}

void end()
{
     int key;

     deallocate ();
     system ("cls");
     gotoxy (30, 3);
     printf ("You lost. Score: %d", score);
     gotoxy (10, 6);
     printf ("Wanna play again? Press Enter to replay or Escape to exit. ");

     key = getch();
     while (key != 13 && key != 27) {
           key = getch();
     }
     if (key == 13) {
             main();
     }
     else if (key == 27) {
          exit(0);
     }
}

void wait(double speed)
{
	clock_t wait;
	wait = clock() + speed * CLOCKS_PER_SEC;
	while ( clock() < wait ) {}
}

void gotoxy(unsigned short x, unsigned short y)
{
     HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
     COORD position = {x, y};
     SetConsoleCursorPosition(handle, position);
}

void init()
{
     int y, x;

     score = 0;
     for (y = 0; y < MAX_Y ; y++) {
         for (x = 0; x < MAX_X; x++) {
             base[y][x] = ' ';
         }
     }
     for (y = 1; y < MAX_Y - 1; y++) {
         base[y][0] = 186;
         base[y][MAX_X - 1] = 186;
     }

     for (x = 1; x < MAX_X - 1; x++) {
         base[0][x] = 205;
         base[MAX_Y - 1][x] = 205;
     }

     base[0][0] = 201;
     base[0][MAX_X - 1] = 187;
     base[MAX_Y - 1][0] = 200;
     base[MAX_Y - 1][MAX_X - 1] = 188;
//sanke
     add_body(10, 30);
     add_body(10, 29);
     draw_snake();
     food();
     key = (rand() % (80-72)) +72;
     while ( (key != 72) && (key != 75) && (key != 77) && (key != 80) ) {
          key = ( rand() % (80-72) ) +72;
     }
     prev_key = key;
}

void add_body(int y, int x)
{
     body *tmp;

     tmp = (body *) malloc ( sizeof(body) );
     if (tmp == NULL) {
             printf ("\nAn error occured while allocating memory.");
             getchar();
             exit(1);
     }

     if (body_head == NULL) {
              body_head = tmp;
              tmp->prev = NULL;
     }
     else {
          body_tail->next = tmp;
          tmp->prev = body_tail;
     }

     tmp->y = y;
     tmp->x = x;
     tmp->next = NULL;
     body_tail = tmp;
}

void draw_snake()
{
     body *curr;
     int flag=1;
     curr = body_head;

     while (curr != NULL) {
           base[curr->y][curr->x] = '*';
           curr = curr->next;
     }
     self_eat();
}

void display()
{
     gotoxy(0 ,0);
     int y, x;

     printf ("\n\tScore: %d", score);
     printf ("\n");

     for (y = 0; y < MAX_Y ; y++) {
         printf ("\t");
         for (x = 0; x < MAX_X; x++) {
             printf ("%c", base[y][x]);
         }
         printf ("\n");
     }
     wait(speed);
}

void input()
{
     if ( kbhit() ) { // If a key is pressed
            prev_key = key;
            key = getch(); // getch() function returns two values, the valid one is the second
            key = getch();

         }

         if (prev_key == 77 && key == 75) { // If the snake goes right and press the left key
                      key = prev_key;
         }
         else if (prev_key == 75 && key == 77) { // If the snake goes left and press the right key
              key = prev_key;
         }
         else if (prev_key == 72 && key == 80) { // If the snake goes up and press the down key
              key = prev_key;
         }
         else if (prev_key == 80 && key == 72) { // If the snake goes down and press the up key
              key = prev_key;
         }
}

void direction()
{
     int y = body_head->y, x = body_head->x;

     if (key == 72) {
          y--;
          move(y, x);
     }
     else if (key == 80) {
          y++;
          move(y, x);
     }
     else if (key == 77) {
          x++;
          move(y, x);
     }
     else if (key == 75) {
          x--;
          move(y, x);
     }
     else {
          key = prev_key;
          direction();
     }

}

void move(int y, int x)
{
     body *curr;
     curr = body_tail;

     base[body_tail->y][body_tail->x] = ' ';

     while (curr != body_head) {
           curr->y = curr->prev->y;
           curr->x = curr->prev->x;
           curr = curr->prev;
     }

     body_head->y = y;
     body_head->x = x;

     draw_snake();
     check();
}

void food()
{
     body *b_tmp;
     obstacle *o_tmp;

     // Values that store if food has the same position with a part of the snake's body or with an obstacle
     int b_con = 0, o_con = 0;

     // Sets the random position of food
     food_pos.x = (rand() % (MAX_X - 3) ) + 1;
     food_pos.y = (rand() % (MAX_Y - 3) ) + 1;

     // Snake check
     b_tmp = body_head;
     while (b_tmp != NULL) {
           if ( (food_pos.y == b_tmp->y) && (food_pos.x == b_tmp->x) ) {
                b_con = 1;
                b_tmp = NULL;
           }
           else {
                b_tmp = b_tmp->next;
           }
     }

     if ( (b_con == 1) || (o_con == 1) ) {
          food();
     }

     base[food_pos.y][food_pos.x] = 254;
}

void check()
{
     if ( (body_head->y == food_pos.y) && (body_head->x == food_pos.x) ) {
          food();
          ++score;
          speed -= 0.01;

          add_body(body_tail->y, body_tail->x);
          draw_snake();
     }
     if ( ( body_head->y == 0 ) || ( body_head->x == 0 ) || ( body_head->y == (MAX_Y - 1) ) || ( body_head->x == (MAX_X - 1) ) ) {
          end();
     }
}

void self_eat()
{
     body *tmp;
     tmp = body_head->next;

     while (tmp != NULL) {
           if ( (body_head->y == tmp->y) && (body_head->x == tmp->x) ) {
                tmp = NULL;
                end();
           }
           else {
                tmp = tmp->next;
           }
     }
}

void deallocate()
{
     body *b_curr, *b_next;
     obstacle *o_curr, *o_next;

     b_curr = body_head;
     while (b_curr != NULL) {
           b_next = b_curr->next;
           free(b_curr);
           b_curr = b_next;
     }

     o_curr = obstacle_head;
     while (o_curr != NULL) {
           o_next = o_curr->next;
           free(o_curr);
           o_curr = o_next;
     }

     body_head = NULL;
     body_tail = NULL;
     obstacle_head = NULL;
     obstacle_tail = NULL;
}

int main(void)
{
    welcome();
    srand (time(NULL));
    init();
    display();

    while (1) {
         direction();
         input();
         display();
    }
    return 0;
}
