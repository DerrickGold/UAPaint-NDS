// Includes
#include <PA9.h>       // Include for PA_Lib
#include "all_gfx.h"
#include "GHController.h"


//extra features
#include "header.h"
//loading frames
#include "loading0.h"
#include "loading1.h"
#include "loading2.h"
#include "loading3.h"
//intro frames
#include "intro0.h"
#include "intro1.h"
#include "intro2.h"
#include "intro3.h"
#include "intro4.h"
#include "intro5.h"
#include "intro6.h"
#include "intro7.h"
#include "intro8.h"
#include "intro9.h"
//misc
#include "bottom.h"
#include "load.h"
#define SPLASH 0
#define EMULATOR 0

inline void UpdateVariableValues(void){
	transparency[0]=PA_RGB(31,31,31);
	transparency[1]=PA_RGB(31,0,31);
	zoom=256;
	drawsize=1;
	erasersize=4;
	linesize=1;
	spraysize=5;
	recsize=1;
	Canvas.drawwidth=256;
	Canvas.drawheight=192;
	Canvas.TempWidth=256;
	Canvas.TempHeight=192;
	ZoomBarX=16;
	CursorX=216;
	CursorY=12;
	tempsize=spraysize;
}
void SetLoadingFrame(int frame){
	switch(frame){
		case 0:PA_LoadGif(	1, (void*)loading0);break;
		case 1:PA_LoadGif(	1, (void*)loading1);break;
		case 2:PA_LoadGif(	1, (void*)loading2);break;
		case 3:PA_LoadGif(	1, (void*)loading3);break;
		case 4:PA_LoadGif(	1, (void*)intro0);break;
	}
}
void SetBGFrame(int frame){
	switch(frame){
		case 0:PA_LoadGif(	1, (void*)intro0);break;
		case 1:PA_LoadGif(	1, (void*)intro1);break;
		case 2:PA_LoadGif(	1, (void*)intro2);break;
		case 3:PA_LoadGif(	1, (void*)intro3);break;
		case 4:PA_LoadGif(	1, (void*)intro4);break;
		case 5:PA_LoadGif(	1, (void*)intro5);break;
		case 6:PA_LoadGif(	1, (void*)intro6);break;
		case 7:PA_LoadGif(	1, (void*)intro7);break;
		case 8:PA_LoadGif(	1, (void*)intro8);break;
		case 9:PA_LoadGif(	1, (void*)intro9);break;
	}
}

void TitleScreen(void){
	int i=0;
	int brightness;
	int fat=0;
	bool folder=0;
	bool loadskin=0;
	if(SPLASH==0){
		PA_Init16bitBg (0,3);
		PA_Init16bitBg (1,3);
		PA_SetBrightness (0,-31);
		PA_SetBrightness (1,-31);
	}
	PA_LoadGif(	1, (void*)loading0);
	PA_LoadGif(	0, (void*)load);
	for(brightness=-31;brightness<0;brightness++){
		PA_SetBrightness (1,brightness);
		PA_SetBrightness (0,brightness);
		PA_WaitForVBL();
	}
	while(folder==0 || folder==1){
		if(i<50)i++;
		
		if(i%10==0){
			SetLoadingFrame(i/10);
			fat=i/10;
		}
		
		if(fat==0 &&folder==0 && loadskin==0){
			if(fatInitDefault())fat=1;
		}
		
		if(fat==1 &&folder==0 && loadskin==0 &&PA_Locate("/", "UAPaint",true,10,location)){
			#ifdef USE_MP3
				SetupMp3System();
			#endif
			folder=1;	
		}
		if(fat==1 && folder==1 && loadskin==0){
			loadskin=1;
		}
		
		if(folder==1 && loadskin==1 && fat==5)folder=2;
		if(i==50&&folder==0)i=0;
		PA_WaitForVBL();
	}
	
	PA_SetBrightness (0,31);	
		//title done loading
	PA_LoadGif(	1, (void*)intro0); // Gif File to play over and over again
	PA_LoadGif(	0, (void*)bottom); // Gif File to play over and over again
	
	for(brightness=31;brightness>0;brightness--){
		PA_SetBrightness (0,brightness);
		PA_WaitForVBL();
	}
	PA_WaitFor(Stylus.Newpress || Pad.Newpress.Anykey || Pad.Newpress.Y);
	ClearDrawing(0);
	
	for(i=0;i<90;i++){
		
		if(i%9==0){
			SetBGFrame(i/9);
		}
	}
	for(brightness=0;brightness<31;brightness++){
		PA_SetBrightness (1,brightness);
		PA_SetBrightness (0,brightness);
		PA_WaitForVBL();
	}
	
}
/*		
void SplashScreen(void){
	PA_SetBrightness (0,-31);
	PA_SetBrightness (1,-31);
	PA_Init16bitBg (0,3);
	PA_Init16bitBg (1,3);
	PA_LoadGif(	1, (void*)splash1); // Gif File to play over and over again
	PA_LoadGif(	0, (void*)splash2); // Gif File to play over and over again
	
	int brightness;
	for(brightness=-31;brightness<0;brightness++){
		PA_SetBrightness (1,brightness);
		PA_SetBrightness (0,brightness);
		PA_WaitForVBL();
	}
	PA_WaitFor(Pad.Newpress.Anykey || Stylus.Newpress || Pad.Newpress.Y);
	for(brightness=0;brightness>-31;brightness--){
		PA_SetBrightness (1,brightness);
		PA_SetBrightness (0,brightness);
		PA_WaitForVBL();
	}
}
*/

void DrawLoop(void){
	bool contdrawloop=1;
	while(contdrawloop==1){
		if(screen==0){
		
			//rest of the tools
			switch(DrawTool){
				//Pen
				case 0:
					if(DrawMode==0){
						if(CanvasLimits()==TRUE)Draw(0,(((Stylus.X*zoom)>>8)+scrollx),(((Stylus.Y*zoom)>>8)+scrolly),PA_RGB(color[pal]&31, color[pal]>>5&31, color[pal]>>10&31),1);
						CheckZoom();
					}
					//pixel plotter mode
					if(DrawMode==1 && zoom<=32){
						if(Pad.Newpress.A)CheckSaveUndo();
						if(Pad.Held.A){
							if(CursorCanvasLimits()==TRUE)PA_Put16bitPixel(0,(((PixCursorX*zoom)>>8)+scrollx),(((PixCursorY*zoom)>>8)+scrolly),PA_RGB(color[pal]&31, color[pal]>>5&31, color[pal]>>10&31));
						}
						AdjustPixelCursor();
						CheckScroll();
					}
					else DrawMode=0;
				break;
				//Bucket
				case 1:
					if(CanvasLimits()==TRUE)CheckFill();
					CheckZoom();
				break;
				//eraser
				case 2:
					PA_SetDrawSize(0,erasersize);
					if(ReplaceColor==0){
						if(CanvasLimits()==TRUE)Draw(0,(((Stylus.X*zoom)>>8)+scrollx),(((Stylus.Y*zoom)>>8)+scrolly),PA_RGB(31,31,31),1);
					}
					else if(ReplaceColor==1){
						if(pal==1){
							if(PA_Get16bitPixel(0,(((Stylus.X*zoom)>>8)+scrollx),(((Stylus.Y*zoom)>>8)+scrolly))==color[0]){
								Draw(0,(((Stylus.X*zoom)>>8)+scrollx),(((Stylus.Y*zoom)>>8)+scrolly),PA_RGB(color[pal]&31, color[pal]>>5&31, color[pal]>>10&31),1);
							}
						}
						if(pal==0){
							if(PA_Get16bitPixel(0,(((Stylus.X*zoom)>>8)+scrollx),(((Stylus.Y*zoom)>>8)+scrolly))==color[1]){
								Draw(0,(((Stylus.X*zoom)>>8)+scrollx),(((Stylus.Y*zoom)>>8)+scrolly),PA_RGB(color[pal]&31, color[pal]>>5&31, color[pal]>>10&31),1);
							}
						}
					}
					CheckZoom();
				break;	
				//line drawer
				case 3:
					Line(0,(((Stylus.X*zoom)>>8)+scrollx),(((Stylus.Y*zoom)>>8)+scrolly),PA_RGB(color[pal]&31, color[pal]>>5&31, color[pal]>>10&31),1);
					CheckZoom();
				break;
				//rectangle drawer
				case 4:
					Rectangle(0,(((Stylus.X*zoom)>>8)+scrollx),(((Stylus.Y*zoom)>>8)+scrolly),color[pal],1);
					CheckZoom();
				break;
				//circle drawer
				case 5:
					UnfilledRectTool(0,(((Stylus.X*zoom)>>8)+scrollx),(((Stylus.Y*zoom)>>8)+scrolly),color[pal],1);
				break;
				//circle
				case 6:
					draw16BitCircle(0,(((Stylus.X*zoom)>>8)+scrollx),(((Stylus.Y*zoom)>>8)+scrolly),color[pal],1);
					CheckZoom();
				break;
				//copy and paste tool
				case 7:
					CopyPasteFun();
				break;
				case 8://textmode
					TextBoarder(0,(((Stylus.X*zoom)>>8)+scrollx),(((Stylus.Y*zoom)>>8)+scrolly),color[pal],1);
					CheckZoom();
				break;
				case 9://spray can
					SprayCan((((Stylus.X*zoom)>>8)+scrollx),(((Stylus.Y*zoom)>>8)+scrolly),spraysize,color[pal]);
					CheckZoom();
				break;
				case 10://stamp
				break;
				case 11://color picker
					if(Stylus.Held){
						if(CanvasLimits()==TRUE)color[pal]=PA_Get16bitPixel(0,(((Stylus.X*zoom)>>8)+scrollx),(((Stylus.Y*zoom)>>8)+scrolly));
						UpdateColorSquare();//updates the color square to show black
					}
					CheckZoom();
				break;
			}		
			if(Stylus.Released && Canvas.drawwidth!=256 && Canvas.drawheight!=192)ClearGreyAreas();
		}
		//switching screens
		switch(options.swaplr){
			case 0:
				if(Pad.Newpress.L){
					screen=1;
					PA_SwitchScreens();
					PA_WaitForVBL();PA_WaitForVBL();
					contdrawloop=0;
					Tools(screen);
					
				}
				if(Pad.Newpress.R){
					pal++;
					if(pal>=2)pal=0;
					UpdateColorSquare();
				}
			break;
			case 1:
				if(Pad.Newpress.R){		
					screen=1;
					PA_SwitchScreens();
					PA_WaitForVBL();PA_WaitForVBL();
					contdrawloop=0;
					Tools(screen);
					
				}
				if(Pad.Newpress.L){
					pal++;
					if(pal>=2)pal=0;
					UpdateColorSquare();
				}
			break;
		}
		//switch to pixil mode
		if(Pad.Newpress.X){
			DrawMode++;
			if(DrawMode==1 && zoom<=32){
				HideSprite(0,0,0);
				UpdateTools(0);
			}
			if(DrawMode>=2)DrawMode=0;
			if(DrawMode==0)HideSprite(0,0,1);
		}
		//undo
		if(Pad.Newpress.Y)CheckUndo();
		//options
		if(Pad.Newpress.Start){
			contdrawloop=0;
			OptionsMenu();
		}
		if(options.ghcontroller==0){
			if(GHPad.Newpress.Blue)UpdateTools(0);
			if(GHPad.Newpress.Yellow)UpdateTools(1);
			if(GHPad.Newpress.Red)UpdateTools(6);
			if(GHPad.Newpress.Green)UpdateTools(3);
			GH_UpdatePad();
		}
		//keeps track of stats
		#ifdef USE_MP3
			UpdateMp3();
		#endif
		Status();
		PA_WaitForVBL();
	}
}

// Function: main()
int main(int argc, char ** argv)
{

	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	//if(SPLASH==1)SplashScreen();
	if(EMULATOR==0)TitleScreen();
	PA_InitRand ();
	UpdateVariableValues();
	//load options
	LoadOptions();	
	loadgui(1,1,0);//sets up the interface
	CSizeMenu(0,1);
	DrawLoop();
	// Infinite loop to keep the program running
	while (1)
	{
		
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()

