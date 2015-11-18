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

struct Sprite
{
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
};

void InitSprites(Sprite &sprite, ALLEGRO_BITMAP *image)
{
	sprite.x = 220;
	sprite.y = 500;
	sprite.velX = 0;
	sprite.velY = 0;
	sprite.dirX = -1;
	sprite.dirY = -1;

	sprite.maxFrame = 4;
	sprite.curFrame = 0;
	sprite.frameCount = 0;
	sprite.frameDelay = 8;
	sprite.frameWidth = 74;
	sprite.frameHeight = 99;
	sprite.animationColumns = 16;
	sprite.animationDirection = 1;

	sprite.image = image;
}

void UpdateSprites(Sprite &sprite)
{
    sprite.x += sprite.velX * sprite.dirX;
	sprite.y += sprite.velY * sprite.dirY;

    	if(++sprite.frameCount >= sprite.frameDelay)
	{
		sprite.curFrame += sprite.animationDirection;
		if(sprite.curFrame >= sprite.maxFrame)
			sprite.curFrame = 0;
		else if(sprite.curFrame <= 0)
			sprite.curFrame = sprite.maxFrame - 1;
		sprite.frameCount = 0;
	}
/*
	if((sprite.x <= 0 && sprite.dirX == -1) ||
		(sprite.x >= width - sprite.frameWidth && sprite.dirX == 1))
	{
		sprite.dirX *= -1;
		sprite.animationDirection *= -1;
	}
	if((sprite.y <= 0 && sprite.dirY == -1) ||
		(sprite.y >= height - sprite.frameHeight && sprite.dirY == 1))
	{
		sprite.dirY *= -1;
		sprite.animationDirection *= -1;
	}*/
}


void DrawSprites(Sprite &sprite)
{
	int fx = (sprite.curFrame % sprite.animationColumns) * sprite.frameWidth;
	int fy = (sprite.curFrame / sprite.animationColumns) * sprite.frameHeight;

	al_draw_bitmap_region(sprite.image, fx, fy, sprite.frameWidth, sprite.frameHeight, sprite.x, sprite.y, 0);
}
#endif // PERSONAGENS_HPP_INCLUDED
