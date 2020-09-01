#include <nds.h>
#include <stdio.h>
#include <stdlib.h> 

#include <SpriteSheet.h>

#include <bg.h>

#define FRAMES_PER_ANIMATION 2
#define ANIMATION_SPEED 0.05


typedef struct
{
	int x, y; // position in screen

	int dX;	// Movement speed

	u16* sprite_gfx;
	u8* frame;

	int state;
	int anim_frame;

}Sprite;

typedef struct
{
	int x, y; // position in screen

	int dY;	// Movement speed

	int isBomb;	// Handles if the sprite is a bomb

	u16* sprite_gfx[2];
	int frame;

	int state;
	int anim_frame;
}Sprites;

//---------------------------------------------------------------------
// Screen dimentions
//---------------------------------------------------------------------
enum {SCREEN_TOP = 0, SCREEN_BOTTOM = 192, SCREEN_LEFT = 0, SCREEN_RIGHT = 256};


int lives = 3;
float currAnim = 0;

bool endGame = false;

Sprite basket = {(SCREEN_RIGHT-32)/2,150, 2};	
Sprites apple = {10,0,1, 0};
Sprites bomb = {50,0,1, 1};
Sprites curr;

Sprite mLives[];

Sprite at = {8, SCREEN_BOTTOM - 32};
Sprite name01 ={32, SCREEN_BOTTOM - 24};
Sprite name02 = {48, SCREEN_BOTTOM - 24};
Sprite name03 = {64, SCREEN_BOTTOM - 24};
Sprite name04 = {80, SCREEN_BOTTOM - 24};
Sprite name05 = {96, SCREEN_BOTTOM - 24};




bool detectCollision(Sprite basket, Sprites sprite) 
{

	if (basket.x < (sprite.x + 16) && 		// left basket will detect right sprite
		(basket.x + 32) > sprite.x && 		// right basket will detect left sprite
		basket.y < (sprite.y + 16) && 		// top basket will detect sprite bottom
		(basket.y + 32) > sprite.y) { 		// bottom basket will detect sprite top
    	return true;
	}
	return false;
}




void animateSprites(){
	curr.frame = curr.anim_frame;
}

void fallSprites(){
	curr.y++;
	currAnim += ANIMATION_SPEED;

	if(currAnim > FRAMES_PER_ANIMATION) currAnim = 0;

	curr.anim_frame = currAnim;
}


void initBasket(Sprite *basket, u8* gfx)
{
	basket->sprite_gfx = oamAllocateGfx(&oamMain, SpriteSize_32x32, SpriteColorFormat_256Color);
	dmaCopy(gfx + (16*16*5), basket->sprite_gfx, 32*32);
	basket->frame = (u8*)gfx;
}

void initApple(Sprites *apple, u8* gfx)
{
	int i;

	for(i = 0; i < 2; i++)
	{
		apple->sprite_gfx[i] = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_256Color);
		dmaCopy(gfx, apple->sprite_gfx[i], 16*16);
		gfx += 16*16;
	}
}

void initBomb(Sprites *bomb, u8* gfx)
{
	int i;

	for(i = 0; i < 2; i++)
	{
		bomb->sprite_gfx[i] = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_256Color);
		dmaCopy(gfx + (16*16*3), bomb->sprite_gfx[i], 16*16);	
		gfx += 16*16;
	}
}

void initHeart(Sprite *heart, u8* gfx)
{
	heart->sprite_gfx = oamAllocateGfx(&oamSub, SpriteSize_16x16, SpriteColorFormat_256Color);
	dmaCopy(gfx + (16*16*2), heart->sprite_gfx, 16*16);
	heart->frame = (u8*)gfx;
}

void initName(Sprite *at, Sprite *name01, Sprite *name02, Sprite *name03, Sprite *name04, Sprite *name05, u8* gfx)
{
	// AT
	at->sprite_gfx = oamAllocateGfx(&oamSub, SpriteSize_32x32, SpriteColorFormat_256Color);
	dmaCopy(gfx + (16*16*15), at->sprite_gfx, 32*32);
	at->frame = (u8*)gfx;

	// 01
	name01->sprite_gfx = oamAllocateGfx(&oamSub, SpriteSize_16x16, SpriteColorFormat_256Color);
	dmaCopy(gfx + (16*16*10), name01->sprite_gfx, 16*16);
	name01->frame = (u8*)gfx;

	//02
	name02->sprite_gfx = oamAllocateGfx(&oamSub, SpriteSize_16x16, SpriteColorFormat_256Color);
	dmaCopy(gfx + (16*16*11), name02->sprite_gfx, 16*16);
	name02->frame = (u8*)gfx;

	//03
	name03->sprite_gfx = oamAllocateGfx(&oamSub, SpriteSize_16x16, SpriteColorFormat_256Color);
	dmaCopy(gfx + (16*16*12), name03->sprite_gfx, 16*16);
	name03->frame = (u8*)gfx;

	//04
	name04->sprite_gfx = oamAllocateGfx(&oamSub, SpriteSize_16x16, SpriteColorFormat_256Color);
	dmaCopy(gfx + (16*16*13), name04->sprite_gfx, 16*16);
	name04->frame = (u8*)gfx;

	//05
	name05->sprite_gfx = oamAllocateGfx(&oamSub, SpriteSize_16x16, SpriteColorFormat_256Color);
	dmaCopy(gfx + (16*16*14), name05->sprite_gfx, 16*16);
	name05->frame = (u8*)gfx;
}


void spawnCurr()
{
	int r = rand() % 100;	
	curr = (r > 20) ? apple : bomb;
	curr.x = (rand() % SCREEN_RIGHT) - 16;
}

void initSprites()
{

	initBasket(&basket, (u8*)SpriteSheetTiles);			// init Basket Sprite
	
	for (int i = 0; i < lives; i++)
		initHeart(&mLives[i], (u8*)SpriteSheetTiles);	// init Hearts Sprite
	

	initApple(&apple, (u8*)SpriteSheetTiles);			// init Apple Sprite
	initBomb(&bomb, (u8*)SpriteSheetTiles);				// init Bomb Sprite

	initName(&at, &name01, &name02, &name03, &name04, &name05, (u8*)SpriteSheetTiles);	// init Name Sprites
}


void init()
{
	// ------------------------------------------------------
	// 						INIT VIDEO
	// ------------------------------------------------------
	videoSetMode(MODE_0_2D);
	videoSetModeSub(MODE_0_2D);

	vramSetBankA(VRAM_A_MAIN_SPRITE);
	vramSetBankD(VRAM_D_SUB_SPRITE);

	oamInit(&oamMain, SpriteMapping_1D_128, false);
	oamInit(&oamSub, SpriteMapping_1D_128, false);

	// ------------------------------------------------------
	// 						INIT BG
	// ------------------------------------------------------
	int bg = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);

	dmaCopy(bgTiles, bgGetGfxPtr(bg), 256*256);
	dmaCopy(bgPal, BG_PALETTE, 256*2);


	// Init Lives
	lives = 3;
	
	// ------------------------------------------------------
	// 						INIT SPRITES
	// ------------------------------------------------------
		// Heart Sprites ( Lives )
	for (int i = 0; i < lives; i++)
	{
		Sprite tmp = {(i*20), 0};
		mLives[i] = tmp;
	}
	
	initSprites();		// init the sprites
	
	spawnCurr();		// spawn apple/bomb


	// ------------------------------------------------------
	// 						INIT PALETTES
	// ------------------------------------------------------
	dmaCopy(SpriteSheetPal, SPRITE_PALETTE, 512);		// Init the sprite sheet palette
	dmaCopy(SpriteSheetPal, SPRITE_PALETTE_SUB, 512);		// Init the sprite sheet palette
}


void step()
{
		scanKeys();

		int keys = keysHeld();

		if(keys)
		{
			if(keys & KEY_LEFT)
				if(basket.x >= SCREEN_LEFT) basket.x -= basket.dX;
			if(keys & KEY_RIGHT)
				if(basket.x <= (SCREEN_RIGHT - 32)) basket.x += basket.dX;
		}

		if(detectCollision(basket, curr))
		{
			if(curr.isBomb == 0)			// curr is an Apple
				resetSprite(0);
			else
				resetSprite(1);
		}
		else if(curr.y >= (SCREEN_BOTTOM - 5))
		{
			if(curr.isBomb)
				resetSprite(0);
			else
				resetSprite(1);
		}


		fallSprites();
		animateSprites();

	//MAIN SCREEN oamSet
		oamSet(&oamMain, 0, basket.x, basket.y, 0, 15, SpriteSize_32x32, SpriteColorFormat_256Color, basket.sprite_gfx, -1, false, false, false, false, false);
	//	oamSet(&oamMain, 1, apple.x, apple.y, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color, apple.sprite_gfx[apple.frame], -1, false, false, false, false, false);
		oamSet(&oamMain, 1, curr.x, curr.y, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color, curr.sprite_gfx[curr.frame], -1, false, false, false, false, false);
		
	//SUB SCREEN oamSet
		oamSet(&oamSub, 0, at.x, at.y, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color, at.sprite_gfx, -1, false, false, false, false, false);
		oamSet(&oamSub, 1, name01.x, name01.y, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color, name01.sprite_gfx, -1, false, false, false, false, false);
		oamSet(&oamSub, 2, name02.x, name02.y, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color, name02.sprite_gfx, -1, false, false, false, false, false);
		oamSet(&oamSub, 3, name03.x, name03.y, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color, name03.sprite_gfx, -1, false, false, false, false, false);
		oamSet(&oamSub, 4, name04.x, name04.y, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color, name04.sprite_gfx, -1, false, false, false, false, false);
		oamSet(&oamSub, 5, name05.x, name05.y, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color, name05.sprite_gfx, -1, false, false, false, false, false);
		



		oamSet(&oamSub, 6, mLives[0].x, mLives[0].y, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color, mLives[0].sprite_gfx, -1, false, false, false, false, false);
		oamSet(&oamSub, 7, mLives[1].x, mLives[1].y, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color, mLives[1].sprite_gfx, -1, false, false, false, false, false);
		oamSet(&oamSub, 8, mLives[2].x, mLives[2].y, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color, mLives[2].sprite_gfx, -1, false, false, false, false, false);


		swiWaitForVBlank();

		oamUpdate(&oamMain);
		oamUpdate(&oamSub);

}

void quit()
{
	// unload all assets from memory
		// oamFree()
	// Clean main screen
	oamFreeGfx(&oamMain, &basket.sprite_gfx);			// Basket
	oamFreeGfx(&oamMain, &apple.sprite_gfx);			// Apple
	oamFreeGfx(&oamMain, &bomb.sprite_gfx);				// Bomb
	oamFreeGfx(&oamMain, &curr.sprite_gfx);				// Current
	
	bgWrapOff(&bgInit);
	bgWindowDisable(&bgInit, WINDOW_0);
	oamFreeGfx(&oamMain, &bgInit);						// BG

	// Clean sub screen
	for (int i = 0; i < 3; i++)
		oamFreeGfx(&oamSub, &mLives[i].sprite_gfx);		// Hearts

	oamFreeGfx(&oamSub, &at.sprite_gfx);				// AT
	oamFreeGfx(&oamSub, &name01.sprite_gfx);			// Name 01
	oamFreeGfx(&oamSub, &name02.sprite_gfx);			// Name 02
	oamFreeGfx(&oamSub, &name03.sprite_gfx);			// Name 03
	oamFreeGfx(&oamSub, &name04.sprite_gfx);			// Name 04
	oamFreeGfx(&oamSub, &name05.sprite_gfx);			// Name 05

	// RESET Variables
	currAnim = 0;

	*basket.sprite_gfx = NULL;
	*basket.frame = NULL;

	*apple.sprite_gfx = NULL;
	*bomb.sprite_gfx = NULL;
	*curr.sprite_gfx = NULL;

	*at.sprite_gfx = NULL;
	*at.frame = NULL;
	
	*name01.sprite_gfx = NULL;
	*name01.frame = NULL;
	
	*name02.sprite_gfx = NULL;
	*name02.frame = NULL;
	
	*name03.sprite_gfx = NULL;
	*name03.frame = NULL;
	
	*name04.sprite_gfx = NULL;
	*name04.frame = NULL;
	
	*name05.sprite_gfx = NULL;
	*name05.frame = NULL;
}







void resetSprite(int livesLost, Sprites *_curr, Sprites *_appple, Sprites *_bomb)
{
	if(lives > 0)
	{
		lives -= livesLost;
		if(livesLost > 0)
			mLives[lives].y = 200;

		spawnCurr(curr, apple, &_bomb);
		// win sound


		if(lives <= 0){
			curr.y = -16;
			endGame = true;
		}
	}
	else
	{
		curr.y = -16;
		endGame = true;
		// Loss sound
	}
	
}

//---------------------------------------------------------------------
// main
//---------------------------------------------------------------------
int main(void) 
{
	init();




	while(!endGame) 
		step();
	

	quit();

	return 0;
}
