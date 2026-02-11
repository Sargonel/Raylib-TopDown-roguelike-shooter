#include "raylib.h"
#include "raymath.h"

#define WIDTH 2000
#define HEIGTH 1500

#define dt GetFrameTime()

#define PLAYERHEALTH 100
#define PLAYERSIZE 100

#define MAXENEMY 100
#define ENEMYSIZE 100

#define MAXBULLET 100

typedef struct player_st{
    Vector2 pos;
    Vector2 size;
    float speed;
    float health;
    float damage;
    int count;
    int wave;
    bool active;
}player_st;

typedef struct enemy_st{
    Vector2 pos;
    Vector2 size;
    Vector2 dir;
    float speed;
    float health;
    float damage;
    bool active;
}enemy_st;

typedef struct bullet_st{
    Vector2 pos;
    Vector2 dir;
    float r;
    float speed;
    bool active;
}bullet_st;
void draw_texts(player_st p){
    DrawText(TextFormat("KILL: %i",p.count),0,50,50,RAYWHITE);
    if(p.active){
        DrawText(TextFormat("HEALTH: %.0f",p.health),0,0,50,RAYWHITE);
    }
}
void draw_player(player_st p){
    if(p.active){DrawRectangleV(p.pos,p.size,BLUE);}
}
void update_player(player_st *p){
    if(p->active){
        if(IsKeyDown(KEY_W)&&p->pos.y > 0){p->pos.y -= p->speed *dt;}
        if(IsKeyDown(KEY_S)&&p->pos.y < HEIGTH - PLAYERSIZE){p->pos.y += p->speed *dt;}
        if(IsKeyDown(KEY_A)&&p->pos.x > 0){p->pos.x -= p->speed *dt;}
        if(IsKeyDown(KEY_D)&&p->pos.x < WIDTH - PLAYERSIZE){p->pos.x += p->speed *dt;}
    }
}
void draw_enemy(enemy_st e[MAXENEMY]){
    for(int i=0;i < MAXENEMY;i++){
        if(e[i].active){
            DrawRectangleV(e[i].pos,e[i].size,RED);
        }
    }
}
void update_enemy(enemy_st e[MAXENEMY],player_st p){
    for(int i=0;i < MAXENEMY;i++){
        if(e[i].active){
            
            Vector2 diff = Vector2Subtract(p.pos,e[i].pos);
            e[i].dir = Vector2Normalize(diff);

            e[i].pos.x += e[i].dir.x * e[i].speed *dt;
            e[i].pos.y += e[i].dir.y * e[i].speed *dt;

        }
    }
}
void check_damge(player_st *p, enemy_st e[MAXENEMY]){
    Rectangle player = {p->pos.x, p->pos.y, PLAYERSIZE, PLAYERSIZE};

    for(int i = 0; i < MAXENEMY; i++){
        if(!e[i].active) continue;

        Rectangle enemy = {e[i].pos.x, e[i].pos.y, ENEMYSIZE, ENEMYSIZE};

        if(CheckCollisionRecs(enemy, player)){
            p->health -= e[i].damage * dt;
        }
    }

    if(p->health <= 0){
        p->active = false;
    }
}
void draw_bullet(bullet_st b[MAXBULLET]){
    for(int i=0;i < MAXBULLET;i++){
        if(b[i].active){
            DrawCircleV(b[i].pos,b[i].r,WHITE);
        }
    }
}
void update_bullet(bullet_st b[MAXBULLET]){
    for(int i=0;i < MAXBULLET;i++){
        if(b[i].active){
            b[i].pos.x += b[i].dir.x * b[i].speed *dt;
            b[i].pos.y += b[i].dir.y * b[i].speed *dt;

            if (b[i].pos.x < 0 || b[i].pos.x > WIDTH || b[i].pos.y < 0 || b[i].pos.y > HEIGTH) {
                b[i].active = false;
            }
        }
    }
}
void shoot_bullet(bullet_st b[MAXENEMY],player_st p){
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        for(int i=0;i < MAXBULLET;i++){
            if(!b[i].active){
                b[i].active = true;
                b[i].pos = (Vector2){p.pos.x + PLAYERSIZE/2,p.pos.y + PLAYERSIZE/2};

                Vector2 mouse_pos = GetMousePosition();
                Vector2 diff = Vector2Subtract(mouse_pos,b[i].pos);

                b[i].dir = Vector2Normalize(diff);

                break;
            }
        }
    }
}
void check_hit(player_st *p,enemy_st e[MAXENEMY],bullet_st b[MAXBULLET]){

    for(int i=0;i < MAXENEMY;i++){
        Rectangle enemy = {e[i].pos.x,e[i].pos.y,ENEMYSIZE,ENEMYSIZE};
        
        for(int j=0;j < MAXBULLET;j++){
            if(b[j].active&&CheckCollisionCircleRec(b[j].pos,b[j].r,enemy)){
                b[j].active = false;
                e[i].health -= p->damage;
                
                if(e[i].health <= 0){
                    e[i].active = false;
                    p->count ++;
                }
                break;
            }
        }
    }
}
void reset_game(player_st *p, enemy_st e[MAXENEMY], bullet_st bullets[]) {
   
    p->pos.x = WIDTH/2 - PLAYERSIZE/2;
    p->pos.y = HEIGTH/2 - PLAYERSIZE/2;
    p->health = 100;
    p->count = 0;
    p->active = true;
    p->wave = 0;

    for(int j=0;j < MAXENEMY;j++){
        e[j].active = false;
    }
    for(int i = 0; i < MAXBULLET; i++) {
        bullets[i].active = false;
    }
}
void draw_wave(player_st p){
    float mes = MeasureText("WAVE",50);
    float mes2 = MeasureText(TextFormat("%i",p.wave),25);
    DrawText("WAVE",WIDTH/2 - mes/2,0,50,WHITE);
    DrawText(TextFormat("%i",p.wave),WIDTH/2-mes2,50,50,WHITE);
}
void update_wave(player_st *p){
    p->wave = p->count / 10;
    if(p->wave > 10) p->wave = 10;
}
void wave_logic(player_st p,int *s,float *h){
    if(p.wave == 0){*s = 1;*h = 200;}
    if(p.wave == 1){*s = 2;*h = 100;}
    if(p.wave == 2){*s = 3;*h = 100;}
    if(p.wave == 3){*s = 4;*h = 100;}
    if(p.wave == 4){*s = 5;*h = 200;}
    if(p.wave == 5){*s = 6;*h = 200;}
    if(p.wave == 6){*s = 7;*h = 200;}
    if(p.wave == 7){*s = 8;*h = 200;}
    if(p.wave == 8){*s = 9;*h = 200;}
    if(p.wave == 9){*s = 10;*h = 200;}
    if(p.wave == 10){*s = 15;*h = 300;}
}
int main(void)
{
    InitWindow(WIDTH, HEIGTH, "raylib wave-shooter");
    SetTargetFPS(60);

    float enemy_health;

    player_st player = {
        .pos = {WIDTH/2 - PLAYERSIZE/2,HEIGTH/2 - PLAYERSIZE/2},
        .size = {PLAYERSIZE,PLAYERSIZE},
        .speed = 400.0f,
        .health = PLAYERHEALTH,
        .count = 0,
        .damage = 100,
        .wave = 0,
        .active = true
    };
    enemy_st enemy[MAXENEMY];
    for(int i=0;i < MAXENEMY;i++){
        enemy[i].size.x = ENEMYSIZE;
        enemy[i].size.y = ENEMYSIZE;
        enemy[i].speed = 500.0f;
        enemy[i].active = false;
        enemy[i].damage = 10.0f;
        enemy[i].health = enemy_health;
    }
    bullet_st bullet[MAXBULLET];
    for(int i=0;i < MAXBULLET;i++){
        bullet[i].speed = 1600.0f;
        bullet[i].r = 15.0f;
        bullet[i].active = false;
    }
    
    int active_count = 0;
    int max_on_screen;

    while (!WindowShouldClose())
    {
        if(player.active){
            update_player(&player);
            update_bullet(bullet);
            shoot_bullet(bullet,player);
            update_wave(&player);
            wave_logic(player,&max_on_screen,&enemy_health);

            if(enemy->active){
                update_enemy(enemy,player);
                check_damge(&player,enemy);
                check_hit(&player,enemy,bullet);

            }
            
            active_count = 0;
            for(int i = 0; i < MAXENEMY; i++) {
                if(enemy[i].active) active_count++;
                
            }

            if(active_count < max_on_screen){
                for(int i=0;i < MAXENEMY;i++){
                    if(!enemy[i].active){
                        int side = GetRandomValue(0,3);
                        if(side == 0){enemy[i].pos.x = -100;enemy[i].pos.y = GetRandomValue(0,WIDTH);}
                        if(side == 1){enemy[i].pos.x = WIDTH + 100;enemy[i].pos.y = GetRandomValue(0,WIDTH);}
                        if(side == 2){enemy[i].pos.x = GetRandomValue(0,WIDTH);enemy[i].pos.y = -100;}
                        if(side == 3){enemy[i].pos.x = GetRandomValue(0,WIDTH);enemy[i].pos.y = HEIGTH + 100;}

                        enemy[i].active = true;
                        enemy[i].health = enemy_health;

                        break;
                    }
                }
            }
        }
        else{
            if(IsKeyPressed(KEY_R)){
                reset_game(&player,enemy,bullet);
            }
        }

        BeginDrawing();
            ClearBackground(BLACK);
            if(player.active){
                draw_texts(player);
                draw_player(player);
                draw_enemy(enemy);
                draw_bullet(bullet);
                draw_wave(player);
            }
            else{
                DrawText("GAME OVER", WIDTH/2 - 300, HEIGTH/2 - 50, 100, RED);
                DrawText("Press 'R' to Restart", WIDTH/2 - 250, HEIGTH/2 + 50, 40, WHITE);
                draw_texts(player);
            }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}