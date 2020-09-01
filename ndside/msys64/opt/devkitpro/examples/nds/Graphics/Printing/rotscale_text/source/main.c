#include <nds.h>

#include <stdio.h>

//Include the font header generated by grit
#include "font.h"

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------

	const int tile_base = 0;
	const int map_base = 20;


	videoSetMode(0);	

	videoSetModeSub(MODE_5_2D);	
	vramSetBankC(VRAM_C_SUB_BG); 

	PrintConsole *console = consoleInit(0, 3, BgType_ExRotation, BgSize_ER_256x256, map_base, tile_base, false, false);

	ConsoleFont font;

	font.gfx = (u16*)fontTiles;
	font.pal = (u16*)fontPal;
	font.numChars = 95;
	font.numColors =  fontPalLen / 2;
	font.bpp = 8;
	font.asciiOffset = 32;
	font.convertSingleColor = false;
	
	consoleSetFont(console, &font);

	int bg3 = console->bgId;

	iprintf("Custom Font Demo\n");
	iprintf("   by Poffy\n");
	iprintf("modified by WinterMute and dovoto\n");
	iprintf("for libnds examples\n");

	
	unsigned int angle = 0;
    int scrollX = 0;
	int scrollY = 0;
	int scaleX = intToFixed(1,8);
	int scaleY = intToFixed(1,8);

	while(1) {
		scanKeys();
		u32 keys = keysHeld();

		if ( keys & KEY_START ) break;

		if ( keys & KEY_L ) angle+=64; 
		if ( keys & KEY_R ) angle-=64;

		if ( keys & KEY_LEFT ) scrollX++;
		if ( keys & KEY_RIGHT ) scrollX--;
		if ( keys & KEY_UP ) scrollY++;
		if ( keys & KEY_DOWN ) scrollY--;

		if ( keys & KEY_A ) scaleX++;
		if ( keys & KEY_B ) scaleX--;

		if( keys & KEY_X ) scaleY++;
		if( keys & KEY_Y ) scaleY--;

		swiWaitForVBlank();


		bgSetRotateScale(bg3, angle, scaleX, scaleY);
		bgSetScroll(bg3, scrollX, scrollY);
		bgUpdate();
	}

}
