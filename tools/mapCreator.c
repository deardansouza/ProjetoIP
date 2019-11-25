#include<stdio.h>
#include<stdbool.h>

#define GRID 60

typedef struct{
	float col1_x, col1_y, col1_w, col1_h;
	float col2_x, col2_y, col2_w, col2_h;
	float col3_x, col3_y, col3_w, col3_h;
	float col4_x, col4_y, col4_w, col4_h;
	float col5_x, col5_y, col5_w, col5_h;
	float col6_x, col6_y, col6_w, col6_h;
	float col7_x, col7_y, col7_w, col7_h;
	float col8_x, col8_y, col8_w, col8_h;
	float col9_x, col9_y, col9_w, col9_h;
	float col10_x, col10_y, col10_w, col10_h;
	float col11_x, col11_y, col11_w, col11_h;
}mapa;

typedef struct{
	float x, y;
	float movx, movy;
	int lives;
	bool invulnerable;
}creature;

int main(void){
    FILE *fp;

    creature player = {.movx = 4.0, .movy = 4.0, .lives = 3, .invulnerable = false};
    creature player2 = {.movx = 4.0, .movy = 4.0, .lives = 3, .invulnerable = false};
    creature enemy1 = {.lives = 1, .invulnerable = false};
    creature enemy2 = {.lives = 1, .invulnerable = false};
    creature enemy3 = {.lives = 1, .invulnerable = false};
    creature arrow = {.movx = 0.0, .movy = 0.0, .lives = 1, .invulnerable = true};

    //Mapa 2
    mapa mapaCaverna = {
        .col1_x = GRID*0, .col1_y = GRID*0, .col1_w = GRID*1, .col1_h = GRID*12,
        .col2_x = GRID*1, .col2_y = GRID*0, .col2_w = GRID*20, .col2_h = GRID*1,
        .col3_x = GRID*0, .col3_y = GRID*11, .col3_w = GRID*4, .col3_h = GRID*1,
        .col4_x = GRID*19, .col4_y = GRID*0, .col4_w = GRID*1, .col4_h = GRID*8,
        .col5_x = GRID*6, .col5_y = GRID*11, .col5_w = GRID*2, .col5_h = GRID*1,
        .col6_x = GRID*14, .col6_y = GRID*10, .col6_w = GRID*7, .col6_h = GRID*2,
        .col7_x = GRID*8, .col7_y = GRID*5, .col7_w = GRID*6, .col7_h = GRID*7,
        .col8_x = GRID*8, .col8_y = GRID*1, .col8_w = GRID*5, .col8_h = GRID*2,
        .col9_x = GRID*2, .col9_y = GRID*3, .col9_w = GRID*2, .col9_h = GRID*3,
        .col10_x = GRID*0, .col10_y = GRID*0, .col10_w = GRID*0, .col10_h = GRID*0,
        .col11_x = GRID*0, .col11_y = GRID*0, .col11_w = GRID*0, .col11_h = GRID*0
    };

    player.x = GRID*5, player.y = GRID*11;

    player2.x = GRID*18, player2.y = GRID*2;

    enemy1.x = GRID*5, enemy1.y = GRID*4;
    enemy1.movx = 4.0, enemy1.movy = 0.0;

    enemy2.x = GRID*17, enemy2.y = GRID*3;
    enemy2.movx = -4.0, enemy2.movy = 0.0;

    enemy3.x = GRID*19, enemy3.y = GRID*8;
    enemy3.movx = 0.0, enemy3.movy = -4.0;

    arrow.x = GRID*22, arrow.y = GRID*6;


    fp = fopen("mapa2.bin", "wb");
    if (fp == NULL){ printf("Unable to open file!");}
    fwrite(&mapaCaverna, sizeof(mapaCaverna), 1, fp);
    fwrite(&player, sizeof(player), 1, fp);
    fwrite(&player2, sizeof(player2), 1, fp);
    fwrite(&enemy1, sizeof(enemy1), 1, fp);
    fwrite(&enemy2, sizeof(enemy2), 1, fp);
    fwrite(&enemy3, sizeof(enemy3), 1, fp);
    fwrite(&arrow, sizeof(arrow), 1, fp);
    fclose(fp);

    return 0;
}