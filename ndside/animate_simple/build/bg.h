
//{{BLOCK(bg)

//======================================================================
//
//	bg, 256x256@8, 
//	Transparent color : FF,00,FF
//	+ palette 256 entries, not compressed
//	+ 1024 tiles Metatiled by 2x2 not compressed
//	Total size: 512 + 65536 = 66048
//
//	Time-stamp: 2020-09-01, 13:40:25
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.15
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_BG_H
#define GRIT_BG_H

#define bgTilesLen 65536
extern const unsigned int bgTiles[16384];

#define bgPalLen 512
extern const unsigned short bgPal[256];

#endif // GRIT_BG_H

//}}BLOCK(bg)
