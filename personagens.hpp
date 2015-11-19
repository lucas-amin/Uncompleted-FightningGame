#ifndef PERSONAGENS_HPP_INCLUDED
#define PERSONAGENS_HPP_INCLUDED
#define width 1024
#define height 768

enum IDS {JOGADOR, PROJETIL, INIMIGOS};

class Personagem
{
public:
    ALLEGRO_BITMAP *bmp;
    int ID;
    int x;
    int y;
    int velocidade;
    int vida;
    int borda_x;
    int borda_y;
};

class Projetil
{
public:
    ALLEGRO_BITMAP *bmp;
    int ID;
    int x;
    int y;
    int velocidade;
    bool ativo;
};

class Inimigo
{
public:
    ALLEGRO_BITMAP *bmp;
    int ID;
    int x;
    int y;
    int velocidade;
    int borda_x;
    int borda_y;
    bool ativo;
};

class Sprite
{
public:
	float x;
	float y;
	float velX;
	float velY;
	int dirX;
	int dirY;

	int maxFrame;
	int curFrame;
	int frameCount;
	int frameDelay;
	int frameWidth;
	int frameHeight;
	int animationColumns;
	int animationDirection;

	ALLEGRO_BITMAP *image;


    void initSprites(ALLEGRO_BITMAP *imageo)
    {
        x = 220;
        y = 500;
        velX = 0;
        velY = 0;
        dirX = -1;
        dirY = -1;

        maxFrame = 4;
        curFrame = 0;
        frameCount = 0;
        frameDelay = 8;
        frameWidth = 74;
        frameHeight = 99;
        animationColumns = 16;
        animationDirection = 1;

        image = imageo;
    }


    void UpdateSprites()
    {
        if(++frameCount >= frameDelay)
        {
            curFrame += animationDirection;
            if(curFrame >= maxFrame)
                curFrame = 0;
            else if(curFrame <= 0)
                curFrame = maxFrame - 1;
            frameCount = 0;
        }

        x += velX * dirX;
        y += velY * dirY;

        if((x <= 0 && dirX == -1) ||
            (x >= width - frameWidth && dirX == 1))
        {
            if (dirX == -1){
                x += 5;
                velX = 0;
            }
            if (dirX == 1){
                x += -5;
                velX = 0;
            }
        }
        if((y <= 0 && dirY == -1) ||
            (y >= height - frameHeight && dirY == 1))
        {
            if (dirY == -1){
                y += 5;
                velY = 0;
            }
            if (dirY == 1){
                y += -5;
                velY = 0;
            }
        }
    }

    void DrawSprites()
    {
        int fx = (curFrame % animationColumns) * frameWidth;
        int fy = (curFrame / animationColumns) * frameHeight;

        al_draw_bitmap_region(image, fx, fy, frameWidth, frameHeight, x, y, 0);
    }
};

class Background
{
    public:
	float x;
	float y;
	float velX;
	float velY;
	int dirX;
	int dirY;

	int largura;
	int altura;

	ALLEGRO_BITMAP *image;
};

void InitBackground(Background &backg, float x, float y, float velx, float vely, int largura, int altura, int dirX, int dirY, ALLEGRO_BITMAP *image)
{
	backg.x += backg.velX * backg.dirX;
	if(backg.x + backg.largura <= 0)
		backg.x = 0;
}
void DrawBackground(Background &backg)
{
	al_draw_bitmap(backg.image, backg.x, backg.y, 0);

	if(backg.x + backg.largura < width)
		al_draw_bitmap(backg.image, backg.x + backg.largura, backg.y, 0);
}
void UpdateBackground(Background &backg)
{
	backg.x += backg.velX * backg.dirX;
	if(backg.x + backg.largura <= 0)
		backg.x = 0;
}
#endif // PERSONAGENS_HPP_INCLUDED
