#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>

#define SCREEN_W 640
#define SCREEN_H 480

//Mapeamento do botao play
#define xPlay_m 232
#define xPlay_M 400
#define yPlay_m 104
#define yPlay_M 177

//Mapeamento do botao load
#define xLoad_m 232
#define xLoad_M 400
#define yLoad_m 185
#define yLoad_M 252

//Mapeamento do botao creditos
#define xCred_m 194
#define xCred_M 432
#define yCred_m 276
#define yCred_M 331

//Mapeamento do botao exit
#define xexit_m 231
#define xexit_M 400
#define yexit_m 344
#define yexit_M 408

//Mapeamento do botao voltar dos creditos
#define xVolta_m 363
#define xVolta_M 627
#define yVolta_m 375
#define yVolta_M 468



void Menu(ALLEGRO_DISPLAY *janela);
int main(){
    al_init();
    al_init_image_addon();
    // Config da janela
    ALLEGRO_DISPLAY *janela = NULL;
    Menu(al_create_display(SCREEN_W, SCREEN_H));
    al_set_window_title(janela, "~Le~ Menu del cao");

    return 0;
}

void Menu(ALLEGRO_DISPLAY *janela){
    int select = 0; // flag de seleção

    //Config da  fila de eventos
    ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
    fila_eventos = al_create_event_queue();

    if (!fila_eventos){
    al_destroy_display(janela);
    return -1;
    }

    // Habilita a aplicação de umas paradas
    if (!al_install_mouse()){
        al_destroy_display(janela);
        return -1;
    }

    // Config mouse
    if (!al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT)){
        al_destroy_display(janela);
        return -1;
    }
    al_register_event_source(fila_eventos, al_get_mouse_event_source());
    // Dizemos que vamos tratar os eventos vindos do mouse


    // Variavel representando o fundo
    ALLEGRO_BITMAP *fundo = NULL;
    fundo = al_load_bitmap("/home/francisco/Desktop/Projects/Games-IP/menu/menu.bmp");

    // Desenha a imagem na tela
    al_draw_bitmap(fundo, 0, 0, 0);

    // Atualiza a tela
    al_flip_display();


    while(select != 1 && select != 2 && select != 3 && select != 4){
        // Verificamos se ha eventos na fila
        while (!al_is_event_queue_empty(fila_eventos)){
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_eventos, &evento);

            // Verificamos se  foi um clique do mouse o evento
            if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
                if (evento.mouse.x >= xPlay_m && evento.mouse.x <= xPlay_M  //Selecao do play
                    && evento.mouse.y >= yPlay_m && evento.mouse.y <= yPlay_M){

                        select = 1;

                }else if (evento.mouse.x >= xLoad_m && evento.mouse.x <= xLoad_M  //Selecao do load
                        && evento.mouse.y >= yLoad_m && evento.mouse.y <= yLoad_M){

                        select = 2;

                }else if (evento.mouse.x >= xCred_m && evento.mouse.x <= xCred_M  //Selecao dos creditos
                        && evento.mouse.y >= yCred_m && evento.mouse.y <= yCred_M){

                        select = 3;

                }else if (evento.mouse.x >= xexit_m && evento.mouse.x <= xexit_M  //Selecao dos creditos
                        && evento.mouse.y >= yexit_m && evento.mouse.y <= yexit_M){

                        select = 4;
                }
            }
        }
    }

    if(select == 1){
    al_destroy_display(janela);

    }else if (select == 2 ){

       // Variavel representando o fundo
    ALLEGRO_BITMAP *fundo = NULL;
    fundo = al_load_bitmap("/home/francisco/Desktop/Projects/Games-IP/menu/menu.bmp");

    // Desenha a imagem na tela
    al_draw_bitmap(fundo, 0, 0, 0);

    // Atualiza a tela
    al_flip_display();


    al_rest(10.0);

    }else if (select == 3 ){
          al_destroy_display(janela);
        Creditos(al_create_display(SCREEN_W, SCREEN_H)); // chama funcao dos creditos

    }else if (select == 4 ){
        al_destroy_display(janela);


    }

}

void Creditos(ALLEGRO_DISPLAY *janela){
    int voltar = 0; // flag de selecao

    //Config da  fila de eventos
    ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
    fila_eventos = al_create_event_queue();

    // Habilita a aplicacao de umas paradas
    if (!al_install_mouse()){
        return -1;
    }

    // Config mouse
    if (!al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT)){
        al_destroy_display(janela);
        return -1;
    }
    al_register_event_source(fila_eventos, al_get_mouse_event_source());
    // Dizemos que vamos tratar os eventos vindos do mouse

    // Variavel representando o fundo dos creditos
    ALLEGRO_BITMAP *creditos = NULL;
    creditos = al_load_bitmap("/home/francisco/Desktop/Projects/Games-IP/menu/credito.bmp");

    // Desenha a imagem na tela
    al_draw_bitmap(creditos, 0, 0, 0);

    // Atualiza a tela
    al_flip_display();

    while(voltar != 1){
        voltar = 0;// declarada la em cima

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
        al_destroy_display(janela);
        Menu(al_create_display(SCREEN_W, SCREEN_H));
    }
}
