#include<stdio.h>
#include<stdbool.h>
#include<allegro5/allegro.h>
#include<allegro5/allegro_image.h>
#include<allegro5/allegro_font.h>
#include<allegro5/allegro_ttf.h>
#include<allegro5/allegro_audio.h>
#include<allegro5/allegro_acodec.h>
//servidor
#include <string.h>
#include "./lib/server.h"
#define MSG_MAX_SIZE 350
#define BUFFER_SIZE (MSG_MAX_SIZE + 100)
#define LOGIN_MAX_SIZE 13
#define MAX_CHAT_CLIENTS 3

//
#define GRID 60
#define SCREEN_W 1260
#define SCREEN_H 720
#define FPS 60.0
enum keys{KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_ATK};
enum directions{UP, DOWN, LEFT, RIGHT};
enum mapNames{MAPA, MAPA_VAZIO};
enum menuStates{PLAY, CREDITS, EXIT, ERROR};

//Posições dos botões no menu
#define xPlay_m 136
#define xPlay_M 314
#define yPlay_m 354
#define yPlay_M 430

#define xCred_m 136
#define xCred_M 314
#define yCred_m 436
#define yCred_M 506

#define xExit_m 93
#define xExit_M 318
#define yExit_m 436
#define yExit_M 582

#define xVolta_m 950
#define xVolta_M 1222
#define yVolta_m 566
#define yVolta_M 670

//Tipos
typedef struct{
	float x, y;
	float movx, movy;
	int lives;
	bool invulnerable;
}creature;

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
	int x, y;
	bool visible;
}Emblema;



//Funcoes
bool Collision(int b1_x, int b1_y, int b1_w, int b1_h, int b2_x, int b2_y, int b2_w, int b2_h){
    if ((b1_x > b2_x + b2_w - 1) || // b1 esta a direita de b2?
        (b1_y > b2_y + b2_h - 1) || // b1 esta abaixo de b2?
        (b2_x > b1_x + b1_w - 1) || // b2 esta a direita de b1?
        (b2_y > b1_y + b1_h - 1)  ) // b2 esta abaixo de b1?
	{
        // sem colisao
        return false;
    }
    // colisao
    return true;
}

bool MapCollision(mapa *mapa, creature *creature){
    if(Collision(creature->x, creature->y, GRID, GRID, mapa->col1_x, mapa->col1_y, mapa->col1_w, mapa->col1_h)||
    Collision(creature->x, creature->y, GRID, GRID, mapa->col2_x, mapa->col2_y, mapa->col2_w, mapa->col2_h)||
    Collision(creature->x, creature->y, GRID, GRID, mapa->col3_x, mapa->col3_y, mapa->col3_w, mapa->col3_h)||
    Collision(creature->x, creature->y, GRID, GRID, mapa->col4_x, mapa->col4_y, mapa->col4_w, mapa->col4_h)||
    Collision(creature->x, creature->y, GRID, GRID, mapa->col5_x, mapa->col5_y, mapa->col5_w, mapa->col5_h)||
    Collision(creature->x, creature->y, GRID, GRID, mapa->col6_x, mapa->col6_y, mapa->col6_w, mapa->col6_h)||
    Collision(creature->x, creature->y, GRID, GRID, mapa->col7_x, mapa->col7_y, mapa->col7_w, mapa->col7_h)|| 
    Collision(creature->x, creature->y, GRID, GRID, mapa->col8_x, mapa->col8_y, mapa->col8_w, mapa->col8_h)|| 
    Collision(creature->x, creature->y, GRID, GRID, mapa->col9_x, mapa->col9_y, mapa->col9_w, mapa->col9_h)||
    Collision(creature->x, creature->y, GRID, GRID, mapa->col10_x, mapa->col10_y, mapa->col10_w, mapa->col10_h)||
    Collision(creature->x, creature->y, GRID, GRID, mapa->col11_x, mapa->col11_y, mapa->col11_w, mapa->col11_h)){
        return true;
    }
    return false;    
}

bool CreatureCollision(creature *c1, creature *c2){
	return Collision(c1->x, c1->y, GRID, GRID, c2->x, c2->y, GRID, GRID);
}

//tela de creditos
int Creditos(ALLEGRO_DISPLAY*);

//tela Menu
int Menu(ALLEGRO_DISPLAY*);


int main(int argc, char *argv[]){

	//posicoes possiveis para os emblemas x y
	int posicoesEmblemas[53][2]={
		600, 250,
		6500, 225,
		700, 250,
		725, 225, 
		750, 250, 
		775, 225, 
		75,  400,
		100, 450, 
		125, 475, 
		150, 500, 
		100, 525, 
		125, 550, 
		150, 570, 
		175, 600,
		200, 600, 
		225, 600, 
		250, 600,
		275, 600, 
		300, 600, 
		325, 600, 
		350, 400, 
		400, 450, 
		425, 475, 
		450, 500, 
		300, 225, 
		350, 225, 
		400, 250, 
		425, 275, 
		375, 300, 
		320, 325, 
		425, 350, 	
		900, 125, 
		950, 150, 
		1000,100, 
		1000,125, 
		1100,100, 
		1125,125, 
		900, 400, 
		850, 400, 
		900, 300, 
		875, 200, 
		900, 250, 
		925, 275, 
		950, 300, 
		975, 350, 
		875, 500, 
		900, 525, 
		925, 550, 
		950, 575, 
		975, 500, 
		1000,525, 
		1125,550, 
		1150,575
	};

	//funcao que retorna randns das posicoes possiveis
	int posicoesEscolhidas[5][2];

	srand(time(NULL));
	for(int i=0; i<6; i++){
		int x = 2*(rand()%26);
		posicoesEscolhidas[i][0] = posicoesEmblemas[x][0]; 
		posicoesEscolhidas[i][1] = posicoesEmblemas[x][1];	
	}


	FILE *fp;
	
	//Flags
	bool reDesenhar = true;
	bool exit = false;
	bool keysP1[5] = {false, false, false, false, false};
	bool keysP2[5] = {false, false, false, false, false};
	bool dir[4] = {false, false, false, false};
	bool changeMap = true;
	int mapFlag = MAPA;

	//Inicializa allegro e addons
	if(!al_init()){
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}
	if(!al_install_keyboard()){
		fprintf(stderr, "failed to initialize keyboard!\n");
		return -1;
	}
	if(!al_init_image_addon()){
		fprintf(stderr, "failed to initialize image addon!\n");
		return -1;
	}
	if(!al_init_font_addon()){
		fprintf(stderr, "failed to initialize font addon!\n");
		return -1;
	}
	if(!al_init_ttf_addon()){
		fprintf(stderr, "failed to initialize ttf addon!\n");
		return -1;
	}
	if(!al_install_audio()){
		fprintf(stderr, "failed to initialize audio!\n");
		return -1;
	}
	if(!al_init_acodec_addon()){
		fprintf(stderr, "failed to initialize acodec addon!\n");
		return -1;
	}
	if(!al_reserve_samples(5)){
		fprintf(stderr, "failed to reserve samples!\n");
		return -1;
	}
	if(!al_install_joystick()){
		fprintf(stderr, "failed to initialize joystick!\n");
		return -1;
	}
	
	//Janelas Allegro
	ALLEGRO_DISPLAY *display = al_create_display(SCREEN_W, SCREEN_H);
	ALLEGRO_TIMER *timer = al_create_timer(1.0/FPS);
	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();

	//carrega imagens e sons
	ALLEGRO_BITMAP *mainCharacter = al_load_bitmap("resources/imgP1Up1.bmp");
	ALLEGRO_BITMAP *mainCharacterUp = al_load_bitmap("resources/imgP1Up1.bmp");
	ALLEGRO_BITMAP *mainCharacterDown = al_load_bitmap("resources/imgP1Down1.bmp");
	ALLEGRO_BITMAP *mainCharacterLeft = al_load_bitmap("resources/imgP1Left1.bmp");
	ALLEGRO_BITMAP *mainCharacterRight = al_load_bitmap("resources/imgP1Right1.bmp");
	//
	ALLEGRO_BITMAP *arrowBmp = al_load_bitmap("resources/arrow.bmp");
	ALLEGRO_BITMAP *obstaculos = al_load_bitmap("resources/enemyAuto.bmp");
	ALLEGRO_BITMAP *enemy = al_load_bitmap("resources/imgP2Down1.bmp");
	ALLEGRO_BITMAP *enemyCharacterUp = al_load_bitmap("resources/imgP2Up1.bmp");
	ALLEGRO_BITMAP *enemyCharacterDown = al_load_bitmap("resources/imgP2Down1.bmp");
	ALLEGRO_BITMAP *enemyCharacterLeft = al_load_bitmap("resources/imgP2Left1.bmp");
	ALLEGRO_BITMAP *enemyCharacterRight = al_load_bitmap("resources/imgP2Right1.bmp");
	//emblemas
	ALLEGRO_BITMAP *emb0 = al_load_bitmap("resources/emb0.bmp");
	ALLEGRO_BITMAP *emb1 = al_load_bitmap("resources/emb1.bmp");
	ALLEGRO_BITMAP *emb2 = al_load_bitmap("resources/emb2.bmp");
	ALLEGRO_BITMAP *emb3 = al_load_bitmap("resources/emb3.bmp");
	ALLEGRO_BITMAP *emb4 = al_load_bitmap("resources/emb4.bmp");
	ALLEGRO_BITMAP *emb5 = al_load_bitmap("resources/emb5.bmp");
	
	ALLEGRO_BITMAP *heart = al_load_bitmap("resources/heart.bmp");
	ALLEGRO_BITMAP *sword = al_load_bitmap("resources/attack.bmp");
	ALLEGRO_BITMAP *map2 = al_load_bitmap("resources/mapaNovo.jpg");
	ALLEGRO_FONT *font_roboto = al_load_font("resources/Gameplay.ttf", 72, 0);
	ALLEGRO_SAMPLE *steps = al_load_sample("resources/zeldasteps.wav");
	ALLEGRO_SAMPLE *theme = al_load_sample("resources/zeldatheme.wav");
	ALLEGRO_SAMPLE_ID stepsid;


	//Verifica se foi criado corretamente
	if(!display){
		fprintf(stderr, "Failed to create display!");
		return -1;
	}
	if(!timer){
		fprintf(stderr, "Failed to create timer!");
		al_destroy_display(display);
		return -1;
	}
	if(!event_queue){
		fprintf(stderr, "Failed to create event queue!");
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}
	if(!mainCharacter){
		fprintf(stderr, "Failed to load mainCharacter bitmap!");
		al_destroy_display(display);
		al_destroy_timer(timer);
		al_destroy_event_queue(event_queue);
		return -1;
	}
	if(!font_roboto){
		fprintf(stderr, "Failed to load roboto font!");
		al_destroy_display(display);
		al_destroy_timer(timer);
		al_destroy_event_queue(event_queue);
		al_destroy_bitmap(mainCharacter);
		return -1;
	}
	if(!enemy){
		fprintf(stderr, "Failed to load enemy bitmap!");
		al_destroy_display(display);
		al_destroy_timer(timer);
		al_destroy_event_queue(event_queue);
		al_destroy_bitmap(mainCharacter);
		al_destroy_font(font_roboto);
		return -1;
	}
	if(!arrowBmp){
		fprintf(stderr, "Failed to load arrow bitmap!");
		al_destroy_display(display);
		al_destroy_timer(timer);
		al_destroy_event_queue(event_queue);
		al_destroy_bitmap(mainCharacter);
		al_destroy_font(font_roboto);
		al_destroy_bitmap(enemy);
		return -1;
	}
	if(!heart){
		fprintf(stderr, "Failed to load heart bitmap!");
		al_destroy_display(display);
		al_destroy_timer(timer);
		al_destroy_event_queue(event_queue);
		al_destroy_bitmap(mainCharacter);
		al_destroy_font(font_roboto);
		al_destroy_bitmap(enemy);
		return -1;
	}
	if(!sword){
		fprintf(stderr, "Failed to load sword bitmap!");
		al_destroy_display(display);
		al_destroy_timer(timer);
		al_destroy_event_queue(event_queue);
		al_destroy_bitmap(mainCharacter);
		al_destroy_font(font_roboto);
		al_destroy_bitmap(enemy);
		al_destroy_bitmap(heart);
		return -1;
	}
	if(!steps){
		fprintf(stderr, "Failed to load steps sample!");
		al_destroy_display(display);
		al_destroy_timer(timer);
		al_destroy_event_queue(event_queue);
		al_destroy_bitmap(mainCharacter);
		al_destroy_font(font_roboto);
		al_destroy_bitmap(enemy);
		al_destroy_bitmap(heart);
		al_destroy_bitmap(sword);
		return -1;
	}
	if(!theme){
		fprintf(stderr, "Failed to load theme sample!");
		al_destroy_display(display);
		al_destroy_timer(timer);
		al_destroy_event_queue(event_queue);
		al_destroy_bitmap(mainCharacter);
		al_destroy_font(font_roboto);
		al_destroy_bitmap(enemy);
		al_destroy_bitmap(heart);
		al_destroy_bitmap(sword);
		al_destroy_sample(steps);
		return -1;
	}
	
	if(!map2){
		fprintf(stderr, "Failed to load map2 bitmap!");
		al_destroy_display(display);
		al_destroy_timer(timer);
		al_destroy_event_queue(event_queue);
		al_destroy_bitmap(mainCharacter);
		al_destroy_font(font_roboto);
		al_destroy_bitmap(enemy);
		al_destroy_bitmap(heart);
		al_destroy_bitmap(sword);
		al_destroy_sample(steps);
		al_destroy_sample(theme);
		return -1;
	}

	//Extra prep
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_joystick_event_source());

	al_clear_to_color(al_map_rgb(0,0,0));
	al_flip_display();

	al_start_timer(timer);

	al_play_sample(theme, 0.2, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);

	//Jogadores
	creature player1 = {.movx = 4.0, .movy = 4.0, .lives = 3, .invulnerable = false};
	creature player2 = {.movx = 4.0, .movy = 4.0, .lives = 3, .invulnerable = false};

	//emblemas
	Emblema emblema0 = {posicoesEscolhidas[0][0], posicoesEscolhidas[0][1], true};
	Emblema emblema1 = {posicoesEscolhidas[1][0], posicoesEscolhidas[1][1], false};
	Emblema emblema2 = {posicoesEscolhidas[2][0], posicoesEscolhidas[2][1], false};
	Emblema emblema3 = {posicoesEscolhidas[3][0], posicoesEscolhidas[3][1], false};
	Emblema emblema4 = {posicoesEscolhidas[4][0], posicoesEscolhidas[4][1], false};
	Emblema emblema5 = {posicoesEscolhidas[5][0], posicoesEscolhidas[5][1], false};	

	creature enemy1 = {.lives = 1, .invulnerable = false};
	creature enemy2 = {.lives = 1, .invulnerable = false};
	creature enemy3 = {.lives = 1, .invulnerable = false};
    creature arrow = {.lives = 1, .invulnerable = true};
    
	mapa mapaAtual;

	switch(Menu(display)){
		case PLAY:
			break;
		case EXIT:
			exit = true;
			break;
		default:
			return -1;
	}

	//Actual game
	while(!exit){
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if(changeMap){
			changeMap = false;

			fp = fopen("maps/mapa2.bin", "rb");
			
			fread(&mapaAtual, sizeof(mapaAtual), 1, fp);
			fread(&player1, sizeof(player1), 1, fp);
			fread(&player2, sizeof(player2), 1, fp);
			//
			fread(&enemy1, sizeof(enemy1), 1, fp);
			fread(&enemy2, sizeof(enemy2), 1, fp);
			fread(&enemy3, sizeof(enemy3), 1, fp);
			fread(&arrow, sizeof(arrow), 1, fp);
			fclose(fp);
		}

		if(ev.type == ALLEGRO_EVENT_TIMER){
			reDesenhar = true;

			//objetos para atraplar o player 1
			enemy1.x += enemy1.movx;
			enemy1.y += enemy1.movy;
			if(CreatureCollision(&player1, &enemy1)){
				enemy1.x -= enemy1.movx;
				enemy1.y -= enemy1.movy;
				
				if(!player1.invulnerable){
					player1.lives--;
					player1.invulnerable = true;
				}
			}
			if(MapCollision(&mapaAtual, &enemy1)){
				enemy1.movx = -enemy1.movx;
				enemy1.movy = -enemy1.movy;
			}

			enemy2.x += enemy2.movx;
			enemy2.y += enemy2.movy;
			if(CreatureCollision(&player1, &enemy2)){
				enemy2.x -= enemy2.movx;
				enemy2.y -= enemy2.movy;
				
				if(!player1.invulnerable){
					player1.lives--;
					player1.invulnerable = true;
				}
			}
			if(MapCollision(&mapaAtual, &enemy2)){
				enemy2.movx = -enemy2.movx;
				enemy2.movy = -enemy2.movy;
			}

			enemy3.x += enemy3.movx;
			enemy3.y += enemy3.movy;
			if(CreatureCollision(&player1, &enemy3)){
				enemy3.x -= enemy3.movx;
				enemy3.y -= enemy3.movy;
				
				if(!player1.invulnerable){
					player1.lives--;
					player1.invulnerable = true;
				}
			}
			if(MapCollision(&mapaAtual, &enemy3)){
				enemy3.movx = -enemy3.movx;
				enemy3.movy = -enemy3.movy;
			}

			arrow.x += arrow.movx;
			arrow.y += arrow.movy;
			if(Collision(player1.x, player1.y, GRID, GRID, arrow.x, arrow.y, GRID, 30) && !player1.invulnerable){
					player1.lives--;
					player1.invulnerable = true;
			}


			//colisao do p1 e p2
			if(CreatureCollision(&player1, &player2)){
				player2.x -= player2.movx;
				player2.y -= player2.movy;
				
				if(!player1.invulnerable){
					player1.lives--;
					player1.invulnerable = true;
				}
			}

			//Movimentos Player1
            if(keysP1[KEY_UP]){
                player1.y -= player1.movy;
				mainCharacter = mainCharacterUp;
                dir[UP] = true;
            }else if(keysP1[KEY_DOWN]){
                player1.y += player1.movy;
				mainCharacter = mainCharacterDown;
                dir[DOWN] = true;
            }else if(keysP1[KEY_LEFT]){
                player1.x -= player1.movx;
				mainCharacter = mainCharacterLeft;
                dir[LEFT] = true;
            }else if(keysP1[KEY_RIGHT]){
                player1.x += player1.movx;
				mainCharacter = mainCharacterRight;
                dir[RIGHT] = true;
            }

			//movimentos do Player2
			if(keysP2[KEY_UP]){
                player2.y -= player2.movy;
				enemy = enemyCharacterUp;
                dir[UP] = true;
            }else if(keysP2[KEY_DOWN]){
                player2.y += player2.movy;
				enemy = enemyCharacterDown;
                dir[DOWN] = true;
            }else if(keysP2[KEY_LEFT]){
                player2.x -= player2.movx;
				enemy = enemyCharacterLeft;
                dir[LEFT] = true;
            }else if(keysP2[KEY_RIGHT]){
                player2.x += player2.movx;
				enemy = enemyCharacterRight;
                dir[RIGHT] = true;
            }


			//Colisao do Player 1
			if(MapCollision(&mapaAtual, &player1) || CreatureCollision(&player1, &enemy1)
			|| CreatureCollision(&player1, &enemy2) || CreatureCollision(&player1, &enemy3)){
				if(dir[UP]){
					player1.y += player1.movy;
				}else if(dir[DOWN]){
					player1.y -= player1.movy;
				}else if(dir[RIGHT]){
					player1.x -= player1.movx;
				}else if(dir[LEFT]){
					player1.x += player1.movx;
				}
			}

			//Colisao do Player 2
			if(MapCollision(&mapaAtual, &player2)){
				if(dir[UP]){
					player2.y += player2.movy;
				}else if(dir[DOWN]){
					player2.y -= player2.movy;
				}else if(dir[RIGHT]){
					player2.x -= player2.movx;
				}else if(dir[LEFT]){
					player2.x += player2.movx;
				}
			}

			//caputura emblemas
			if(emblema0.visible==true && Collision(player1.x, player1.y, GRID, GRID, (float) emblema0.x, (float) emblema0.y, GRID, GRID)){
				emblema0.visible = false;
				emblema1.visible = true;	
			}
			if(emblema1.visible==true && Collision(player1.x, player1.y, GRID, GRID, (float) emblema1.x, (float) emblema1.y, GRID, GRID)){
				emblema1.visible = false;
				emblema2.visible = true;	
			}
			if(emblema2.visible==true && Collision(player1.x, player1.y, GRID, GRID, (float) emblema2.x, (float) emblema2.y, GRID, GRID)){
				emblema2.visible = false;
				emblema3.visible = true;	
			}
			if(emblema3.visible==true && Collision(player1.x, player1.y, GRID, GRID, (float) emblema3.x, (float) emblema3.y, GRID, GRID)){
				emblema3.visible = false;
				emblema4.visible = true;	
			}
			if(emblema4.visible==true && Collision(player1.x, player1.y, GRID, GRID, (float) emblema4.x, (float) emblema4.y, GRID, GRID)){
				emblema4.visible = false;	
				emblema5.visible = true;
			}
			if(emblema5.visible==true && Collision(player1.x, player1.y, GRID, GRID, (float) emblema5.x, (float) emblema5.y, GRID, GRID)){
				emblema5.visible = false;
				mapFlag = MAPA_VAZIO;
				changeMap = true;	
			}



			//Attack do player1
			if(keysP1[KEY_ATK]){
				
				if(Collision(player1.x-GRID, player1.y-GRID, 3*GRID, 3*GRID,
							 enemy1.x, enemy1.y, GRID, GRID)){
					enemy1.lives--;
				}
				if(Collision(player1.x-GRID, player1.y-GRID, 3*GRID, 3*GRID,
							 enemy2.x, enemy2.y, GRID, GRID)){
					enemy2.lives--;
				}
				if(Collision(player1.x-GRID, player1.y-GRID, 3*GRID, 3*GRID,
							 enemy3.x, enemy3.y, GRID, GRID)){
					enemy3.lives--;
				}

				if(Collision(player1.x-GRID, player1.y-GRID, 3*GRID, 3*GRID,
							 player2.x, player2.y, GRID, GRID)){
					if(!player2.invulnerable){
					player2.lives--;
					player2.invulnerable = true;
				}
				}
			}

			//Attack do player2
			if(keysP2[KEY_ATK]){
				if(Collision(player2.x-GRID, player2.y-GRID, 3*GRID, 3*GRID,
							 player1.x, player1.y, GRID, GRID)){
					if(!player1.invulnerable){
					player1.lives--;
					player1.invulnerable = true;
				}
				}
			}

			//ponto falso do mapa, armadilha da flecha
			if(Collision(player1.x, player1.y, GRID, GRID, GRID*14, GRID*6, GRID*3, GRID*1)){
						arrow.movx = -8.0;
			}

			//Verifica Vidas
			if(player1.lives <= 0){
				al_clear_to_color(al_map_rgb(0,0,0));
				al_draw_text(font_roboto, al_map_rgb(255,255,255), SCREEN_W/2, SCREEN_H/3,
								ALLEGRO_ALIGN_CENTRE, "PLAYER 2 VENCEU.");
				al_flip_display();
				reDesenhar = false;
			}
			if(player2.lives <= 0){
				player2.x = -GRID*2;
				player2.y = -GRID;
				player2.movx = 0;
				player2.movy = 0;
			}

			if(enemy1.lives <= 0){
				enemy1.x = -GRID;
				enemy1.y = -GRID;
				enemy1.movx = 0;
				enemy1.movy = 0;
			}
			if(enemy2.lives <= 0){
				enemy2.x = -GRID*2;
				enemy2.y = -GRID;
				enemy2.movx = 0;
				enemy2.movy = 0;
			}
			if(enemy3.lives <= 0){
				enemy3.x = -GRID*3;
				enemy3.y = -GRID;
				enemy3.movx = 0;
				enemy3.movy = 0;
			}

			
		}
		//keys de direcao do player 1 e 2
		else if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
			switch(ev.keyboard.keycode){
				case ALLEGRO_KEY_W:
				 	keysP1[KEY_UP] = true;
					dir[UP] = true;
					al_play_sample(steps, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, &stepsid);
					break;	
				case ALLEGRO_KEY_UP:
                    keysP2[KEY_UP] = true;
					dir[UP] = true;
					al_play_sample(steps, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, &stepsid);
					break;
				case ALLEGRO_KEY_S:
					keysP1[KEY_DOWN] = true;	
					dir[DOWN] = true;	
					al_play_sample(steps, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, &stepsid);		
					break;
				case ALLEGRO_KEY_DOWN:
                    keysP2[KEY_DOWN] = true;	
					dir[DOWN] = true;	
					al_play_sample(steps, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, &stepsid);		
					break;
				case ALLEGRO_KEY_A:
				    keysP1[KEY_LEFT] = true;	
					dir[LEFT] = true;	
					al_play_sample(steps, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, &stepsid);		
					break;
				case ALLEGRO_KEY_LEFT:
                    keysP2[KEY_LEFT] = true;	
					dir[LEFT] = true;	
					al_play_sample(steps, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, &stepsid);		
					break;
				case ALLEGRO_KEY_D:
				    keysP1[KEY_RIGHT] = true;	
					dir[RIGHT] = true;	
					al_play_sample(steps, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, &stepsid);		
					break;
				case ALLEGRO_KEY_RIGHT:
                    keysP2[KEY_RIGHT] = true;
					dir[RIGHT] = true;
					al_play_sample(steps, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, &stepsid);
					break;
				case ALLEGRO_KEY_SPACE:
                    keysP1[KEY_ATK] = true;
					break;	
				case ALLEGRO_KEY_ESCAPE:
					exit = true;
					break;
			}     
		}
        else if(ev.type == ALLEGRO_EVENT_KEY_UP){
            switch(ev.keyboard.keycode){
				case ALLEGRO_KEY_W:
					keysP1[KEY_UP] = false;
					al_stop_sample(&stepsid);
					break;
				case ALLEGRO_KEY_UP:
                    keysP2[KEY_UP] = false;
					al_stop_sample(&stepsid);
					break;
				case ALLEGRO_KEY_S:
				 	keysP1[KEY_DOWN] = false;
					al_stop_sample(&stepsid);			
					break;
				case ALLEGRO_KEY_DOWN:
                    keysP2[KEY_DOWN] = false;
					al_stop_sample(&stepsid);			
					break;
				case ALLEGRO_KEY_A:
				   keysP1[KEY_LEFT] = false;
					al_stop_sample(&stepsid);				
					break;
				case ALLEGRO_KEY_LEFT:
                    keysP2[KEY_LEFT] = false;
					al_stop_sample(&stepsid);				
					break;
				case ALLEGRO_KEY_D:
			    	keysP1[KEY_RIGHT] = false;
					al_stop_sample(&stepsid);
					break;
				case ALLEGRO_KEY_RIGHT:
                    keysP2[KEY_RIGHT] = false;
					al_stop_sample(&stepsid);
					break;
				case ALLEGRO_KEY_SPACE:
                    keysP1[KEY_ATK] = false;
					break;
            }
			player1.invulnerable = false;
			player2.invulnerable = false;
        }

		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
			exit = true;
		}

		if(reDesenhar && al_is_event_queue_empty(event_queue)){
			reDesenhar = false;


			//Desenha map
			switch(mapFlag){
				case MAPA:
					al_draw_bitmap(map2, 0, 0, 0);
					break;
				case MAPA_VAZIO:
					al_clear_to_color(al_map_rgb(0, 0, 0));
					al_draw_text(font_roboto, al_map_rgb(255, 255, 255), GRID*10.5, GRID*5,
					ALLEGRO_ALIGN_CENTER, "PLAYER 1 VENCEU!");
					reDesenhar = false;
					break;
			}

			
			

			//Desenha Emblemas
			
			if(emblema0.visible==true) {al_draw_bitmap(emb0, (float) emblema0.x, (float) emblema0.y, 0);}
			if(emblema1.visible==true) {al_draw_bitmap(emb1, (float) emblema1.x, (float) emblema1.y, 0);}
			if(emblema2.visible==true) {al_draw_bitmap(emb2, (float) emblema2.x, (float) emblema2.y, 0);}
			if(emblema3.visible==true) {al_draw_bitmap(emb3, (float) emblema3.x, (float) emblema3.y, 0);}
			if(emblema4.visible==true) {al_draw_bitmap(emb4, (float) emblema4.x, (float) emblema4.y, 0);}
			if(emblema5.visible==true) {al_draw_bitmap(emb5, (float) emblema5.x, (float) emblema5.y, 0);}
			

			//Desenha as vidas
			switch(player1.lives){
			case 3:
				al_draw_bitmap(heart, GRID*1+32, GRID*0.5, 0);
			case 2:
				al_draw_bitmap(heart, GRID*1+16, GRID*0.5, 0);
			case 1:
				al_draw_bitmap(heart, GRID*1, GRID*0.5, 0);
				break;
			}

			switch(player2.lives){
			case 3:
				al_draw_bitmap(heart, GRID*19+32, GRID*0.5, 0);
			case 2:
				al_draw_bitmap(heart, GRID*19+16, GRID*0.5, 0);
			case 1:
				al_draw_bitmap(heart, GRID*19, GRID*0.5, 0);
				break;
			}
			
			//Desenha attack
			if(keysP1[KEY_ATK]){
				al_draw_bitmap(sword, player1.x-GRID, player1.y-GRID, 0);
			}

			//Desenha os jogadores
			al_draw_bitmap(mainCharacter, player1.x, player1.y, 0);
			al_draw_bitmap(enemy, player2.x, player2.y, 0);
			//objetos inimigos
			al_draw_bitmap(obstaculos, enemy1.x, enemy1.y, 0);
			al_draw_bitmap(obstaculos, enemy2.x, enemy2.y, 0);
			al_draw_bitmap(obstaculos, enemy3.x, enemy3.y, 0);
			al_draw_bitmap(arrowBmp, arrow.x, arrow.y, 0);
			
			al_flip_display();

			dir[UP] = false;
			dir[DOWN] = false;
			dir[RIGHT] = false;
			dir[LEFT] = false;
		}

	}//fim while game


	al_destroy_display(display);
	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);
	al_destroy_bitmap(mainCharacter);
	al_destroy_bitmap(mainCharacterUp);
	al_destroy_bitmap(mainCharacterDown);
	al_destroy_bitmap(mainCharacterLeft);
	al_destroy_bitmap(mainCharacterRight);
	al_destroy_bitmap(enemy);
	al_destroy_bitmap(arrowBmp);
	al_destroy_bitmap(obstaculos);
	al_destroy_bitmap(enemyCharacterUp);
	al_destroy_bitmap(enemyCharacterDown);
	al_destroy_bitmap(enemyCharacterLeft);
	al_destroy_bitmap(enemyCharacterRight); 
	//
	al_destroy_bitmap(emb0); 
	al_destroy_bitmap(emb1); 
	al_destroy_bitmap(emb2); 
	al_destroy_bitmap(emb3); 
	al_destroy_bitmap(emb4); 
	al_destroy_bitmap(emb5); 
	al_destroy_bitmap(heart);
	al_destroy_bitmap(sword);
	al_destroy_bitmap(map2);
	al_destroy_font(font_roboto);
	al_destroy_sample(steps);
	al_destroy_sample(theme);

	return 0;
}


int Creditos(ALLEGRO_DISPLAY *menuDisplay){
    int voltar = 0; // flag de selecao

    ALLEGRO_BITMAP *creditos = al_load_bitmap("resources/menu/credits.jpg");

    //Config da  fila de eventos
    ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
    fila_eventos = al_create_event_queue();

    if (!fila_eventos){
        fprintf(stderr, "failed to create event queue!\n");
        al_destroy_display(menuDisplay);
        return -1;
    }

    if(!creditos){
        fprintf(stderr, "failed to load creditos bitmap!\n");
        al_destroy_display(menuDisplay);
        al_destroy_event_queue(fila_eventos);
        return -1;
    }

    // Habilita a aplicacao do mouse
    if (!al_install_mouse()){
        fprintf(stderr, "failed to install mouse!\n");
        al_destroy_display(menuDisplay);
        al_destroy_event_queue(fila_eventos);
        al_destroy_bitmap(creditos);
        return -1;
    }

    // Config mouse
    if (!al_set_system_mouse_cursor(menuDisplay, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT)){
        fprintf(stderr, "failed to set cursor!\n");
        al_destroy_display(menuDisplay);
        al_destroy_event_queue(fila_eventos);
        al_destroy_bitmap(creditos);
        return -1;
    }
    // Dizemos que vamos tratar os eventos vindos do mouse
    al_register_event_source(fila_eventos, al_get_mouse_event_source());

    // Desenha a imagem na tela
    al_draw_bitmap(creditos, 0, 0, 0);
    al_flip_display();

    while(voltar != 1){
        voltar = 0;

        // Verificamos se ha eventos na fila
        while (!al_is_event_queue_empty(fila_eventos)){
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_eventos, &evento);

            // Verificamos se  foi um clique do mouse o evento
            if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP && 
                evento.mouse.x >= xVolta_m && evento.mouse.x <= xVolta_M &&
                evento.mouse.y >= yVolta_m && evento.mouse.y <= yVolta_M){
                        
                    voltar = 1;    
            }
        }
    }

    if(voltar == 1){
        Menu(menuDisplay);
    }

    al_destroy_event_queue(fila_eventos);
    al_destroy_bitmap(creditos);
    return 0;
}

int Menu(ALLEGRO_DISPLAY *menuDisplay){
    int select = 0; // flag de selecao

    ALLEGRO_BITMAP *fundo = al_load_bitmap("resources/menu/menuNovo.jpg");

    //Config da  fila de eventos
    ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
    fila_eventos = al_create_event_queue();

    if (!fila_eventos){
        fprintf(stderr, "failed to create event queue!\n");
        al_destroy_display(menuDisplay);
        return ERROR;
    }

    if(!fundo){
        fprintf(stderr, "failed to load fundo bitmap!\n");
        al_destroy_display(menuDisplay);
        al_destroy_event_queue(fila_eventos);
        return ERROR;
    }

    // Habilita a aplicacao do mouse
    if (!al_install_mouse()){
        fprintf(stderr, "failed to install mouse!\n");
        al_destroy_display(menuDisplay);
        al_destroy_event_queue(fila_eventos);
        al_destroy_bitmap(fundo);
        return ERROR;
    }

    // Config mouse
    if (!al_set_system_mouse_cursor(menuDisplay, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT)){
        fprintf(stderr, "failed to set cursor!\n");
        al_destroy_display(menuDisplay);
        al_destroy_event_queue(fila_eventos);
        al_destroy_bitmap(fundo);
        return ERROR;
    }
    // Dizemos que vamos tratar os eventos vindos do mouse
    al_register_event_source(fila_eventos, al_get_mouse_event_source());

    // Desenha a imagem na tela
    al_draw_bitmap(fundo, 0, 0, 0);
    al_flip_display();


    while(!select){
        // Verificamos se ha eventos na fila
        while (!al_is_event_queue_empty(fila_eventos)){
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_eventos, &evento);

            // Verificamos se  foi um clique do mouse o evento
            if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
                if (evento.mouse.x >= xPlay_m && evento.mouse.x <= xPlay_M  //play
                    && evento.mouse.y >= yPlay_m && evento.mouse.y <= yPlay_M){
                        select = 1;

                }else if (evento.mouse.x >= xCred_m && evento.mouse.x <= xCred_M  //creditos
                        && evento.mouse.y >= yCred_m && evento.mouse.y <= yCred_M){
                        select = 3;

                }else if (evento.mouse.x >= xExit_m && evento.mouse.x <= xExit_M  //sair
                        && evento.mouse.y >= yExit_m && evento.mouse.y <= yExit_M){
                        select = 4;
                }
            }
        }
    }

	al_destroy_event_queue(fila_eventos);
	al_destroy_bitmap(fundo);

    switch(select){
        case 1: //Vai para o jogo
			return PLAY;
		case 3: //Chama função dos créditos
			if(Creditos(menuDisplay) == -1)
				return ERROR;
			break;
    	case 4:
			return EXIT;
		default:
			fprintf(stderr, "error in menu function");
			return ERROR;
	}
}
