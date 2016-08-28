#include <PA9.h>  
#include "header.h"


void PA_ResetBgLayer(u8 screen, u8 bg_select){
  PA_DeleteBg(screen, bg_select);
  PA_BgInfo[screen][bg_select].tilesetsize = 0;
  PA_BgInfo[screen][bg_select].mapsize = 0;
  PA_BgInfo[screen][bg_select].TileSetChar = 0;
  PA_BgInfo[screen][bg_select].mapchar = 0;
  PA_parallaxX[screen][bg_select] = 0;
  PA_parallaxY[screen][bg_select] = 0;
  scrollpos[screen][bg_select].infscroll = 0;
  return;
}

inline bool CanvasLimits(void){
	if(((((Stylus.X*zoom)>>8)+scrollx)>=Canvas.X)&&((((Stylus.X*zoom)>>8)+scrollx)<=Canvas.X+Canvas.drawwidth)){
		if(((((Stylus.Y*zoom)>>8)+scrolly)>=Canvas.Y)&&((((Stylus.Y*zoom)>>8)+scrolly)<=Canvas.Y+Canvas.drawheight)){
			return 1;
		}
	}
	return 0;
}
inline bool CursorCanvasLimits(void){
	if(((((PixCursorX*zoom)>>8)+scrollx)>=Canvas.X)&&((((PixCursorX*zoom)>>8)+scrollx)<Canvas.X+Canvas.drawwidth)){
		if(((((PixCursorY*zoom)>>8)+scrolly)>=Canvas.Y)&&((((PixCursorY*zoom)>>8)+scrolly)<Canvas.Y+Canvas.drawheight)){
			return 1;
		}
	}
	return 0;
}

inline void HideSprite(bool screen, int sprite,bool hide){
	#define ATTR0_DISABLED   (2<<8) 
	switch(hide){
		case 0://show sprite
			PA_obj[screen][sprite].atr0 &= ~ATTR0_DISABLED;
		break;
		case 1://hide sprite
			PA_obj[screen][sprite].atr0 |= ATTR0_DISABLED;
		break;
	}
}

void Copy(bool screen,int x1,int x2,int y1,int y2,int slot){

	u16 checkX=0;
	u16 checkY=0;
	u16 pixel=0;
	//start from top left to bottom right
	if(x2>x1 && y1<y2){
		CnP.width[slot]=x2-x1;
		CnP.height[slot]=y2-y1;
		for(checkX=x1;checkX<x2;++checkX){
			for(checkY=y1;checkY<y2;++checkY){
				CnP.copy[slot][pixel]=PA_Get16bitPixel(screen, checkX, checkY);
				pixel++;
			}
			pixel++;
		}
	}
	//from bottom left to top right
	if(x2>x1 && y1>y2){
		CnP.width[slot]=x2-x1;
		CnP.height[slot]=y1-y2;
		for(checkX=x1;checkX<x2;++checkX){
			for(checkY=y2;checkY<y1;++checkY){
				CnP.copy[slot][pixel]=PA_Get16bitPixel(screen, checkX, checkY);
				pixel++;
			}
			pixel++;
		}
	}
	//start from top right to bottom left
	if(x2<x1 && y1<y2){
		CnP.width[slot]=x1-x2;
		CnP.height[slot]=y2-y1;
		for(checkX=x2;checkX<x1;++checkX){
			for(checkY=y1;checkY<y2;++checkY){
				CnP.copy[slot][pixel]=PA_Get16bitPixel(screen, checkX, checkY);
				pixel++;
			}
			pixel++;
		}
	}
	//from bottom right to top left
	if(x2<x1 && y1>y2){
		CnP.width[slot]=x1-x2;
		CnP.height[slot]=y1-y2;
		for(checkX=x2;checkX<x1;++checkX){
			for(checkY=y2;checkY<y1;++checkY){
				CnP.copy[slot][pixel]=PA_Get16bitPixel(screen, checkX, checkY);
				pixel++;
			}
			pixel++;
		}
	}		
			
}
void Paste(bool screen,int x,int y,bool xflip,bool yflip,int slot){
	s16 checkX=0;
	s16 checkY=0;
	s16 pixel=0;
	//no flip
	if(xflip==0 && yflip==0){
		for(checkX=x;checkX<x+CnP.width[slot];++checkX){
			for(checkY=y;checkY<y+CnP.height[slot];++checkY){
				if(CnP.copy[slot][pixel]!=transparency[slot])PA_Put16bitPixel (screen,checkX,checkY,CnP.copy[slot][pixel]);
				pixel++;
			}
			pixel++;
		}
	}
	//x flip
	if(xflip==1 && yflip==0){
		for(checkX=x+CnP.width[slot];checkX>x;--checkX){
			for(checkY=y;checkY<y+CnP.height[slot];++checkY){
				if(CnP.copy[slot][pixel]!=transparency[slot])PA_Put16bitPixel (screen,checkX,checkY,CnP.copy[slot][pixel]);
				pixel++;
			}
			pixel++;
		}
	}
	//yflip
	if(xflip==0 && yflip==1){
		for(checkX=x;checkX<x+CnP.width[slot];++checkX){
			for(checkY=y+CnP.height[slot];checkY>y;--checkY){
				if(CnP.copy[slot][pixel]!=transparency[slot])PA_Put16bitPixel (screen,checkX,checkY,CnP.copy[slot][pixel]);
				pixel++;
			}
			pixel++;
		}
	}
	//y and x flip
	if(xflip==1 && yflip==1){
		for(checkX=x+CnP.width[slot];checkX>x;--checkX){
			for(checkY=y+CnP.height[slot];checkY>y;--checkY){
				if(CnP.copy[slot][pixel]!=transparency[slot])PA_Put16bitPixel (screen,checkX,checkY,CnP.copy[slot][pixel]);
				pixel++;
			}
			pixel++;
		}
	}
}

inline void CheckFlips(bool screen,int slot){
	if(CnP.yflip[slot]>=2)CnP.yflip[slot]=0;
	if(CnP.xflip[slot]>=2)CnP.xflip[slot]=0;
	Paste(screen,CnP.OldStylusX,CnP.OldStylusY,CnP.xflip[slot],CnP.yflip[slot],slot);
}
	


//captures screen before undo
inline void SaveUndo(int slot){
	DMA_Copy(PA_DrawBg[0], undo[slot], 256*192, DMA_16NOW);
}
//makes sure that everything saves in the correct place
inline void CheckSaveUndo(void){
	++EndSlot;
	if(EndSlot>UNDOSLOTEND )EndSlot=0;
	if(EndSlot==StartSlot){
		StartSlot++;
	}
	SaveUndo(EndSlot);
	if(StartSlot>UNDOSLOTEND)StartSlot=0;
}
//undo stuff
inline void UndoSlot(int slot){
	DMA_Copy(undo[slot], PA_DrawBg[0], 256*192, DMA_16NOW);  
}
//makes sure to undo specific parts
inline void CheckUndo(void){
	if(EndSlot!=StartSlot){
		UndoSlot(EndSlot);
		--EndSlot;
		if(EndSlot<0)EndSlot=UNDOSLOTEND;
	}
}

void UpdateLastPal(int pallete){
	u16 x;
	u16 y;
	u16 NextY=98+(7*pallete);
	u16 EndY=NextY+4;
	for(x=3; x<17; ++x){
		for(y=NextY; y<EndY; ++y){
			PA_Put16bitPixel(1, x, y,tempcolors[pallete]);
		}
	}
}
void ClearPal(int pallete){
	u16 x;
	u16 y;
	u16 NextY=98+(7*pallete);
	u16 EndY=NextY+4;
	for(x=3; x<17; ++x){
		for(y=NextY; y<EndY; ++y){
			PA_Put16bitPixel(1, x, y, PA_RGB(31,31,31));
		}
	}
}
void ClearAllPalletes(void){
	u8 i;
	for(i=0;i<10;++i){
		ClearPal(i);
		tempcolors[i]=PA_RGB(31,31,31);
	}

}

void CheckPalletes(void){
	u8 i;
	for(i=0;i<10;++i){
		UpdateLastPal(i);
	}
}
//erases screen
void ClearDrawing(bool save){
	if(save==1){
		CheckSaveUndo();
		SaveUndo(MENUBUF);
	}
	u16 x,y;
	for(x=Canvas.X; x<Canvas.X+Canvas.drawwidth; ++x){
		for(y=Canvas.Y; y<Canvas.Y+Canvas.drawheight; ++y){
			PA_Put16bitPixel(0, x, y, PA_RGB(31,31,31));
		}
	}
}	

//custom draw function
void Draw(u8 screen, u16 x, u16 y, u16 color,bool undo){
	s16 i, j, low, high;
	if(Stylus.Newpress){
		if(undo==1)CheckSaveUndo();
		CheckPalletes();
	}
		
	if (Stylus.Held){
		i = 0;
		j = 0;
		
		low = (PA_drawsize[screen] >> 1) - PA_drawsize[screen]+1;
		high = (PA_drawsize[screen] >> 1)+1;
	
		// Si nouvelle pression, on fait juste un point. Sinon, on trace un trait entre les 2 points...
			if (Stylus.Newpress) {
				for (i = low; i < high; ++i)
					for (j = low; j < high; ++j)
						if ((x+i > 0) && (y+j > 0) && (x+i < 256) && (y+j < 192))
							PA_Put16bitPixel(screen, x+i, y+j, color);
			}
			else {

				PA_Draw16bitLineEx(screen, x, y, PA_oldx[screen], PA_oldy[screen], color, PA_drawsize[screen]);
			}
		
		PA_oldx[screen] = x; PA_oldy[screen] = y;
	}
	
}
//line fucntion
inline void Line(u8 screen, u16 x,u16 y,u16 color,bool undo){
	if(Stylus.Newpress){
		if(undo==1)CheckSaveUndo();
		CheckPalletes();
		Draw(screen,x,y,color,0);
		while(Stylus.Held){
			UndoSlot(EndSlot);
			PA_Draw16bitLineEx (screen,PA_oldx[screen],PA_oldy[screen],(((Stylus.X*zoom)>>8)+scrollx),(((Stylus.Y*zoom)>>8)+scrolly),color,PA_drawsize[screen]);
			PA_WaitForVBL();
		}
	}
}
void UnfilledRectTool(bool screen, s16 x,s16 y, u16 color,bool undo){
	if(Stylus.Newpress){
		if(undo==1) CheckSaveUndo();
		CheckPalletes();
		PA_oldx[screen] = x; PA_oldy[screen] = y;
		while(Stylus.Held){
			if(CanvasLimits()==TRUE){
				UndoSlot(EndSlot);
				//top line
				PA_Draw16bitLineEx (screen,PA_oldx[screen],PA_oldy[screen],(((Stylus.X*zoom)>>8)+scrollx),PA_oldy[screen],color,PA_drawsize[screen]);
				//bottom line
				PA_Draw16bitLineEx (screen,PA_oldx[screen],(((Stylus.Y*zoom)>>8)+scrolly),(((Stylus.X*zoom)>>8)+scrollx),(((Stylus.Y*zoom)>>8)+scrolly),color,PA_drawsize[screen]);
				//left line
				PA_Draw16bitLineEx (screen,PA_oldx[screen],PA_oldy[screen],PA_oldx[screen],(((Stylus.Y*zoom)>>8)+scrolly),color,PA_drawsize[screen]);
				//right line
				PA_Draw16bitLineEx (screen,(((Stylus.X*zoom)>>8)+scrollx),PA_oldy[screen],(((Stylus.X*zoom)>>8)+scrollx),(((Stylus.Y*zoom)>>8)+scrolly),color,PA_drawsize[screen]);
				Sx1=PA_oldx[0];
				Sx2=(((Stylus.X*zoom)>>8)+scrollx);
				Sy1=PA_oldy[0];
				Sy2=(((Stylus.Y*zoom)>>8)+scrolly);
			}
			PA_WaitForVBL();
		}
	}
}

//rectangle function
//copy and paste tool
void UnfilledRectangle(bool screen,u16 x,u16 y, u16 color){
		if(Stylus.Newpress){//if sprite touched
			//and there is a picture in the buffer
			if(CnP.hasCopy==1 && !PA_SpriteTouched(3) &&!PA_SpriteTouched(4)){
				PA_SetSpriteAnimEx (0,3,32,16,1,2);
				if(options.staticsprites==0){
					if(Stylus.X<128){
						PA_SetSpriteXY (0,3,(((Stylus.X*zoom)>>8)+scrollx)+64,(((Stylus.Y*zoom)>>8)+scrolly));
						PA_SetSpriteXY (0,4,(((Stylus.X*zoom)>>8)+scrollx)+64,(((Stylus.Y*zoom)>>8)+scrolly)+16);
					}
					else{
						PA_SetSpriteXY (0,3,(((Stylus.X*zoom)>>8)+scrollx)-64,(((Stylus.Y*zoom)>>8)+scrolly));
						PA_SetSpriteXY (0,4,(((Stylus.X*zoom)>>8)+scrollx)-64,(((Stylus.Y*zoom)>>8)+scrolly)+16);
					}
				}
				else if(options.staticsprites==1){
					if(Stylus.X<128){
						PA_SetSpriteXY (0,3,224,0);
						PA_SetSpriteXY (0,4,224,16);
					}
					if(Stylus.X>128){
						PA_SetSpriteXY (0,3,0,0);
						PA_SetSpriteXY (0,4,0,16);
					}
				}
			}
			//else continue copying as normal
			else {
				SaveUndo(MENUBUF);
				CheckSaveUndo();
				//updates old position so everything pastes in the right area
				PA_oldx[screen] = x; PA_oldy[screen] = y;
				CnP.useCopy=1;
			}
		}
		if (CnP.hasCopy==1 && Stylus.Newpress){
			//paste
			SaveUndo(MENUBUF);
			CnP.hasCopy=3;
			CnP.useCopy=3;
			PA_WaitForVBL();
		}
		
		while(Stylus.Held && CnP.useCopy==1 && CnP.hasCopy!=1 && CnP.hasCopy!=3){
			if(CanvasLimits()==TRUE){
				UndoSlot(MENUBUF);
				//top line
				PA_Draw16bitLineEx (screen,PA_oldx[screen],PA_oldy[screen],(((Stylus.X*zoom)>>8)+scrollx),PA_oldy[screen],color,PA_drawsize[screen]);
				//bottom line
				PA_Draw16bitLineEx (screen,PA_oldx[screen],(((Stylus.Y*zoom)>>8)+scrolly),(((Stylus.X*zoom)>>8)+scrollx),(((Stylus.Y*zoom)>>8)+scrolly),color,PA_drawsize[screen]);
				//left line
				PA_Draw16bitLineEx (screen,PA_oldx[screen],PA_oldy[screen],PA_oldx[screen],(((Stylus.Y*zoom)>>8)+scrolly),color,PA_drawsize[screen]);
				//right line
				PA_Draw16bitLineEx (screen,(((Stylus.X*zoom)>>8)+scrollx),PA_oldy[screen],(((Stylus.X*zoom)>>8)+scrollx),(((Stylus.Y*zoom)>>8)+scrolly),color,PA_drawsize[screen]);
				Sx1=PA_oldx[0];
				Sx2=(((Stylus.X*zoom)>>8)+scrollx);
				Sy1=PA_oldy[0];
				Sy2=(((Stylus.Y*zoom)>>8)+scrolly);
			}
			PA_WaitForVBL();
		}
		if(Stylus.Released && CnP.useCopy==1 && CnP.hasCopy!=1 && CnP.hasCopy!=3){
			if(options.staticsprites==0){
				PA_SetSpriteXY (0,1,Sx1,Sy1);
				PA_SetSpriteXY (0,4,Sx1,Sy1+16);
				HideSprite(0,1,0);
				if(CnP.hasCopy==2){
					PA_SetSpriteXY (0,3,Sx1,Sy1+16);
					PA_SetSpriteXY (0,4,Sx1,Sy1+32);
					HideSprite(0,3,0);
				}
			}
			else if(options.staticsprites==1){
				if(Stylus.X<128){
					PA_SetSpriteXY (0,1,224,0);
					PA_SetSpriteXY (0,4,224,16);
					HideSprite(0,1,0);
					if(CnP.hasCopy==2){
						PA_SetSpriteXY (0,3,224,16);
						PA_SetSpriteXY (0,4,224,32);
						HideSprite(0,3,0);
					}				
					HideSprite(0,4,0);
					CnP.useCopy=2;
				}
				else if(Stylus.X>128){
					PA_SetSpriteXY (0,1,0,0);
					PA_SetSpriteXY (0,4,0,16);
					HideSprite(0,1,0);
					if(CnP.hasCopy==2){
						PA_SetSpriteXY (0,3,0,16);
						PA_SetSpriteXY (0,4,0,32);
						HideSprite(0,3,0);
					}				

				}
			}
			HideSprite(0,4,0);
			CnP.useCopy=2;
		}
}
void TextBoarder(bool screen, int x,int y,u16 color, bool mode){	
	if(CanvasLimits()==TRUE){
		if (Stylus.Newpress&&mode==1){
			CheckSaveUndo();
			SaveUndo(MENUBUF);
			Draw(screen,x,y,color,0);
			PA_WaitForVBL();
		}
		if (Stylus.Newpress&&mode==0){
			CheckSaveUndo();
			SaveUndo(MENUBUF);
			//Draw(screen,x,y,color,0);
			PA_oldx[0]=Stylus.X;
			PA_oldy[0]=Stylus.Y;
			PA_WaitForVBL();
		}
		while(Stylus.Held ){
			if(CanvasLimits()==TRUE){
				UndoSlot(MENUBUF);
				//top line
				PA_Draw16bitLineEx (screen,PA_oldx[screen],PA_oldy[screen],(((Stylus.X*zoom)>>8)+scrollx),PA_oldy[screen],color,PA_drawsize[screen]);
				//bottom line
				PA_Draw16bitLineEx (screen,PA_oldx[screen],(((Stylus.Y*zoom)>>8)+scrolly),(((Stylus.X*zoom)>>8)+scrollx),(((Stylus.Y*zoom)>>8)+scrolly),color,PA_drawsize[screen]);
				//left line
				PA_Draw16bitLineEx (screen,PA_oldx[screen],PA_oldy[screen],PA_oldx[screen],(((Stylus.Y*zoom)>>8)+scrolly),color,PA_drawsize[screen]);
				//right line
				PA_Draw16bitLineEx (screen,(((Stylus.X*zoom)>>8)+scrollx),PA_oldy[screen],(((Stylus.X*zoom)>>8)+scrollx),(((Stylus.Y*zoom)>>8)+scrolly),color,PA_drawsize[screen]);
				Sx1=PA_oldx[0];
				Sx2=(((Stylus.X*zoom)>>8)+scrollx);
				Sy1=PA_oldy[0];
				Sy2=(((Stylus.Y*zoom)>>8)+scrolly);
			}
			PA_WaitForVBL();
		}
		if(mode==1 &&Stylus.Released){
			type.x1=Sx1;
			type.x2=Sx2;
			type.y1=Sy1;
			type.y2=Sy2;
			TypeWriterMode(0);
		}
		if(mode==0&&Stylus.Released){
			type.x1=Sx1;
			type.x2=Sx2;
			type.y1=Sy1;
			type.y2=Sy2;
		}		
	}
}
	
inline void Rectangle(u8 screen,u16 x,u16 y,u16 color, bool undo){
	if(Stylus.Newpress){
	   if(undo==1) CheckSaveUndo();
		CheckPalletes();
		Draw(screen,x,y,color,0);
		
		while(Stylus.Held){
			UndoSlot(EndSlot);
			PA_Draw16bitRect (screen,PA_oldx[screen],PA_oldy[screen],(((Stylus.X*zoom)>>8)+scrollx),(((Stylus.Y*zoom)>>8)+scrolly),color);			
			PA_WaitForVBL();
		}
	}
}

s16 radius,tempx,tempy;
inline void draw16BitCircle(u8 screen, u16 x0, u16 y0, u16 colour,bool undo) {
	if(Stylus.Newpress){
		if(undo==1)CheckSaveUndo();
		PA_WaitForVBL();PA_WaitForVBL();
		CheckPalletes();
		//Draw(screen,x0,y0,colour,0);
		PA_oldx[screen] = x0; PA_oldy[screen] = y0;
		tempy=y0;
		tempx=x0;
		radius=0;
	}
	if(Stylus.Held){
		if(x0>PA_oldx[screen])radius=(x0-PA_oldx[screen]);
		else if(x0<PA_oldx[screen])radius=(PA_oldx[screen]-x0);
		s16 f = 1 - radius;
		s16 ddF_x = 0;
		s16 ddF_y = -2 * radius;
		s16 x = 0;
		s16 y = radius;
		UndoSlot(EndSlot);
		if(x0>PA_oldx[screen]){
			PA_Put16bitPixel(screen, tempx+radius,tempy + radius, colour);
			PA_Put16bitPixel(screen, tempx+radius,tempy - radius, colour);
			PA_Put16bitPixel(screen, tempx + radius*2,tempy, colour);
			PA_Put16bitPixel(screen, tempx , tempy, colour);
		}
		if(x0<PA_oldx[screen]){
			PA_Put16bitPixel(screen, tempx-radius,tempy + radius, colour);
			PA_Put16bitPixel(screen, tempx-radius,tempy - radius, colour);
			PA_Put16bitPixel(screen, tempx,tempy, colour);
			PA_Put16bitPixel(screen, tempx -radius*2, tempy, colour);
		}
		//- radius
		for(x=0;x < y;x++) {
			if (f >= 0) {
				y--;
				ddF_y += 2;
				f += ddF_y;
			}
			//x++;
			ddF_x += 2;
			f += ddF_x + 1;   
			PA_Put16bitPixel(screen, x0 + x, tempy + y, colour);
			PA_Put16bitPixel(screen, x0 - x, tempy + y, colour);
			PA_Put16bitPixel(screen, x0 + x, tempy - y, colour);
			PA_Put16bitPixel(screen, x0 - x, tempy - y, colour);
			PA_Put16bitPixel(screen, x0 + y, tempy + x, colour);
			PA_Put16bitPixel(screen, x0 - y, tempy + x, colour);
			PA_Put16bitPixel(screen, x0 + y, tempy - x, colour);
			PA_Put16bitPixel(screen, x0 - y, tempy - x, colour);	
		}
		
	}
}

inline void AddFill(s16 x, s16 y){
	Bucket.nextx[Bucket.nextfill] = x;
	Bucket.nexty[Bucket.nextfill] = y;   
   Bucket.nextfill++;
   PA_Put16bitPixel(0, x, y, Bucket.newcolor);
}   

inline void DoFill(s16 x, s16 y){
	if((x > 0)&&(PA_Get16bitPixel(0, x-1, y) == Bucket.oldcolor)) AddFill(x-1, y);
	if((x < 255)&&(PA_Get16bitPixel(0, x+1, y) == Bucket.oldcolor)) AddFill(x+1, y);		
	if((y > 0)&&(PA_Get16bitPixel(0, x, y-1) == Bucket.oldcolor)) AddFill(x, y-1);
	if((y < 191)&&(PA_Get16bitPixel(0, x, y+1) == Bucket.oldcolor)) AddFill(x, y+1);	
}   

void CheckFill(void){
	if(Stylus.Newpress){
		CheckSaveUndo();
		CheckPalletes();
		s32 x = ((Stylus.X*zoom)>>8)+scrollx; // adjust according to zoom...
		s32 y = ((Stylus.Y*zoom)>>8)+scrolly; // adjust according to zoom...		 
		Bucket.oldcolor = PA_Get16bitPixel(0, x, y);
		Bucket.newcolor = color[pal];
		PA_WaitForVBL();
		Bucket.nextfill = 0;
		if(Bucket.oldcolor!=Bucket.newcolor){
			AddFill(x, y);
			s32 i;
			for(i = 0; i < Bucket.nextfill; ++i){
				DoFill(Bucket.nextx[i], Bucket.nexty[i]);
			}  
		}
		PA_WaitForVBL();
	}
} 
void SprayCan(int x, int y, int size,int color){
	if(Stylus.Newpress){
		CheckSaveUndo();
		PA_oldx[0]=Stylus.X;
		PA_oldy[0]=Stylus.Y;	
	}
	if(Stylus.Held){
		u8 i;
		for(i=0;i<12;++i){	
			PA_Put16bitPixel(0,x+PA_RandMax (size),y+PA_RandMax (size), color);
		}
	}
}

void StampOptions(void){
	//create all buttons and paste stamp first
	if(stamp==1){						
		CheckSaveUndo();
		CnP.yflip[1]=0;
		CnP.xflip[1]=0;
		HideSprite(0,3,0);
		HideSprite(0,4,0);
		HideSprite(0,5,0);
		HideSprite(0,2,0);
		if(options.staticsprites==0){
			if(Stylus.X>128){
				PA_SetSpriteXY (0,3,(((Stylus.X*zoom)>>8)+scrollx)-64,(((Stylus.Y*zoom)>>8)+scrolly));
				PA_SetSpriteXY (0,4,(((Stylus.X*zoom)>>8)+scrollx)-64,(((Stylus.Y*zoom)>>8)+scrolly)+16);
				PA_SetSpriteXY (0,5,(((Stylus.X*zoom)>>8)+scrollx)-64,(((Stylus.Y*zoom)>>8)+scrolly)+32);
				PA_SetSpriteXY (0,2,(((Stylus.X*zoom)>>8)+scrollx)-64,(((Stylus.Y*zoom)>>8)+scrolly)+48);
			}
			if(Stylus.X<128){
				PA_SetSpriteXY (0,3,(((Stylus.X*zoom)>>8)+scrollx)+64,(((Stylus.Y*zoom)>>8)+scrolly));
				PA_SetSpriteXY (0,4,(((Stylus.X*zoom)>>8)+scrollx)+64,(((Stylus.Y*zoom)>>8)+scrolly)+16);
				PA_SetSpriteXY (0,5,(((Stylus.X*zoom)>>8)+scrollx)+64,(((Stylus.Y*zoom)>>8)+scrolly)+32);
				PA_SetSpriteXY (0,2,(((Stylus.X*zoom)>>8)+scrollx)+64,(((Stylus.Y*zoom)>>8)+scrolly)+48);
			}
		}
		else if(options.staticsprites==1){
			if(Stylus.X<128){
				PA_SetSpriteXY (0,3,224,0);
				PA_SetSpriteXY (0,4,224,16);
				PA_SetSpriteXY (0,5,224,32);
				PA_SetSpriteXY (0,2,224,48);
			}
			if(Stylus.X>128){
				PA_SetSpriteXY (0,3,0,0);
				PA_SetSpriteXY (0,4,0,16);
				PA_SetSpriteXY (0,5,0,32);
				PA_SetSpriteXY (0,2,0,48);
			}
		}
		Paste(0,(((Stylus.X*zoom)>>8)+scrollx)-16,(((Stylus.Y*zoom)>>8)+scrolly)-16,CnP.xflip[1],CnP.yflip[1],1);
		stamp++;
	}
	//decide what the user wants to do
	if(stamp==2){
		//cancel
		if(Stylus.Newpress&&PA_SpriteTouched(4)){
			UndoSlot(MENUBUF);
			stamp=0;
			HideSprite(0,3,1);
			HideSprite(0,4,1);
			HideSprite(0,5,1);
			HideSprite(0,2,1);
			PA_WaitForVBL();
		}
		//paste
		if(Stylus.Newpress&&PA_SpriteTouched(3)){
			stamp=0;
			HideSprite(0,5,1);
			HideSprite(0,2,1);
			HideSprite(0,3,1);
			HideSprite(0,4,1);
			PA_WaitForVBL();
		}
		//Xflip
		if(Stylus.Newpress&&PA_SpriteTouched(5)){
			CnP.xflip[1]++;
			UndoSlot(EndSlot);
			CheckFlips(0,1);
			PA_WaitForVBL();
		}
		//Yflip
		if(Stylus.Newpress&&PA_SpriteTouched(2)){
			CnP.yflip[1]++;
			UndoSlot(EndSlot);
			CheckFlips(0,1);
			PA_WaitForVBL();
		}
		if(Stylus.Held && !PA_SpriteTouched(3) &&!PA_SpriteTouched(4) &&!PA_SpriteTouched(2)&&!PA_SpriteTouched(5)){
			UndoSlot(EndSlot);
			if(options.staticsprites==0){
				if(Stylus.X>128){
					PA_SetSpriteXY (0,3,(((Stylus.X*zoom)>>8)+scrollx)-64,(((Stylus.Y*zoom)>>8)+scrolly));
					PA_SetSpriteXY (0,4,(((Stylus.X*zoom)>>8)+scrollx)-64,(((Stylus.Y*zoom)>>8)+scrolly)+16);
					PA_SetSpriteXY (0,5,(((Stylus.X*zoom)>>8)+scrollx)-64,(((Stylus.Y*zoom)>>8)+scrolly)+32);
					PA_SetSpriteXY (0,2,(((Stylus.X*zoom)>>8)+scrollx)-64,(((Stylus.Y*zoom)>>8)+scrolly)+48);
				}
				if(Stylus.X<128){
					PA_SetSpriteXY (0,3,(((Stylus.X*zoom)>>8)+scrollx)+64,(((Stylus.Y*zoom)>>8)+scrolly));
					PA_SetSpriteXY (0,4,(((Stylus.X*zoom)>>8)+scrollx)+64,(((Stylus.Y*zoom)>>8)+scrolly)+16);
					PA_SetSpriteXY (0,5,(((Stylus.X*zoom)>>8)+scrollx)+64,(((Stylus.Y*zoom)>>8)+scrolly)+32);
					PA_SetSpriteXY (0,2,(((Stylus.X*zoom)>>8)+scrollx)+64,(((Stylus.Y*zoom)>>8)+scrolly)+48);
				}
			}
			else if(options.staticsprites==1){
				if(Stylus.X<128){
					PA_SetSpriteXY (0,3,224,0);
					PA_SetSpriteXY (0,4,224,16);
					PA_SetSpriteXY (0,5,224,32);
					PA_SetSpriteXY (0,2,224,48);
				}
				if(Stylus.X>128){
					PA_SetSpriteXY (0,3,0,0);
					PA_SetSpriteXY (0,4,0,16);
					PA_SetSpriteXY (0,5,0,32);
					PA_SetSpriteXY (0,2,0,48);
				}
			}
			CnP.OldStylusX=(((Stylus.X*zoom)>>8)+scrollx)-16;
			CnP.OldStylusY=(((Stylus.Y*zoom)>>8)+scrolly)-16;
			Paste(0,(((Stylus.X*zoom)>>8)+scrollx)-16,(((Stylus.Y*zoom)>>8)+scrolly)-16,CnP.xflip[1],CnP.yflip[1],1);
		}
	}
}
void CopyPasteFun(void){			
	if(CnP.useCopy==0){
		int testcolor=PA_RandMinMax (5000,99999);
		CnP.xflip[0]=0;
		CnP.yflip[0]=0;
		if(CanvasLimits()==TRUE)UnfilledRectangle(0,(((Stylus.X*zoom)>>8)+scrollx),(((Stylus.Y*zoom)>>8)+scrolly),testcolor);
		CheckZoom();
	}
	if(CnP.useCopy==2){
	//copy
		if(PA_SpriteTouched(1)&& Stylus.Newpress){
			UndoSlot(MENUBUF);
			Copy(0,Sx1,Sx2,Sy1,Sy2,0);
			PA_WaitForVBL();
			CnP.useCopy++;
			HideSprite(0,1,1);
			HideSprite(0,2,1);
		}
		//cancel
		if(PA_SpriteTouched(4) && Stylus.Newpress){
			UndoSlot(MENUBUF);
			CnP.useCopy=0;
			if(CnP.hasCopy!=0)CnP.hasCopy=2;
			HideSprite(0,1,1);
			HideSprite(0,2,1);
			HideSprite(0,3,1);
			HideSprite(0,4,1);
			HideSprite(0,5,1);
		}
		if(PA_SpriteTouched(3) && Stylus.Newpress){
			//paste
			UndoSlot(MENUBUF);
			CnP.hasCopy=3;
			CnP.useCopy=3;
			HideSprite(0,1,1);
			HideSprite(0,2,1);
			HideSprite(0,5,1);
			HideSprite(0,3,1);
		}
	}
	if(CnP.useCopy==3){						
		if(CnP.hasCopy==0 || CnP.hasCopy==3 || CnP.hasCopy==2){
			CheckSaveUndo();
			CnP.hasCopy=4;
			HideSprite(0,3,0);
			HideSprite(0,4,0);
			HideSprite(0,5,0);
			HideSprite(0,2,0);
			if(options.staticsprites==0){
				if(Stylus.X>128){
					PA_SetSpriteXY (0,3,(((Stylus.X*zoom)>>8)+scrollx)-64,(((Stylus.Y*zoom)>>8)+scrolly));
					PA_SetSpriteXY (0,4,(((Stylus.X*zoom)>>8)+scrollx)-64,(((Stylus.Y*zoom)>>8)+scrolly)+16);
					PA_SetSpriteXY (0,5,(((Stylus.X*zoom)>>8)+scrollx)-64,(((Stylus.Y*zoom)>>8)+scrolly)+32);
					PA_SetSpriteXY (0,2,(((Stylus.X*zoom)>>8)+scrollx)-64,(((Stylus.Y*zoom)>>8)+scrolly)+48);
				}
				if(Stylus.X<128){
					PA_SetSpriteXY (0,3,(((Stylus.X*zoom)>>8)+scrollx)+64,(((Stylus.Y*zoom)>>8)+scrolly));
					PA_SetSpriteXY (0,4,(((Stylus.X*zoom)>>8)+scrollx)+64,(((Stylus.Y*zoom)>>8)+scrolly)+16);
					PA_SetSpriteXY (0,5,(((Stylus.X*zoom)>>8)+scrollx)+64,(((Stylus.Y*zoom)>>8)+scrolly)+32);
					PA_SetSpriteXY (0,2,(((Stylus.X*zoom)>>8)+scrollx)+64,(((Stylus.Y*zoom)>>8)+scrolly)+48);
				}
			}
			else if(options.staticsprites==1){
				if(Stylus.X<128){
					PA_SetSpriteXY (0,3,224,0);
					PA_SetSpriteXY (0,4,224,16);
					PA_SetSpriteXY (0,5,224,32);
					PA_SetSpriteXY (0,2,224,48);
				}
				if(Stylus.X>128){
					PA_SetSpriteXY (0,3,0,0);
					PA_SetSpriteXY (0,4,0,16);
					PA_SetSpriteXY (0,5,0,32);
					PA_SetSpriteXY (0,2,0,48);
				}
			}
			Paste(0,(((Stylus.X*zoom)>>8)+scrollx)-16,(((Stylus.Y*zoom)>>8)+scrolly)-16,CnP.xflip[0],CnP.yflip[0],0);
		}
		//cancel
		if(PA_SpriteTouched(4) && Stylus.Newpress){
			UndoSlot(MENUBUF);
			CnP.useCopy=0;
			CnP.hasCopy=2;
			HideSprite(0,3,1);
			HideSprite(0,4,1);
			HideSprite(0,5,1);
			HideSprite(0,2,1);
			PA_WaitForVBL();
		}
		//paste
		if(PA_SpriteTouched(3) && Stylus.Newpress){
			CnP.useCopy=0;
			CnP.hasCopy=1;
			HideSprite(0,5,1);
			HideSprite(0,2,1);
			HideSprite(0,3,1);
			HideSprite(0,4,1);
			PA_WaitForVBL();
		}
		//Xflip
		if(PA_SpriteTouched(5) && Stylus.Newpress){
			CnP.xflip[0]++;
			UndoSlot(EndSlot);
			CheckFlips(0,0);
			PA_WaitForVBL();
		}
		//Yflip
		if(PA_SpriteTouched(2) && Stylus.Newpress){
			CnP.yflip[0]++;
			UndoSlot(EndSlot);
			CheckFlips(0,0);
			PA_WaitForVBL();
		}
		if(Stylus.Held && !PA_SpriteTouched(3) &&!PA_SpriteTouched(4) &&!PA_SpriteTouched(2)&&!PA_SpriteTouched(5)){
			UndoSlot(EndSlot);
			if(options.staticsprites==0){
				if(Stylus.X>128){
					PA_SetSpriteXY (0,3,(((Stylus.X*zoom)>>8)+scrollx)-64,(((Stylus.Y*zoom)>>8)+scrolly));
					PA_SetSpriteXY (0,4,(((Stylus.X*zoom)>>8)+scrollx)-64,(((Stylus.Y*zoom)>>8)+scrolly)+16);
					PA_SetSpriteXY (0,5,(((Stylus.X*zoom)>>8)+scrollx)-64,(((Stylus.Y*zoom)>>8)+scrolly)+32);
					PA_SetSpriteXY (0,2,(((Stylus.X*zoom)>>8)+scrollx)-64,(((Stylus.Y*zoom)>>8)+scrolly)+48);
				}
				if(Stylus.X<128){
					PA_SetSpriteXY (0,3,(((Stylus.X*zoom)>>8)+scrollx)+64,(((Stylus.Y*zoom)>>8)+scrolly));
					PA_SetSpriteXY (0,4,(((Stylus.X*zoom)>>8)+scrollx)+64,(((Stylus.Y*zoom)>>8)+scrolly)+16);
					PA_SetSpriteXY (0,5,(((Stylus.X*zoom)>>8)+scrollx)+64,(((Stylus.Y*zoom)>>8)+scrolly)+32);
					PA_SetSpriteXY (0,2,(((Stylus.X*zoom)>>8)+scrollx)+64,(((Stylus.Y*zoom)>>8)+scrolly)+48);
				}
			}
			else if(options.staticsprites==1){
				if(Stylus.X<128){
					PA_SetSpriteXY (0,3,224,0);
					PA_SetSpriteXY (0,4,224,16);
					PA_SetSpriteXY (0,5,224,32);
					PA_SetSpriteXY (0,2,224,48);
				}
				if(Stylus.X>128){
					PA_SetSpriteXY (0,3,0,0);
					PA_SetSpriteXY (0,4,0,16);
					PA_SetSpriteXY (0,5,0,32);
					PA_SetSpriteXY (0,2,0,48);
				}
			}
			CnP.OldStylusX=(((Stylus.X*zoom)>>8)+scrollx)-16;
			CnP.OldStylusY=(((Stylus.Y*zoom)>>8)+scrolly)-16;
			Paste(0,(((Stylus.X*zoom)>>8)+scrollx)-16,(((Stylus.Y*zoom)>>8)+scrolly)-16,CnP.xflip[0],CnP.yflip[0],0);
		}
	}
}

