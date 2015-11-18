#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <cstdlib>
#include <ctime>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "personagens.hpp"


//using namespace std;

// -------- VARIÁVEIS GLOBAIS --------
const int LARGURA_T = 1024;
const int ALTURA_T = 768;
const int FPS = 60;
const int NUM_BALAS = 5;
const int NUM_INIMIGOS = 10;
const int numSprites = 4;

enum TECLAS {CIMA, BAIXO, ESQUERDA, DIREITA, ESPACO};

// ___________________________________

// ------------------ PROTÓTIPOS ------------------
void InitPersonagem (Personagem &personagem_principal);
void MovePersonagemCima (Personagem &personagem_principal);
void MovePersonagemBaixo (Personagem &personagem_principal);
void MovePersonagemEsquerda (Personagem &personagem_principal);
void MovePersonagemDireita (Personagem &personagem_principal);

void InitBalas (Projetil balas[], int tamanho);
void AtiraBalas (Projetil balas[], int tamanho, Personagem personagem_principal);
void AtualizaBalas (Projetil balas[], int tamanho);
void DesenhaBalas (Projetil balas[], int tamanho);
void BalaColidida (Projetil balas[], int b_tamanho, Inimigo inimigos[], int i_tamanho);

void InitInimigo (Inimigo inimigos[], int tamanho);
void GeraInimigos (Inimigo inimigos[], int tamanho);
void AtualizaInimigos (Inimigo inimigos[], int tamanho);
void DesenhaInimigos (Inimigo inimigos[], int tamanho);


// ________________________________________________




int main(int argc, char *argv[])
{

// -------- VARIÁVEIS DO JOGO --------
    ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
    ALLEGRO_TIMER *timer = NULL;
    bool desenha = false;
    bool fim = false;
	bool render = false;
	bool direita = true;
	bool parado = true;
	bool ataque = false;
	float gameTime = 0;
	int frames = 0;
	int gameFPS = 0;
    int i = 0;

// __________________________________________

// -------- INICIALIZAÇÃO DA ALLEGRO E DO DISPLAY --------
    ALLEGRO_DISPLAY *display = NULL;

    if (!al_init())
    {
        al_show_native_message_box(NULL, "AVISO!", "ERRO!", "ERRO AO INICIALIZAR A ALLEGRO!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    display = al_create_display(LARGURA_T, ALTURA_T);

    if (!display)
    {
        al_show_native_message_box(NULL, "AVISO!", "ERRO!", "ERRO AO CRIAR O DISPLAY!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }



// ____________________________________________________

// -------- INICIALIZAÇÃO DE ADDONS E INSTALAÇÕES --------
    al_init_primitives_addon();
    al_install_keyboard();
    al_init_image_addon();

	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_FONT *font18 = NULL;
	ALLEGRO_BITMAP *imageR;
	ALLEGRO_BITMAP *imageL;
	ALLEGRO_BITMAP *imageER;
	ALLEGRO_BITMAP *imageEL;
	ALLEGRO_BITMAP *imageSR;
	ALLEGRO_BITMAP *imageSL;


// -------- INICIALIZAÇÃO DE OBJETOS --------

    Personagem personagem_principal;
    Projetil balas[NUM_BALAS];
    Inimigo* inimigos = new Inimigo[NUM_INIMIGOS];
    Sprite sprites[numSprites];

    imageR = al_load_bitmap("MoveR.png");
    imageL = al_load_bitmap("MoveL.png");
    imageER = al_load_bitmap("EspadaR.png");
    imageEL = al_load_bitmap("EspadaL.png");
    imageSR = al_load_bitmap("StandR.png");
    imageSL = al_load_bitmap("StandL.png");

    al_convert_mask_to_alpha(imageR, al_map_rgb(96, 80, 0));
    al_convert_mask_to_alpha(imageL, al_map_rgb(96, 80, 0));
    al_convert_mask_to_alpha(imageER, al_map_rgb(96, 80, 0));
    al_convert_mask_to_alpha(imageEL, al_map_rgb(96, 80, 0));


    InitSprites(sprites[0], imageR);
    InitSprites(sprites[1], imageL);
    InitSprites(sprites[2], imageER);
    InitSprites(sprites[3], imageEL);
    InitSprites(sprites[4], imageSR);
    InitSprites(sprites[5], imageSL);




    //Atacando para direita
    sprites[2].frameDelay = 8;
    sprites[2].frameWidth = 138;
	sprites[2].frameHeight = 96;
	sprites[2].x = 200;
	sprites[2].y = 490;

    //Atacando para esquerda
    sprites[3].frameWidth = 150;
	sprites[3].frameHeight = 96;
	sprites[3].x = 200;
	sprites[3].y = 490;

    // Parado para direita
    sprites[4].maxFrame = 1;
    sprites[4].frameWidth = 101;
	sprites[4].frameHeight = 75;

	// Parado para esquerda
    sprites[5].maxFrame = 1;
    sprites[5].frameWidth = 101;
	sprites[5].frameHeight = 75;

// _______________________________________________________

// -------- CRIAÇÃO DE FILAS E DEMAIS DISPOSITIVOS --------
    fila_eventos = al_create_event_queue();
    timer = al_create_timer(1.0 / FPS);
	font18 = al_load_font("arial.ttf", 18, 0);

// ________________________________________________________

// -------- REGISTRO DE SOURCES --------
    al_register_event_source(fila_eventos, al_get_display_event_source(display));
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));

// _____________________________________

// -------- FUNÇÕES INICIAIS --------
    srand(time(NULL));
    InitPersonagem(personagem_principal);
    InitBalas(balas, NUM_BALAS);
    InitInimigo(inimigos, NUM_INIMIGOS);
// __________________________________

// ----------------- LOOP PRINCIPAL -----------------
	al_start_timer(timer);
	gameTime = al_current_time();
    while(!fim)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(fila_eventos, &ev);

        // -------- EVENTOS E LÓGICA DO JOGO --------
        if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            fim = true;
        }

       if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch(ev.keyboard.keycode)
			{
                case ALLEGRO_KEY_ESCAPE:
                    fim = true;
                    break;
                case ALLEGRO_KEY_A:{
                    ataque = true;
                    break;
                }
                case ALLEGRO_KEY_LEFT:{
                    for (i=0 ; i < numSprites ; i++){
                        sprites[i].velX = 5;
                        sprites[i].dirX = -1;
                    }
                    direita = false;
                    parado = false;
                    break;
                }
                case ALLEGRO_KEY_RIGHT:{
                    for (i=0 ; i < numSprites ; i++){
                        sprites[i].velX = 5;
                        sprites[i].dirX = 1;
                    }
                    direita = true;
                    parado = false;
                    break;
                }
                case ALLEGRO_KEY_UP:{
                    for (i=0 ; i < numSprites ; i++){
                        sprites[i].velY = 5;
                        sprites[i].dirY = -1;
                    }
                    parado = false;
                    break;
                }
                case ALLEGRO_KEY_DOWN:{
                    for (i=0 ; i < numSprites ; i++){
                        sprites[i].velY = 5;
                        sprites[i].dirY = 1;
                    }
                    parado = false;
                    break;
                }
            }
		}
        else if (ev.type == ALLEGRO_EVENT_KEY_UP)
        {
            switch(ev.keyboard.keycode)
            {
                case ALLEGRO_KEY_A:{
                    ataque = false;
                    break;
                }
                case ALLEGRO_KEY_UP:{
                    for (i=0 ; i < numSprites ; i++){
                        sprites[i].velY = 0;
                        sprites[i].dirY = 1;
                        }
                    parado = true;
                    break;
                }
                case ALLEGRO_KEY_DOWN:{
                    for (i=0 ; i < numSprites ; i++){
                        sprites[i].velY = 0;
                        sprites[i].dirY = 1;
                        }
                    parado = true;
                    break;
                }
                case ALLEGRO_KEY_LEFT:{
                        for (i=0 ; i < numSprites ; i++){
                        sprites[i].velX = 0;
                        sprites[i].dirX = 1;
                        }
                    direita = false;
                    parado = true;
                    break;
                    }
                case ALLEGRO_KEY_RIGHT:
                {
                    for (i=0 ; i < numSprites ; i++){
                        sprites[i].velX = 0;
                        sprites[i].dirX = 1;
                        }
                    direita = true;
                    parado = true;
                    break;
                }
            }
        }
        else if (ev.type == ALLEGRO_EVENT_TIMER)
        {
            frames++;
			if(al_current_time() - gameTime >= 1)
			{
				gameTime = al_current_time();
				gameFPS = frames;
				frames = 0;
			}
            //if(!parado){
                for(int i = 0; i < numSprites; i++)
                    UpdateSprites(sprites[i]);
            //}
			render = true;

            desenha = true;

        }
        if(render && al_is_event_queue_empty(fila_eventos))
		{
			render = false;
			if (ataque){
                if(direita){
                    DrawSprites(sprites[2]);
                    //UpdateSprites(sprites[2]);
                }
                else{
                    for(i=0 ; i < sprites[3].frameCount ; i++)
                    DrawSprites(sprites[3]);
                    //UpdateSprites(sprites[3]);
                }
			}
			else{
            if(direita)
				DrawSprites(sprites[0]);
            else
                DrawSprites(sprites[1]);
			}
			//al_draw_textf(font18, al_map_rgb(255, 0, 255), 5, 5, 0, "FPS: %i", gameFPS);

			al_flip_display();
			al_clear_to_color(al_map_rgb(0,0,0));
		}


        // _________________________________________

        // ---------------- DESENHO ----------------

        /*if(desenha && al_is_event_queue_empty(fila_eventos))
        {
            desenha = false;
            al_clear_to_color(al_map_rgb(0, 0, 0));

            personagem_principal.bmp = al_load_bitmap("ship.png");
            for (i = 0; i < NUM_BALAS; i++)
                balas[i].bmp = al_load_bitmap("bala.png");

            for (i = 0; i < NUM_INIMIGOS; i++)
                inimigos[i].bmp = al_load_bitmap("enemyRed.png");


            al_draw_bitmap(personagem_principal.bmp, personagem_principal.x, personagem_principal.y, 0); //Desenha nave
            DesenhaBalas(balas, NUM_BALAS);
            DesenhaInimigos(inimigos, NUM_INIMIGOS);

            al_flip_display();
            al_clear_to_color(al_map_rgb(0, 0, 0));

        }*/


        // _________________________________________
    }

// _________________________________________________

// -------- FINALIZAÇÕES DO PROGRAMA --------
    al_destroy_display(display);
    al_destroy_event_queue(fila_eventos);
    al_destroy_timer(timer);
    delete[] inimigos;
	al_destroy_bitmap(imageEL);
	al_destroy_bitmap(imageER);
	al_destroy_bitmap(imageR);
	al_destroy_bitmap(imageL);
	al_destroy_bitmap(imageSL);
	al_destroy_bitmap(imageSR);
//___________________________________________
    return 0;
}

// ----------- DEFINIÇÃO DE FUNÇÕES -----------

// ------------- Personagem -------------
void InitPersonagem (Personagem &personagem_principal)
{
    personagem_principal.x = 20;
    personagem_principal.y = ALTURA_T / 2;
    personagem_principal.ID = JOGADOR;
    personagem_principal.vida = 3;
    personagem_principal.velocidade = 7;
    personagem_principal.borda_x = 6;
    personagem_principal.borda_y = 7;
    personagem_principal.bmp = NULL;
    //personagem_principal.dano = 5;
    //personagem_principal.pontos = 0;
}

void DesenhaPersonagem (Personagem personagem_principal)
{
    al_draw_bitmap(personagem_principal.bmp, personagem_principal.x, personagem_principal.y, 0);
}

void MovePersonagemCima (Personagem &personagem_principal)
{
    personagem_principal.y -= personagem_principal.velocidade;

    if (personagem_principal.y < 9)
        personagem_principal.y = 9;

    al_draw_bitmap(personagem_principal.bmp, personagem_principal.x, personagem_principal.y, 0);

}
void MovePersonagemBaixo (Personagem &personagem_principal)
{
    personagem_principal.y += personagem_principal.velocidade;

    if (personagem_principal.y > ALTURA_T-15)
        personagem_principal.y = ALTURA_T-15;

    al_draw_bitmap(personagem_principal.bmp, personagem_principal.x, personagem_principal.y, 0);

}
void MovePersonagemEsquerda (Personagem &personagem_principal)
{
    personagem_principal.x -= personagem_principal.velocidade;

    if (personagem_principal.x < 0)
        personagem_principal.x = 0;

    al_draw_bitmap(personagem_principal.bmp, personagem_principal.x, personagem_principal.y, 0);

}
void MovePersonagemDireita (Personagem &personagem_principal)
{
    personagem_principal.x += personagem_principal.velocidade;

    if (personagem_principal.x > LARGURA_T-15)
        personagem_principal.x = LARGURA_T-15;

    al_draw_bitmap(personagem_principal.bmp, personagem_principal.x, personagem_principal.y, 0);

}

// ____________________________________________

// -------- Projeteis --------
void InitBalas (Projetil balas[], int tamanho)
{
    int i;
    for (i = 0; i < tamanho; i++)
    {
        balas[i].ID = PROJETIL;
        balas[i].velocidade = 10;
        balas[i].ativo = false;
        balas[i].bmp = NULL;
    }
}
void AtiraBalas (Projetil balas[], int tamanho, Personagem personagem_principal)
{
    int i;
    for (i = 0; i < tamanho; i++)
    {
        if (!balas[i].ativo)
        {
            balas[i].x = personagem_principal.x + 45;
            balas[i].y = personagem_principal.y - 20;
            balas[i].ativo = true;
            break;
        }
    }
}
void AtualizaBalas (Projetil balas[], int tamanho)
{
    int i;
    for (i = 0; i < tamanho; i++)
    {
        if (balas[i].ativo)
        {
            balas[i].x += balas[i].velocidade;

            if (balas[i].x > LARGURA_T)
                balas[i].ativo = false;
        }
    }
}
void DesenhaBalas (Projetil balas[], int tamanho)
{
    int i;
    for (i = 0; i < tamanho; i++)
    {
        if (balas[i].ativo)
            al_draw_bitmap(balas[i].bmp, balas[i].x, balas[i].y, 0);
    }
}
void BalaColidida (Projetil balas[], int b_tamanho, Inimigo inimigos[], int i_tamanho)
{
    int i, j;
    for (i = 0; i < b_tamanho; i++)
    {
        if (balas[i].ativo)
        {
            for (j = 0; j < i_tamanho; j++)
            {
                if (inimigos[j].ativo)
                {
                    if (balas[i].x > (inimigos[j].x) && balas[i].x < (inimigos[j].x + inimigos[j].borda_x) && balas[i].y > (inimigos[j].y-10) && balas[i].y < (inimigos[j].y + inimigos[j].borda_y))
                    {
                        balas[i].ativo = false;
                        // mas como inimigo desaparece???
                        inimigos[j].ativo = false;
                    }
                }
            }
        }
    }
}

// _________________________________

// -------------- Inimigos --------------
void InitInimigo (Inimigo inimigos[], int tamanho)
{
    int i;
    for (i = 0; i < tamanho; i++)
    {
        inimigos[i].ID = INIMIGOS;
        inimigos[i].velocidade = 5;
        inimigos[i].borda_x = 77;
        inimigos[i].borda_y = 71;
        inimigos[i].ativo = false;
        inimigos[i].bmp = NULL;
    }
}
void GeraInimigos (Inimigo inimigos[], int tamanho)
{
    int i;
    for (i = 0; i < tamanho; i++)
    {
        if(!inimigos[i].ativo)
        {
            if(rand() % 500 == 0) //Geração aleatória de inimigos = 1 em 500.
            {
                inimigos[i].x = LARGURA_T;
                inimigos[i].y = 30 + rand() % (ALTURA_T - 60); //Expressão que fornence um número dentro da tela do jogo.
                inimigos[i].ativo = true;
                break;
            }
        }
    }
}
void AtualizaInimigos (Inimigo inimigos[], int tamanho)
{
    int i;
    for (i = 0; i < tamanho; i++)
    {
        if (inimigos[i].ativo)
        {
            inimigos[i].x -= inimigos[i].velocidade;

            if (inimigos[i].x < 0)
            {
                inimigos[i].ativo = false;
            }
        }
    }
}
void DesenhaInimigos (Inimigo inimigos[], int tamanho)
{
    int i;
    for (i = 0; i < tamanho; i++)
    {
        if (inimigos[i].ativo)
        {
            al_draw_bitmap(inimigos[i].bmp, inimigos[i].x, inimigos[i].y, 0);
        }

    }
}
//_________________________________________

//_____________________________________________
