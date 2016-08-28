
#include <PA9.h>
#include "all_gfx.h"
#include "bmp.h"
#include "header.h"
//graphics
#include "topscreen.h"
#include "tools1.h"
#include "tools2.h"
//updates square which shows current color
void UpdateColorSquare(){
	u8 x,y;
	switch(pal){
		case 0:
			//back square
			for(x=236; x<247; ++x){
				for(y=10; y<21; ++y){
					PA_Put16bitPixel(1, x, y, PA_RGB(color[1]&31,color[1]>>5&31,color[1]>>10&31));
				}
			}
			//front square
			for(x=229; x<240; ++x){
				for(y=3; y<14; ++y){
					PA_Put16bitPixel(1, x, y, PA_RGB(color[0]&31,color[0]>>5&31,color[0]>>10&31));
				}
			}
		break;
		case 1:
			//back square
			for(x=236; x<247; ++x){
				for(y=10; y<21; ++y){
					PA_Put16bitPixel(1, x, y, PA_RGB(color[0]&31,color[0]>>5&31,color[0]>>10&31));
				}
			}
			//front square
			for(x=229; x<240; ++x){
				for(y=3; y<14; ++y){
					PA_Put16bitPixel(1, x, y, PA_RGB(color[1]&31,color[1]>>5&31,color[1]>>10&31));
				}
			}
		break;	
	}

}

//updates square which shows current draw size
inline void UpdateDrawSizeSquare(int size){
	u8 x,y;
	for(x=1; x<22; ++x){
		for(y=1; y<22; ++y){
			PA_Put16bitPixel(1, x, y, PA_RGB(0,0,0));
		}
	}
	PA_SetDrawSize(1,size);
	Draw(1,11,12,PA_RGB(31,31,31),0);
}
inline void ResetDrawSizes(void){
	UpdateDrawSizeSquare(1);
	PA_SetDrawSize(0,1);
	PA_SetDrawSize(1,1);
	Draw(1,11,12,PA_RGB(31,31,31),0);
	PA_SetSpriteY(1,2,35);
}
inline void RecallDrawSize(u8 tool){
	if(tool==0){
		UpdateDrawSizeSquare(drawsize);
		PA_SetDrawSize(0,drawsize);
		PA_SetDrawSize(1,drawsize);
		Draw(1,11,12,PA_RGB(31,31,31),0);
		PA_SetSpriteY(1,2,35+drawsize);
	}
	else if(tool==1){
		UpdateDrawSizeSquare(erasersize);
		PA_SetDrawSize(0,erasersize);
		PA_SetDrawSize(1,erasersize);
		Draw(1,11,12,PA_RGB(31,31,31),0);
		PA_SetSpriteY(1,2,35+erasersize);	
	}
	else if(tool==2){
		UpdateDrawSizeSquare(linesize);
		PA_SetDrawSize(0,linesize);
		PA_SetDrawSize(1,linesize);
		Draw(1,11,12,PA_RGB(31,31,31),0);
		PA_SetSpriteY(1,2,35+linesize);	
	}	
	else if(tool==5){
		UpdateDrawSizeSquare(recsize);
		PA_SetDrawSize(0,recsize);
		PA_SetDrawSize(1,recsize);
		Draw(1,11,12,PA_RGB(31,31,31),0);
		PA_SetSpriteY(1,2,35+recsize);	
	}
	else if(tool==9){
		PA_SetSpriteY(1,2,30+spraysize);	
	}	
}
	
inline void CreateSlider(void){
	//loads background
	PA_LoadSpritePal(1, 1, (void*)eraser_Pal);
	PA_CreateSprite(1, 1, (void*)eraser_Sprite, OBJ_SIZE_32X64, 1, 1,4,26);
	PA_SetSpritePrio(1,1,2); 
	//loads the bar
	PA_LoadSpritePal(1, 2, (void*)bar_Pal);
	PA_CreateSprite(1, 2, (void*)bar_Sprite, OBJ_SIZE_16X8, 1, 2,5,35);	
}
//bool resetpal=0;
inline void UpdateColrCursor(void){
	if(Colx==0)PA_SetSpriteXY (1,5,216,12);
	else PA_SetSpriteXY (1,5,Colx/2+30,Coly/12+4);
}
void loadguisprites(void){
	//load all sprites
	//loads the tool sprite
	PA_LoadSpritePal(1, 0, (void*)tool_Pal);
	PA_CreateSprite(1, 0, (void*)tool_Sprite, OBJ_SIZE_32X32, 1, 0, 34,33);
	//zoom slider bar
	PA_LoadSpritePal(1, 3, (void*)zoombar_Pal);
	PA_CreateSprite(1, 3, (void*)zoombar_Sprite, OBJ_SIZE_8X16, 1, 3, ZoomBarX,175);

	//create stat sprite
	PA_LoadSpritePal(1,6,(void*)stat_Pal);
	PA_CreateSprite(1,7,(void*)stat_Sprite, OBJ_SIZE_64X64, 1, 6,91,67);
	PA_SetSpritePrio(1,7,2); 
	HideSprite(1,7,1);
	PA_LoadSpritePal(1, 4, (void*)palls_Pal);
	PA_CreateSprite(1, 4, (void*)palls_Sprite, OBJ_SIZE_32X32, 1, 3, 228,-9);
	//loads color picker cursor
	PA_LoadSpritePal(1, 5, (void*)cursor_Pal);
	PA_CreateSprite(1, 5, (void*)cursor_Sprite, OBJ_SIZE_8X8, 1, 4, 216,12);
	UpdateColrCursor();
	//loads pixilcursor
	PA_LoadSpritePal(0,0,(void*)pixcurs_Pal);
	PA_CreateSprite(0,0,(void*)pixcurs_Sprite, OBJ_SIZE_8X8, 1, 0,0,0);
	//PA_SetSpriteRotEnable(0, 0, 0); // Same rotset as the other sprite... it'll be zoomed the same way
	HideSprite(0,0,1);
	//load copy cut and paste buttons
	//copy
	PA_LoadSpritePal(0,1,(void*)control_Pal);
	PA_CreateSprite(0,1,(void*)control_Sprite, OBJ_SIZE_32X16, 1, 1,0,0);
	HideSprite(0,1,1);
	//Yflip
	PA_CreateSprite(0,2,(void*)control_Sprite, OBJ_SIZE_32X16, 1, 1,0,0);
	PA_SetSpriteAnimEx (0,2,32,16,1,1);
	HideSprite(0,2,1);
	//paste
	PA_CreateSprite(0,3,(void*)control_Sprite, OBJ_SIZE_32X16, 1, 1,0,0);
	PA_SetSpriteAnimEx (0,3,32,16,1,2);
	HideSprite(0,3,1);
	//cancel
	PA_CreateSprite(0,4,(void*)control_Sprite, OBJ_SIZE_32X16, 1, 1,0,0);
	PA_SetSpriteAnimEx (0,4,32,16,1,3);
	HideSprite(0,4,1);
	//Xflip
	PA_CreateSprite(0,5,(void*)control_Sprite, OBJ_SIZE_32X16, 1, 1,0,0);
	PA_SetSpriteAnimEx (0,5,32,16,1,4);
	HideSprite(0,5,1);
	//loads sliders
	CreateSlider();
}
	
void loadgui(bool initdrawscreen,bool resetpal,bool restoredrawing){
	PA_oldx[0]=Stylus.X;
	PA_oldy[0]=Stylus.Y;	
	DrawTool=0;//sets default tool

	//Clear the screens
	PA_ResetBgSysScreen (1);
	PA_ResetSpriteSysScreen(1);
	PA_ResetSpriteSysScreen(0);
	PA_ResetBgLayer(0,1);
	PA_ResetBgLayer(0,0);
	PA_Init16bitBg(1, 3);
	
	loadguisprites();
	//load gui backgrounds
	PA_LoadBmpEx (1,0,0, (void *)topscreen);//load background
	if(ToolBox==0)PA_LoadGifXY(1,239,29,(void*)tools1); 
	else if(ToolBox==1)PA_LoadGifXY(1,239,29,(void*)tools2); 
	PA_InitText(1,0);
	PA_SetTextCol (1,0,0,0);

	if(resetpal==1){
		color[1]=PA_RGB(31,31,31);//set secondary pallet to white
		ClearAllPalletes();
		resetpal=1;
		LoadDefaultPallet();
		CheckPalletes();
	}
	UpdateColorSquare();//updates the color square to show black

	if(initdrawscreen==1){
		PA_ResetBgSysScreen (0);
		PA_Init16bitBg (0,3);
		if(restoredrawing==0)LoadCanvas(1);
		else if(restoredrawing==1) LoadCanvas(0);
		CheckSaveUndo();
	}	
	if(restoredrawing==1)UndoSlot(MENUBUF);
	//delgrid=0;
	if(resetpal==0)CheckPalletes();
	if(zoom==32)zoom=0;
	CheckZoom();
	RecallDrawSize(0);
	UpdateTools(0);
}

inline void HideSlider(bool hide){
	HideSprite(1,1,hide);
	HideSprite(1,2,hide);
}

void UpdateTools(int tool){
	switch (tool){
		case 0://pen
			ResetDrawSizes();
			RecallDrawSize(0);
			HideSlider(0);
			ReplaceColor=0;
		break;
		case 1://bucket
			ResetDrawSizes();
			HideSlider(1);
			ReplaceColor=0;
		break;
		case 2://eraser
			ResetDrawSizes();
			RecallDrawSize(1);
			HideSlider(0);

		break;
		case 3://line
			ResetDrawSizes();
			RecallDrawSize(2);
			HideSlider(0);
			ReplaceColor=0;
		break;
		case 4://rectangle
			HideSlider(1);
			ReplaceColor=0;
		break;
		case 5://unfilled rect
			ResetDrawSizes();
			RecallDrawSize(5);
			HideSlider(0);
			ReplaceColor=0;
		break;
		case 6:ResetDrawSizes();HideSlider(1);ReplaceColor=0;break;//circle
		case 7:ResetDrawSizes();HideSlider(1);if(CnP.hasCopy==4)CnP.hasCopy=3;ReplaceColor=0;break;//copy tool
		case 8:ResetDrawSizes();HideSlider(1);break;//type writer
		case 9:ResetDrawSizes();RecallDrawSize(9);HideSlider(0);break;	//spray can	
		case 10:ResetDrawSizes();HideSlider(1);StampMenu();break;
		case 11:ResetDrawSizes();HideSlider(1);ReplaceColor=0;break;//color picker
	}
	DrawTool=tool;
	if(tool!=11)PA_SetSpriteAnimEx (1,0,32,32,1,tool);
	else PA_SetSpriteAnimEx (1,0,32,32,1,10);

}

inline void UpdateColorCursor(void){
	PA_SetSpriteXY (1,5,CursorX,CursorY);
	color[pal] = PA_Get16bitPixel(1,CursorX,CursorY);
	UpdateColorSquare();
}

void AdjustCursor(void){
	if(Pad.Newpress.Y){
		CursorX--;
		if(CursorX<25)CursorX++;
		UpdateColorCursor();
	}
	else if(Pad.Newpress.A){
		CursorX++;
		if(CursorX>223)CursorX--;
		UpdateColorCursor();
	}
	if(Pad.Newpress.B){
		CursorY++;
		if(CursorY>19)CursorY--;
		UpdateColorCursor();
	}
	else if(Pad.Newpress.X){
		CursorY--;
		if(CursorY<3)CursorY++;
		UpdateColorCursor();
	}
}

s16 timer;
inline void ScrollMovementSpeed(int speed){
	int i;
	for(i=0;i<speed;i++)PA_WaitForVBL();
}	
void AdjustPixelCursor(void){
	if(Pad.Newpress.Left){
		PixCursorX-=8;
		if(PixCursorX<0){
			scrollx--;
			centerx = scrollx + (zoom >> 1); 
			PixCursorX+=8;
		}
		timer=0;
	}
	else if(Pad.Held.Left){
		timer++;
		if(timer>=HELDTIME){
			timer-=1;
			PixCursorX-=8;
			if(PixCursorX<0){
				scrollx--;
				centerx = scrollx + (zoom >> 1); 
				PixCursorX+=8;
			}
			ScrollMovementSpeed(SCROLLSPEED);
		}
	}
	if(Pad.Newpress.Right){
		PixCursorX+=8;
		if(PixCursorX+8>256){
			scrollx++;
			centerx = scrollx + (zoom >> 1); 
			PixCursorX-=8;	
		}
		timer=0;
	}
	else if(Pad.Held.Right){
		timer++;
		if(timer>=HELDTIME){
			timer-=1;
			PixCursorX+=8;
			if(PixCursorX+8>256){
				scrollx++;
				centerx = scrollx + (zoom >> 1); 
				PixCursorX-=8;
			}
			ScrollMovementSpeed(SCROLLSPEED);
		}
	}
	if(Pad.Newpress.Down){
		PixCursorY+=8;
		if(PixCursorY+8>192){
			scrolly++;
			centery = scrolly + (zoom >> 1); 
			PixCursorY-=8;
		}
		timer=0;
	}
	else if(Pad.Held.Down){
		timer++;
		if(timer>=HELDTIME){
			timer-=1;
			PixCursorY+=8;
			if(PixCursorY+8>192){
				scrolly++;
				centery = scrolly + (zoom >> 1); 
				PixCursorY-=8;
			}
			ScrollMovementSpeed(SCROLLSPEED);
		}
	}
	if(Pad.Newpress.Up){
		PixCursorY-=8;
		if(PixCursorY<0){
			scrolly--;
			centery = scrolly + (zoom >> 1); 
			PixCursorY+=8;
		}
		timer=0;
	}
	else if(Pad.Held.Up){
		timer++;
		if(timer>=HELDTIME){
			timer-=1;
			PixCursorY-=8;
			if(PixCursorY<0){
				scrolly--;
				centery = scrolly + (zoom >> 1); 
				PixCursorY+=8;
			}
			ScrollMovementSpeed(SCROLLSPEED);
		}
	}
	PA_SetSpriteXY (0,0,PixCursorX,PixCursorY);
	CheckScroll();
}

void PickLastColor(void){
	int i;
	for(i=0;i<10;++i){
		int NextY=98+(7*i);
		int EndY=NextY+4;
		if(PA_StylusInZone(3,NextY,17,EndY)){
			StylusOldX=Stylus.X;
			StylusOldY=Stylus.Y;
			color[pal] = PA_Get16bitPixel(1,StylusOldX,StylusOldY);
			UpdateColorSquare();
		}
	}
}
void Status(void){
	if(ShowStats==1){
		HideSprite(1,7,0);
		if(DrawMode==0 && screen==0)PA_OutputText(1,12,11,"X:%d  \nY:%d   ",(((Stylus.X*zoom)>>8)+scrollx),(((Stylus.Y*zoom)>>8)+scrolly));
		else PA_OutputText(1,12,11,"X:%d  \nY:%d   ",(((PixCursorX*zoom)>>8)+scrollx),(((PixCursorY*zoom)>>8)+scrolly));
		//PA_OutputText(1,12,13,"Colors:\n %d",NumOfColors);	
		if(Stylus.Newpress && PA_SpriteTouched(7))ShowStats=0;
	}
	else {
		HideSprite(1,7,1);
		PA_ClearTextBg(1);
	}
}
//Top screen menu ------------------------------------------------------------------------------------------------------------------------------------		
void Tools(bool switchscreen){
	bool toolsscreen=1;
	bool touched=0;
	while(toolsscreen==1){
		//color picker
		if(Stylus.Held){
			if(PA_StylusInZone(25, 3,223,19)){
				StylusOldX=Stylus.X;
				StylusOldY=Stylus.Y;
				color[pal] = PA_Get16bitPixel(1,StylusOldX,StylusOldY);
				UpdateColorSquare();
				CursorX=StylusOldX;
				CursorY=StylusOldY;
				PA_SetSpriteXY (1,5,CursorX,CursorY);
			}
		}
		//If the user touches the screen
		if (Stylus.Newpress) {
			if(ToolBox==0){
				touched=1;
				//changes tool
				//copy tool
				if(PA_StylusInZone(240,30,255,43))UpdateTools(7);
				//pen
				if(PA_StylusInZone(240,44,255,59))UpdateTools(0);
				//bucket
				if(PA_StylusInZone(240,60,255,75))UpdateTools(1);
				//eraser
				if(PA_StylusInZone(240,76,255,91))UpdateTools(2);
				//line
				if(PA_StylusInZone(240,92,255,107))UpdateTools(3);
				//square
				if(PA_StylusInZone(240,108,255,123))UpdateTools(4);
				//circle
				if(PA_StylusInZone(240,124,255,139))UpdateTools(5);
				//circle
				if(PA_StylusInZone(240,140,255,155))UpdateTools(6);
				//new tools
				if(PA_StylusInZone(240,156,255,171)&&touched==1){
					ToolBox=1;
					PA_LoadGifXY(1,239,29,(void*)tools2); 
					PA_WaitForVBL();
				}
			}
			if(ToolBox==1){
				//spray can
				if(PA_StylusInZone(240,30,255,43))UpdateTools(9);
				//type writer
				if(PA_StylusInZone(240,44,255,59))UpdateTools(8);
				//stats
				if(PA_StylusInZone(240,60,255,75)){
					ShowStats++;
					if(ShowStats>=2)ShowStats=0;
				}	
				//stamp
				if(PA_StylusInZone(240,76,255,91))UpdateTools(10);
				if(PA_StylusInZone(240,92,255,107))UpdateTools(11);
				//tool box 0
				if(PA_StylusInZone(240,156,255,171)&&touched==0){
					ToolBox=0;
					//PA_LoadBmpEx (1,239,29, (void *)tools1);
					PA_LoadGifXY(1,239,29,(void*)tools1); 
					PA_WaitForVBL();
				}	
			}
			if(ShowStats==0){
				//clear screen
				if(PA_StylusInZone(26,126,222,172))ClearDrawing(1);
				//save
				if(PA_StylusInZone(26,76,122,122)){
					toolsscreen=0;
					KeyBoardMenu(0);
					
				}
				if(PA_StylusInZone(126,76,222,122)){
					toolsscreen=0;
					LoadMenu();
				}
			}
			//color creator mode
			if(PA_SpriteTouched(4)){
				toolsscreen=0;
				ColorCreator(1,0);
			}
		}
		if(Stylus.Released)touched=0;
		//if the stylus is held on screen
		if(Stylus.Held){
			//used for stuff like the draw size slider
			switch(DrawTool){
				case 0://draw size
					if(Stylus.Y<=74 && Stylus.Y>=35 && Stylus.X>=5 && Stylus.X<=(5+16)){
						PA_SetSpriteY(1,2,Stylus.Y);
						drawsize=(Stylus.Y-35)/2;
						if(drawsize<=0)drawsize++;
						PA_SetDrawSize(0,drawsize);
						UpdateDrawSizeSquare(drawsize);
					}
					//if plus or minus simbles pressed
					//increase draw size
					if(PA_StylusInZone(5,82,19,89)&&Stylus.Newpress){
						drawsize++;
						if(drawsize>=39)drawsize--;
						PA_SetSpriteY(1,2,35+drawsize);
						PA_SetDrawSize(0,drawsize/2);
						UpdateDrawSizeSquare(drawsize/2);
					}		
					//decrease draw size
					if(PA_StylusInZone(5,27,19,34)&&Stylus.Newpress){
						drawsize--;
						if(drawsize<=0)drawsize++;
						PA_SetSpriteY(1,2,35+drawsize);
						PA_SetDrawSize(0,drawsize/2);
						UpdateDrawSizeSquare(drawsize/2);
					}						
				break;
				case 2://eraser size
					if(Stylus.Y<=74 && Stylus.Y>=35 && Stylus.X>=5 && Stylus.X<=(5+16)){
						PA_SetSpriteY(1,2,Stylus.Y);
						erasersize=(Stylus.Y-35)/2;
						if(erasersize<=0)erasersize++;
						PA_SetDrawSize(0,erasersize);
						UpdateDrawSizeSquare(erasersize);
					}
					//increase draw size
					if(PA_StylusInZone(5,82,19,89)&&Stylus.Newpress){
						erasersize++;
						if(erasersize>=39)erasersize--;
						PA_SetSpriteY(1,2,35+erasersize);
						PA_SetDrawSize(0,erasersize/2);
						UpdateDrawSizeSquare(erasersize/2);
					}		
					//decrease draw size
					if(PA_StylusInZone(5,27,19,34)&&Stylus.Newpress){
						erasersize--;
						if(erasersize<=0)erasersize++;
						PA_SetSpriteY(1,2,35+erasersize);
						PA_SetDrawSize(0,erasersize/2);
						UpdateDrawSizeSquare(erasersize/2);
					}	
				break;
				case 3://line size size
					if(Stylus.Y<=74 && Stylus.Y>=35 && Stylus.X>=5 && Stylus.X<=(5+16)){
						PA_SetSpriteY(1,2,Stylus.Y);
						linesize=(Stylus.Y-35)/2;
						if(linesize<=0)linesize++;
						PA_SetDrawSize(0,linesize);
						UpdateDrawSizeSquare(linesize);
					}
					//increase draw size
					if(PA_StylusInZone(5,82,19,89)&&Stylus.Newpress){
						linesize++;
						if(linesize>=39)linesize--;
						PA_SetSpriteY(1,2,35+linesize);
						PA_SetDrawSize(0,linesize/2);
						UpdateDrawSizeSquare(linesize/2);
					}		
					//decrease draw size
					if(PA_StylusInZone(5,27,19,34)&&Stylus.Newpress){
						linesize--;
						if(linesize<=0)linesize++;
						PA_SetSpriteY(1,2,35+linesize);
						PA_SetDrawSize(0,linesize/2);
						UpdateDrawSizeSquare(linesize/2);
					}	
				break;
				case 5://rectangle size
					if(Stylus.Y<=74 && Stylus.Y>=35 && Stylus.X>=5 && Stylus.X<=(5+16)){
						PA_SetSpriteY(1,2,Stylus.Y);
						recsize=(Stylus.Y-35)/2;
						if(recsize<=0)recsize++;
						PA_SetDrawSize(0,recsize);
						UpdateDrawSizeSquare(recsize);
					}
					//increase draw size
					if(PA_StylusInZone(5,82,19,89)&&Stylus.Newpress){
						recsize++;
						if(recsize>=39)recsize--;
						PA_SetSpriteY(1,2,35+recsize);
						PA_SetDrawSize(0,recsize/2);
						UpdateDrawSizeSquare(recsize/2);
					}		
					//decrease draw size
					if(PA_StylusInZone(5,27,19,34)&&Stylus.Newpress){
						recsize--;
						if(recsize<=0)recsize++;
						PA_SetSpriteY(1,2,35+recsize);
						PA_SetDrawSize(0,recsize/2);
						UpdateDrawSizeSquare(recsize/2);
					}				
				case 9://spray size
					
					if(Stylus.Y<=74 && Stylus.Y>=35 && Stylus.X>=5 && Stylus.X<=(5+16)){
						PA_SetSpriteY(1,2,Stylus.Y);
						spraysize=(Stylus.Y-30)/2;
						if(spraysize<5)spraysize++;
						tempsize=spraysize;
					}
					//increase draw size
					if(PA_StylusInZone(5,82,19,89)&&Stylus.Newpress){
						spraysize++;
						if(spraysize*2>39)spraysize--;
						PA_SetSpriteY(1,2,35+spraysize*2);
					}		
					//decrease draw size
					if(PA_StylusInZone(5,27,19,34)&&Stylus.Newpress){
						spraysize--;
						if(spraysize*2<5)spraysize++;
						PA_SetSpriteY(1,2,30+spraysize*2);
					}	
				break;
			}
			//zoom slider
			if(Stylus.Y>=175 && Stylus.X>=16 && Stylus.X<=235){
				PA_SetSpriteX(1,3,Stylus.X);
				zoom=(256-(Stylus.X));
				ZoomBarX=PA_GetSpriteX(1,3);
				CheckZoom();
			}
			PickLastColor();
		}
		Status();
		//pixil mode for top screen
		if(DrawMode==0)CheckZoom();
		if(DrawMode==1 && zoom<=32){
			if(Pad.Newpress.A)CheckSaveUndo();
			if(Pad.Held.A){
				PA_Put16bitPixel(0,(((PixCursorX*zoom)>>8)+scrollx),(((PixCursorY*zoom)>>8)+scrolly),PA_RGB(color[pal]&31, color[pal]>>5&31, color[pal]>>10&31));
			}
			AdjustPixelCursor();
			CheckScroll();
		}
		else DrawMode=0;
		if(Pad.Newpress.X){
			DrawMode++;
			if(DrawMode==1 && zoom<=32){
				HideSprite(0,0,0);
				UpdateTools(0);
			}
			if(DrawMode>=2)DrawMode=0;
			if(DrawMode==0)HideSprite(0,0,1);
		}
		if(Pad.Newpress.Y)CheckUndo();
		if(Pad.Newpress.Start)OptionsMenu();
		
		//change screens back
		switch(options.swaplr){
			case 0:
				if(Pad.Newpress.L){
					screen=0;
					toolsscreen=0;
					PA_SwitchScreens();
					PA_WaitForVBL();PA_WaitForVBL();
					DrawLoop();
				}
				if(Pad.Newpress.R){
					pal++;
					if(pal>=2)pal=0;
					UpdateColorSquare();
				}
			break;
			case 1:
				if(Pad.Newpress.R){
					screen=0;
					toolsscreen=0;
					PA_SwitchScreens();
					PA_WaitForVBL();PA_WaitForVBL();
					DrawLoop();
				}
				if(Pad.Newpress.L){
					pal++;
					if(pal>=2)pal=0;
					UpdateColorSquare();
				}
			break;
		}
		PA_WaitForVBL();
	}
}
