//gcc main.c -o main -lraylib -lGL -lm -lpthread -ldl -lrt
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "raylib.h"

const int WIDTH = 800;
const int HEIGHT = 600;

const int STEP = 50;

int score = 0;

bool game = true;

typedef enum{
    right, left, up, down
}direction;

direction head_dir = right;

typedef struct{
    int x;
    int y;
}position;

typedef struct node{
    position pos;
    struct node* next;
}snake_node;

void add_front(snake_node** head){
    snake_node* new_head = (snake_node*)malloc(sizeof(snake_node));
    new_head->pos.x = (*head)->pos.x;
    new_head->pos.y = (*head)->pos.y;
    new_head->next = (*head);
    *head = new_head;
}

void pop_back(snake_node* head){
    snake_node* current = head;
    while(current->next->next != NULL){
        current = current->next;
    }
    free(current->next);
    current->next = NULL;
}

void snake_init(snake_node** head){
    *head = (snake_node*)malloc(sizeof(snake_node));
    (*head)->pos.x = 3;
    (*head)->pos.y = 5;
    (*head)->next = (snake_node*)malloc(sizeof(snake_node));
    (*head)->next->pos.x = 2;
    (*head)->next->pos.y = 5;
    (*head)->next->next = (snake_node*)malloc(sizeof(snake_node));
    (*head)->next->next->pos.x = 1;
    (*head)->next->next->pos.x = 5;
    (*head)->next->next->next = NULL;
}


position* gen_apple_pos(){
    position* apple_pos = (position*)malloc(sizeof(position));
    apple_pos->x = rand() % (WIDTH/STEP);
    apple_pos->y = rand() % (HEIGHT/STEP);
    return apple_pos;
}

int main(){
    InitWindow(WIDTH, HEIGHT, "Snake");
    SetTargetFPS(10);

    snake_node* head = NULL;
    snake_init(&head);


    position* apple_pos = gen_apple_pos();

    while(!WindowShouldClose()){
        BeginDrawing();
            ClearBackground(BLACK);

            if(IsKeyDown(KEY_RIGHT) && head_dir != left){
                head_dir = right;
            }
            if(IsKeyDown(KEY_LEFT) && head_dir != right){
                head_dir = left;
            }
            if(IsKeyDown(KEY_UP) && head_dir != down){
                head_dir = up;
            }
            if(IsKeyDown(KEY_DOWN) && head_dir != up){
                head_dir = down;
            }

            if(game){
                add_front(&head);

                switch(head_dir){
                    case 0:
                        head->pos.x++;
                        break;
                    case 1:
                        head->pos.x--;
                        break;
                    case 2:
                        head->pos.y--;
                        break;
                    case 3:
                        head->pos.y++;
                        break;
                }

                if(apple_pos->x == head->pos.x && apple_pos->y == head->pos.y){
                    score++; 
                    free(apple_pos);
                    apple_pos = gen_apple_pos();
                }
                else pop_back(head);
            }

            snake_node* iter = head;
            while(iter != NULL){
                if(iter->pos.x == head->pos.x && iter->pos.y == head->pos.y && iter != head && game){
                    game = false;
                }
                DrawRectangle(iter->pos.x*STEP, iter->pos.y*STEP, STEP, STEP, GREEN);
                iter = iter->next;
            }

            if(head->pos.x <= 0 || head->pos.x >= WIDTH/STEP - 1 || head->pos.y <= 0 || head->pos.y >= HEIGHT/STEP - 1) game = false;

            DrawRectangle(apple_pos->x * STEP, apple_pos->y * STEP, STEP, STEP, RED);

            for(int i = 0; i < WIDTH/STEP; i++){
                DrawLine(STEP*i, 0, STEP*i, HEIGHT, WHITE);
            }

            for(int i = 0; i < HEIGHT/STEP; i++){
                DrawLine(0, STEP*i, WIDTH, STEP*i, WHITE);
            }

            if(!game){
                DrawText(TextFormat("SCORE: %i", score), WIDTH/2 - STEP * 5, HEIGHT/2 - STEP * 2, 100, YELLOW);
                snake_node* iter = head;
                while(head->next != NULL){
                    iter = head;
                    head = head->next;
                    free(iter);
                }
                if(IsKeyDown(KEY_ENTER)){
                    free(head);
                    snake_init(&head);
                    head_dir = right;
                    score = 0;
                    game = true;
                }
            }
        EndDrawing();
    }

    free(apple_pos);
    free(head);
    CloseWindow();

    return 0;
}