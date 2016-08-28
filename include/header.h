#ifndef _header_h_
#define _header_h_

//#define USE_MP3

#define MENUBUF 17
#define UNDOSLOTEND 16
u16 undo[18][256*192] __attribute__ ((aligned (4)));

typedef struct{
	int swaplr;
	int autogrid;
	int staticsprites;
	int ghcontroller;
}optionscrap;
optionscrap options;
//stamps
typedef struct{
	s16 x;
	s16 y;
	s16 endy;
	s16 endx;
}stampinfo;
stampinfo Stamps[6][100];
char StampUsed[500];
typedef struct{
	s16 x1;
	s16 x2;
	s16 y1;
	s16 y2;
	s16 width;
	s16 height;
	s16 nletter; // Next letter to right. 0 since no letters are there yet
	char letter; // New letter to write.
	char text[500];
	s8 fontsize;
}typingcrap;
typingcrap type;
//file browser--------------------------------------------
typedef struct{
	char filename[100];
	char name[100];
	char ext[10];  
	u32 size;
}  PAfiletype;
PAfiletype files[500];

#ifdef USE_MP3
	typedef struct{
		char filename[100];
		char name[100];
		char ext[10];  
		u32 size;
	}  MP3type;
	MP3type MP3[500];

	int MP3FILES;
#endif

u32 FileNumber;
char location[512];	
char path[512];

char filename[256];
char * buffer;
FILE *savefile;
bool filesfound;
s16 number;


//drawing----------------------------------------------------
int transparency[2];
int tempcolors[11];
s8 pal;
u8 DrawMode;
int color[2];
s16 scrolly; 
s16 scrollx;
s16 centerx;
s16 centery;
s16 zoom;

u8 drawsize;
u8 erasersize;
u8 linesize;
u8 spraysize;
u8 recsize;
u8 tempsize;

//dpad mode
s16 PixCursorX;
s16 PixCursorY;

//copy and paste
typedef struct{
	u8 useCopy;
	u8 hasCopy;
	u8 yflip[2];
	u8 xflip[2];
	s16 OldStylusX;
	s16 OldStylusY;
	u16 copy[2][256*192];
	u16 width[2];
	u16 height[2];
}Copyandpasteinfo;
Copyandpasteinfo CnP;

s32 nfile;
bool ReplaceColor;
u8 ToolBox;
s16 Sx1,Sx2,Sy1,Sy2;//stylus coordinates for copying

//canvas dimension data stuff
typedef struct{
	s16 drawwidth;
	s16 drawheight;
	s16 X;
	s16 Y;
	s16 TempWidth;
	s16 TempHeight;
}canvasinfo;
canvasinfo Canvas;

//fill function
typedef struct{
	u16 oldcolor;
	u16 newcolor;
	s32 nextfill;
	u8 nextx[256*192];
	u8 nexty[256*192];
}fillinfo;
fillinfo Bucket;

//undo slots
s8 StartSlot,EndSlot;

//menu variables
int DrawTool;
bool ShowStats;
s16 ZoomBarX;
u8 Colx;
u8 Coly;
s16 StylusOldX;
s16 StylusOldY;
//pixel cursor
#define HELDTIME 25
#define SCROLLSPEED 2
s16 CursorX;
s16 CursorY;

//color variables
s8 red,green,blue;
s16 stamp;

//menus
bool screen;
//canvas size functions
void LoadCanvas(bool save);
void ClearGreyAreas(void);
//menus
void CSizeMenu(bool screen,bool firststart);
void ColorCreator(bool savedraw,bool BackToKeys);
void LoadOptions(void);
void OptionsMenu(void);
void LoadMenu(void);
void loadgui(bool initdrawscreen,bool resetpal,bool restoredrawing);
void Tools(bool switchscreen);
void CreateSlider(void);
void UpdateTools(int tool);
void LoadDefaultPallet(void);
void DrawLoop(void);
void KeyBoardMenu(bool savetype);
void LoadPalMenu(void);
void UpdateColorSquare(void);
void UpdateDrawSizeSquare(int size);
void ResetDrawSizes(void);
void RecallDrawSize(u8 tool);
void UpdateColrCursor(void);
void AdjustPixelCursor(void);
void Status(void);
void StampMenu(void);
void TypeWriterMode(bool BackToKeys);

//zoooming
void CheckZoom(void);
void CheckScroll(void);

//drawing
void CopyPasteFun(void);
void SaveUndo(int slot);
void PA_ResetBgLayer(u8 screen, u8 bg_select);
bool CanvasLimits(void);
bool CursorCanvasLimits(void);
void HideSprite(bool screen, int sprite,bool hide);
void Copy(bool screen,int x1,int x2,int y1,int y2,int slot);
void Paste(bool screen,int x,int y,bool xflip,bool yflip,int slot);
void CheckFlips(bool screen,int slot);
void CheckSaveUndo(void);
void UndoSlot(int slot);
void CheckUndo(void);
void UpdateLastPal(int pallete);
void ClearPal(int pallete);
void ClearAllPalletes(void);
void CheckPalletes(void);
void ClearDrawing(bool save);
void Draw(u8 screen, u16 x, u16 y, u16 color,bool undo);
void Line(u8 screen, u16 x,u16 y,u16 color,bool undo);
void UnfilledRectangle(bool screen,u16 x,u16 y, u16 color);
void TextBoarder(bool screen, int x,int y,u16 color, bool mode);
void Rectangle(u8 screen,u16 x,u16 y,u16 color, bool undo);
void draw16BitCircle(u8 screen, u16 x0, u16 y0, u16 colour,bool undo);
void CheckFill(void);
void SprayCan(int x, int y, int size,int color);
void UnfilledRectTool(bool screen, s16 x,s16 y, u16 color,bool undo);
void StampOptions(void);

//file browser
void SavePallet(char* filename);
void LoadPallet(char *directory,int filenumber);
void LoadDefaultPallet(void);
void ResetFATSearch(void);
void PA_LoadFileFat(int filenumber);
void LoadPreviewPic(char *directory,int filenumber);
void InitFatSys(char *directory, char *ext);
void ListDirectory(char *directory, bool screen,int x, int y);
void LoadStamp(char *filepath, int filenumber,int x,int y,bool disp);
void ClearBuffer(void);
void CopyMp3Info(void);
//general
void UpdateVariableValues(void);

//mp3
void UpdateMp3(void);
void SetupMp3System(void);

#endif 
