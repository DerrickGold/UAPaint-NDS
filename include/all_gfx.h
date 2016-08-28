//Gfx converted using Mollusk's PAGfx Converter

//This file contains all the .h, for easier inclusion in a project

#ifndef ALL_GFX_H
#define ALL_GFX_H

#ifndef PAGfx_struct
    typedef struct{
    void *Map;
    int MapSize;
    void *Tiles;
    int TileSize;
    void *Palette;
    int *Info;
} PAGfx_struct;
#endif


// Sprite files : 
extern const unsigned char arrow_Sprite[256] __attribute__ ((aligned (4))) ;  // Pal : arrow_Pal
extern const unsigned char backbutton_Sprite[2048] __attribute__ ((aligned (4))) ;  // Pal : backbutton_Pal
extern const unsigned char bar_Sprite[128] __attribute__ ((aligned (4))) ;  // Pal : bar_Pal
extern const unsigned char button_Sprite[2048] __attribute__ ((aligned (4))) ;  // Pal : button_Pal
extern const unsigned char control_Sprite[2560] __attribute__ ((aligned (4))) ;  // Pal : control_Pal
extern const unsigned char cursor_Sprite[64] __attribute__ ((aligned (4))) ;  // Pal : cursor_Pal
extern const unsigned char cvasbutton_Sprite[2048] __attribute__ ((aligned (4))) ;  // Pal : cvasbutton_Pal
extern const unsigned char eraser_Sprite[2048] __attribute__ ((aligned (4))) ;  // Pal : eraser_Pal
extern const unsigned char largepals_Sprite[4096] __attribute__ ((aligned (4))) ;  // Pal : largepals_Pal
extern const unsigned char onoff_Sprite[1024] __attribute__ ((aligned (4))) ;  // Pal : onoff_Pal
extern const unsigned char palls_Sprite[1024] __attribute__ ((aligned (4))) ;  // Pal : palls_Pal
extern const unsigned char pixcurs_Sprite[64] __attribute__ ((aligned (4))) ;  // Pal : pixcurs_Pal
extern const unsigned char savebuttons_Sprite[4096] __attribute__ ((aligned (4))) ;  // Pal : savebuttons_Pal
extern const unsigned char stat_Sprite[4096] __attribute__ ((aligned (4))) ;  // Pal : stat_Pal
extern const unsigned char tool_Sprite[10240] __attribute__ ((aligned (4))) ;  // Pal : tool_Pal
extern const unsigned char zoombar_Sprite[128] __attribute__ ((aligned (4))) ;  // Pal : zoombar_Pal

// Background files : 
extern const int canvas_Info[3]; // BgMode, Width, Height
extern const unsigned short canvas_Map[768] __attribute__ ((aligned (4))) ;  // Pal : canvas_Pal
extern const unsigned char canvas_Tiles[7744] __attribute__ ((aligned (4))) ;  // Pal : canvas_Pal
extern PAGfx_struct canvas; // background pointer

extern const int grid_Info[3]; // BgMode, Width, Height
extern const unsigned short grid_Map[768] __attribute__ ((aligned (4))) ;  // Pal : grid_Pal
extern const unsigned char grid_Tiles[512] __attribute__ ((aligned (4))) ;  // Pal : grid_Pal
extern PAGfx_struct grid; // background pointer

extern const int loadbg_Info[3]; // BgMode, Width, Height
extern const unsigned short loadbg_Map[768] __attribute__ ((aligned (4))) ;  // Pal : loadbg_Pal
extern const unsigned char loadbg_Tiles[9536] __attribute__ ((aligned (4))) ;  // Pal : loadbg_Pal
extern PAGfx_struct loadbg; // background pointer

extern const int stampgrid_Info[3]; // BgMode, Width, Height
extern const unsigned short stampgrid_Map[768] __attribute__ ((aligned (4))) ;  // Pal : stampgrid_Pal
extern const unsigned char stampgrid_Tiles[640] __attribute__ ((aligned (4))) ;  // Pal : stampgrid_Pal
extern PAGfx_struct stampgrid; // background pointer


// Palette files : 
extern const unsigned short arrow_Pal[7] __attribute__ ((aligned (4))) ;
extern const unsigned short backbutton_Pal[52] __attribute__ ((aligned (4))) ;
extern const unsigned short bar_Pal[9] __attribute__ ((aligned (4))) ;
extern const unsigned short button_Pal[52] __attribute__ ((aligned (4))) ;
extern const unsigned short control_Pal[27] __attribute__ ((aligned (4))) ;
extern const unsigned short cursor_Pal[2] __attribute__ ((aligned (4))) ;
extern const unsigned short cvasbutton_Pal[54] __attribute__ ((aligned (4))) ;
extern const unsigned short eraser_Pal[7] __attribute__ ((aligned (4))) ;
extern const unsigned short largepals_Pal[3] __attribute__ ((aligned (4))) ;
extern const unsigned short onoff_Pal[19] __attribute__ ((aligned (4))) ;
extern const unsigned short palls_Pal[3] __attribute__ ((aligned (4))) ;
extern const unsigned short pixcurs_Pal[2] __attribute__ ((aligned (4))) ;
extern const unsigned short savebuttons_Pal[50] __attribute__ ((aligned (4))) ;
extern const unsigned short stat_Pal[23] __attribute__ ((aligned (4))) ;
extern const unsigned short tool_Pal[180] __attribute__ ((aligned (4))) ;
extern const unsigned short zoombar_Pal[9] __attribute__ ((aligned (4))) ;
extern const unsigned short canvas_Pal[104] __attribute__ ((aligned (4))) ;
extern const unsigned short grid_Pal[4] __attribute__ ((aligned (4))) ;
extern const unsigned short loadbg_Pal[116] __attribute__ ((aligned (4))) ;
extern const unsigned short stampgrid_Pal[2] __attribute__ ((aligned (4))) ;


#endif

