
#include <PA9.h>
#include "header.h"
#include "all_gfx.h"

void LoadCanvas(bool save){
	//sets background color
	u16 x,y;
	for(x=0; x<256; x++){
		for(y=0; y<192; y++){
			PA_Put16bitPixel(0, x, y, PA_RGB(27,27,27));
		}
	}
	//creates drawable square
	Canvas.X=128-(Canvas.drawwidth/2);
	Canvas.Y=96-(Canvas.drawheight/2);
	
	for(x=Canvas.X; x<Canvas.X+Canvas.drawwidth; x++){
		for(y=Canvas.Y; y<Canvas.Y+Canvas.drawheight; y++){
			PA_Put16bitPixel(0, x, y, PA_RGB(31,31,31));
		}
	}
	if(save==1)SaveUndo(MENUBUF);
	centerx = scrollx + (zoom >> 1); 
	centery = scrolly + (zoom >> 1); 
	
}

inline void ClearGreyAreas(void){
	//clear top area
	PA_Draw16bitRect (0,0,0,256,Canvas.Y,PA_RGB(27,27,27));
	//clear bottom area
	PA_Draw16bitRect (0,0,Canvas.Y+Canvas.drawheight,256,192,PA_RGB(27,27,27));
	//clear left area
	PA_Draw16bitRect (0,0,0,Canvas.X,192,PA_RGB(27,27,27));
	//clear right area
	PA_Draw16bitRect (0,Canvas.X+Canvas.drawwidth,0,256,192,PA_RGB(27,27,27));	
}

void CSizeMenu(bool screen,bool firststart){
	bool canvasmenu=1;
	SaveUndo(MENUBUF);
	PA_SetBrightness (1,0);
	PA_SetBrightness (0,0);
	PA_ResetBgSysScreen (screen);
	PA_ResetSpriteSysScreen(screen);
	PA_EasyBgLoad(screen, 3,loadbg); 
	PA_EasyBgLoad(screen, 2,canvas);
    PA_InitText(screen, 0);	
	
	//loads slider for Height
	PA_LoadSpritePal(screen, 2, (void*)bar_Pal);
	PA_CreateSprite(screen, 6, (void*)bar_Sprite, OBJ_SIZE_16X8, 1, 2,17,58);	
	//Loads slider for width
	PA_LoadSpritePal(screen, 3, (void*)zoombar_Pal);
	PA_CreateSprite(screen, 7, (void*)zoombar_Sprite, OBJ_SIZE_8X16, 1, 3, 69,179-15);
	
	PA_SetSpriteY(screen,6,(Canvas.drawheight+116)/2);
	PA_SetSpriteX(screen,7,(Canvas.drawwidth+136)/2);

	PA_OutputText(screen,15,10,"%d   ",Canvas.drawwidth);
	PA_OutputText(screen,15,14,"%d   ",Canvas.drawheight);
	
	PA_SetTextCol (screen,0,0,0);
	
	while(canvasmenu==1){
		if(Stylus.Held){
			//height slider
			if(Stylus.Y>=58 && Stylus.Y<=159 && Stylus.X>=16 && Stylus.X<=31){
				PA_SetSpriteY(screen,6,Stylus.Y);
				Canvas.TempHeight=(Stylus.Y-58)*2;
				if(Canvas.TempHeight>192)Canvas.TempHeight=192;
				if(Canvas.TempHeight<1)Canvas.TempHeight=1;
			}
			//width
			if(Stylus.Y>=165 && Stylus.Y<=180 && Stylus.X>=68 && Stylus.X<=197){
				PA_SetSpriteX(screen,7,Stylus.X);
				Canvas.TempWidth=(Stylus.X-68)*2;
				if(Canvas.TempWidth>256)Canvas.TempWidth=256;
				if(Canvas.TempWidth<1)Canvas.TempWidth=1;
			}
			PA_OutputText(screen,15,10,"%d   ",Canvas.TempWidth);
			PA_OutputText(screen,15,14,"%d   ",Canvas.TempHeight);
		}	
		if(Stylus.Newpress){
			if(PA_StylusInZone(15,29,30,42)){Canvas.TempHeight--;PA_SetSpriteY(screen,2,(Canvas.TempHeight+116)/2);}
			if(PA_StylusInZone(15,169,30,182)){Canvas.TempHeight++;PA_SetSpriteY(screen,2,(Canvas.TempHeight+116)/2);}
			if(PA_StylusInZone(39,166,52,181)){Canvas.TempWidth--;PA_SetSpriteX(screen,3,(Canvas.TempWidth+136)/2);}
			if(PA_StylusInZone(212,166,225,181)){Canvas.TempWidth++;PA_SetSpriteX(screen,3,(Canvas.TempWidth+136)/2);}
			
			if(Canvas.TempWidth>256)Canvas.TempWidth=256;
			if(Canvas.TempWidth<1)Canvas.TempWidth=1;
			
			if(Canvas.TempHeight>192)Canvas.TempHeight=192;
			if(Canvas.TempHeight<1)Canvas.TempHeight=1;
		
			//exit button
			if(PA_StylusInZone(207,1,249,18)&&firststart==0){
				canvasmenu=0;
				PA_WaitForVBL();PA_WaitForVBL();
				loadgui(0,0,0);
				if(screen==1)Tools(0);
				
				
			}
			//ok
			if(PA_StylusInZone(191,78,226,99)){
				canvasmenu=0;
				Canvas.drawheight=Canvas.TempHeight;
				Canvas.drawwidth=Canvas.TempWidth;
				PA_WaitForVBL();PA_WaitForVBL();
				loadgui(1,0,0);
				CheckSaveUndo();
				SaveUndo(MENUBUF);
				if(screen==1)Tools(0);
				
			}
			//stamp
			if(PA_StylusInZone(182,111,236,135)){
				Canvas.TempHeight=16;
				PA_SetSpriteY(screen,6,(Canvas.TempHeight+116)/2);
				Canvas.TempWidth=16;
				PA_SetSpriteX(screen,7,(Canvas.TempWidth+136)/2);
			}
			PA_OutputText(screen,15,10,"%d   ",Canvas.TempWidth);
			PA_OutputText(screen,15,14,"%d   ",Canvas.TempHeight);
		}
		PA_WaitForVBL();
	}
}

