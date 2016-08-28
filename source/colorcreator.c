#include <PA9.h>
#include "all_gfx.h"
#include "header.h"
#include "gradient.h"
#include "clorcreator.h"

//color creator stuff-------------------------------------------------------------------
void ColorCreatorSquares(void){
	u8 x,y;
	switch(pal){
		case 0:
			//back square
			for(x=171; x<207; ++x){
				for(y=56; y<93; ++y){
					PA_Put16bitPixel(0, x, y, PA_RGB(color[1]&31,color[1]>>5&31,color[1]>>10&31));
				}
			}
			//front square
			for(x=149; x<186; ++x){
				for(y=35; y<73; ++y){
					PA_Put16bitPixel(0, x, y, PA_RGB(color[0]&31,color[0]>>5&31,color[0]>>10&31));
				}
			}
		break;
		case 1:
			//back square
			for(x=171; x<207; ++x){
				for(y=56; y<93;++y){
					PA_Put16bitPixel(0, x, y, PA_RGB(color[0]&31,color[0]>>5&31,color[0]>>10&31));
				}
			}
			//front square
			for(x=149; x<186; ++x){
				for(y=35; y<73; ++y){
					PA_Put16bitPixel(0, x, y, PA_RGB(color[1]&31,color[1]>>5&31,color[1]>>10&31));
				}
			}
		break;	
	}
}
inline void TransStorage(void){
	u8 x,y;
	for(x=12;x<39;++x){
		for(y=155;y<176;++y){
			PA_Put16bitPixel(0, x, y, transparency[0]);
		}
	}
}
			
void ColorStorage(int colornum){
	u8 x,y;
	if(colornum<5){
		u8 NextX=48+(33*colornum);
		u8 Endx=NextX+27;
		for(x=NextX;x<Endx;++x){
			for(y=128;y<150;++y){
				PA_Put16bitPixel(0, x, y, tempcolors[colornum]);
			}
		}
	}
	else if(colornum>=5){
		u8 newcolor=colornum-5;
		u8 NextX=48+(33*newcolor);
		u8 Endx=NextX+27;
		for(x=NextX;x<Endx;++x){
			for(y=155;y<176;++y){
				PA_Put16bitPixel(0, x, y, tempcolors[colornum]);
			}
		}	
	}
}
void CheckColorStorage(void){
	u8 i;
	for(i=0;i<10;++i){
		if(i<=4){
			u8 NewX=48+(33*i);
			u8 EndX= NewX+26;
			if(PA_StylusInZone(NewX,128,EndX,149)&&Stylus.Newpress){
				tempcolors[i]=color[pal];
				ColorStorage(i);
			}
		}
		if(i>=5){
			u8 newcolor=i-5;
			u8 NewX=48+(33*newcolor);
			u8 EndX= NewX+26;
			if(PA_StylusInZone(NewX,155,EndX,175)&& Stylus.Newpress){
				tempcolors[i]=color[pal];
				ColorStorage(i);
			}
		}
	}
	if(PA_StylusInZone(12,155,38,175)&& Stylus.Newpress){
		tempcolors[11]=color[pal];
		transparency[0]=color[pal];
		TransStorage();
	}	
}
extern inline void ScanColorStorage(void){
	u8 i;
	for(i=0;i<10;++i)ColorStorage(i);
}

void UpdateSliderPos(void){
	red=color[pal]&31;
	PA_SetSpriteY(0,2,(red*2)+51);
	green=color[pal]>>5&31;
	PA_SetSpriteY(0,3,(green*2)+51);
	blue=color[pal]>>10&31;
	PA_SetSpriteY(0,4,(blue*2)+51);
}

//gradient loads to top screen
void ColorCreator(bool savedraw,bool BackToKeys){
	PA_WaitForVBL();PA_WaitForVBL();
	bool colorcreatormode=1;
	PA_SwitchScreens();
	screen=0;
	
	if(savedraw==1)SaveUndo(MENUBUF);//save image to last slot so we can clear the screen for menu
	
	//clear screens
	PA_ResetBgSysScreen (1);
	PA_ResetBgSysScreen (0);
	PA_ResetSpriteSysScreen(1);
	PA_ResetSpriteSysScreen(0);
	PA_WaitForVBL();PA_WaitForVBL();
	//load backgrounds
	PA_Init16bitBg(1, 3);
	PA_Init16bitBg(0, 3);
	PA_LoadBmpEx (1,0,0, (void *)gradient);
	//PA_LoadBmpEx (0,0,0, (void *)clorcreator);
	PA_LoadGifXY(0,0,0,(void*)clorcreator); 
	ColorCreatorSquares();
	ScanColorStorage();
	TransStorage();

	PA_InitText(0, 0);
	PA_SetTextCol (0,0,0,0);
	
	//load sprites
	//pallet square color holder
	PA_LoadSpritePal(0, 0, (void*)largepals_Pal);
	PA_CreateSprite(0, 0, (void*)largepals_Sprite, OBJ_SIZE_64X64, 1, 0, 148,34);

	
	//loads the bar
	PA_LoadSpritePal(0, 2, (void*)bar_Pal);
	//red
	PA_CreateSprite(0, 2, (void*)bar_Sprite, OBJ_SIZE_16X8, 1, 2,48,51);
	//green
	PA_CreateSprite(0, 3, (void*)bar_Sprite, OBJ_SIZE_16X8, 1, 2,68,51);
	//blue
	PA_CreateSprite(0, 4, (void*)bar_Sprite, OBJ_SIZE_16X8, 1, 2,88,51);	
	//lightness
	//PA_CreateSprite(0, 5, (void*)bar_Sprite, OBJ_SIZE_16X8, 1, 2,121,25);
	
	UpdateSliderPos();
	while(colorcreatormode==1){
		PA_OutputText(0,14,5,"R:%d \n\nG:%d \n\nB:%d ",red,green,blue);
		//color picker
		if(screen==1 && Stylus.Held){
			color[pal]=PA_Get16bitPixel(1,Stylus.X,Stylus.Y);
			Colx=Stylus.X;
			Coly=Stylus.Y;
			UpdateSliderPos();	
			ColorCreatorSquares();
		}
		if(screen==0){
					//exit button
			if(PA_StylusInZone(207,1,249,18)&& Stylus.Newpress){
				if(BackToKeys==0){
					PA_WaitForVBL();PA_WaitForVBL();
					colorcreatormode=0;
					loadgui(1,0,1);
					screen=1;
					PA_SwitchScreens();
					Tools(0);
				}
				else{
					PA_WaitForVBL();PA_WaitForVBL();
					colorcreatormode=0;
					TypeWriterMode(1);
				}
			}
				
			//rbg sliders
			if(Stylus.Held){
				//red
				if(Stylus.Y<=113 && Stylus.Y>=51&& Stylus.X>=46 && Stylus.X<=62){
					PA_SetSpriteY(0,2,Stylus.Y);
					red=(Stylus.Y-51)/2;
					if(red<0)red++;
					if(red>31)red--;
					color[pal]=PA_RGB(red,green,blue);
					ColorCreatorSquares();
				}
				//green
				if(Stylus.Y<=113 && Stylus.Y>=51&& Stylus.X>=66 && Stylus.X<=82){
					PA_SetSpriteY(0,3,Stylus.Y);
					green=(Stylus.Y-51)/2;
					if(green<0)green++;
					if(green>31)green--;
					color[pal]=PA_RGB(red,green,blue);
					ColorCreatorSquares();
				}
				//blue
				if(Stylus.Y<=113 && Stylus.Y>=51&& Stylus.X>=86 && Stylus.X<=102){
					PA_SetSpriteY(0,4,Stylus.Y);
					blue=(Stylus.Y-51)/2;
					if(blue<0)blue++;
					if(blue>31)blue--;
					color[pal]=PA_RGB(red,green,blue);
					ColorCreatorSquares();
				}
				if(PA_StylusInZone(212,113,243,148)&& Stylus.Newpress){
					colorcreatormode=0;
					KeyBoardMenu(1);
				}
				if(PA_StylusInZone(212,155,243,170)&& Stylus.Newpress){
					colorcreatormode=0;		
					LoadPalMenu();
				}
			}
		
				CheckColorStorage();	
		}
		
		//swap screens
		switch(options.swaplr){
			case 0:
				if(Pad.Newpress.L){
					screen++;
					if(screen==2)screen=0;
					PA_WaitForVBL();PA_WaitForVBL();
					PA_SwitchScreens();
				}
				if(Pad.Newpress.R){
					pal++;
					if(pal>=2)pal=0;
					ColorCreatorSquares();
				}
			break;
			case 1:
				if(Pad.Newpress.R){
					screen++;
					if(screen==2)screen=0;
					PA_WaitForVBL();PA_WaitForVBL();
					PA_SwitchScreens();
				}
				if(Pad.Newpress.L){
					pal++;
					if(pal>=2)pal=0;
					ColorCreatorSquares();
				}
			break;
		}
		
		PA_WaitForVBL();	
	}
}
