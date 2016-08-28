#include <PA9.h>
#include "all_gfx.h"
#include "header.h"
#include "keys.h"

void Update8bitColorSquare(int x,int y){
	int width = 12;
	int height = 12;
	int startx,starty;
	switch(pal){
		case 0:
			//back square
			for(startx=x+7; startx<x+7+width; ++startx){
				for(starty=y+7; starty<y+7+height; ++starty){
					//PA_Put8bitPixel (1,startx,starty,14);
					PA_Put16bitPixel (1,startx,starty,color[1]);
				}
			}
			//front square
			for(startx=x; startx<x+width; ++startx){
				for(starty=y; starty<y+height; ++starty){
					//PA_Put8bitPixel (1,startx,starty,13);
					PA_Put16bitPixel (1,startx,starty,color[0]);
				}
			}
		break;
		case 1:
			//back square
			for(startx=x+7; startx<x+7+width; ++startx){
				for(starty=y+7; starty<y+7+height; ++starty){
					//PA_Put8bitPixel (1,startx,starty,13);
					PA_Put16bitPixel (1,startx,starty,color[0]);
				}
			}
			//front square
			for(startx=x; startx<x+width; ++startx){
				for(starty=y; starty<y+height; ++starty){
					//PA_Put8bitPixel (1,startx,starty,14);
					PA_Put16bitPixel (1,startx,starty,color[1]);
				}
			}
		break;	
		PA_16bitText(0,type.x1,type.y1,type.x2,type.y2,type.text,color[pal],type.fontsize,1,100);
	}

}

inline void TextBoxDirection(void){
	//from top left to bottom right
	if(type.x2>type.x1 && type.y2>type.y1){
		type.width = type.x2-type.x1;
		type.height = type.y2-type.y1;	
	}
	//from top right to bottom left
	else if(type.x2<type.x1 && type.y2>type.y1){
		type.width = type.x1-type.x2;
		type.height = type.y2-type.y1;	
		int tempx1=type.x1;
		//switch type x1 to be top left
		type.x1=type.x2;
		type.x2=tempx1;
	}
	//from bottom leftto top right
	else if(type.x2>type.x1 && type.y2<type.y1){
		type.width = type.x2-type.x1;
		type.height = type.y1-type.y2;	
		//switch type one to be top
		int tempy1=type.y1;
		type.y1=type.y2;
		type.y2=tempy1;
	}
	//from bottom left to top left
	else if(type.x2<type.x1 && type.y2<type.y1){
		type.width = type.x1-type.x2;
		type.height = type.y2-type.y1;	
		//switch type one to be top
		int tempy1=type.y1;
		type.y1=type.y2;
		type.y2=tempy1;
		//switch type x1 to be top left
		int tempx1=type.x1;
		type.x1=type.x2;
		type.x2=tempx1;		
	}
}

inline void ResetKeys(void){
	type.fontsize=1;
	type.letter=0;
	type.nletter=0;
	int loop;
	for(loop=0;loop<199;++loop)type.text[loop] = ' ';
}
void TypeWriterMode(bool BackToKeys){
	bool typewriter=1;
	PA_SwitchScreens();
	screen=1;
	if(BackToKeys==0)ResetKeys();
	else if(BackToKeys==1){
		PA_ResetSpriteSysScreen(0);
		PA_ResetBgSysScreen (0);
		PA_Init16bitBg (0,3);
		LoadCanvas(0);
		UndoSlot(MENUBUF);
		PA_16bitText(0,type.x1,type.y1,type.x2,type.y2,type.text,color[pal],type.fontsize,1,100);
	}
	//init background
	PA_ResetBgSysScreen (1);
	PA_ResetSpriteSysScreen(1);
	PA_Init16bitBg (1,3);
	PA_LoadGif(	1, (void*)keys);
	
	PA_SetKeyboardScreen(1);
	PA_InitKeyboard(1);

	PA_SetBgPalCol(1,13,PA_RGB(color[0]&31,color[0]>>5&31,color[0]>>10&31));
	PA_SetBgPalCol(1,14,PA_RGB(color[1]&31,color[1]>>5&31,color[1]>>10&31));
	
	PA_LoadSpritePal(1, 3, (void*)palls_Pal);
	PA_CreateSprite(1, 4, (void*)palls_Sprite, OBJ_SIZE_32X32, 1, 3, 219,26);
	Update8bitColorSquare(220,38);
	
	PA_KeyboardIn(23, 88); // This scrolls the keyboard from the bottom, until it's at the right positio
	TextBoxDirection();
	
	bool NewTextBox=0;

	while(typewriter==1){
		if(screen==1){
			type.letter = PA_CheckKeyboard();
			if (type.letter > 31) { // there is a new letter
				switch(type.fontsize){
					case 1:
					case 2:
						if(PA_LetterPos.Letter[type.nletter].X>=type.x2-8){
							type.text[type.nletter] = '\n';
							type.nletter++;	
						}
					break;
					case 3:
						if(PA_LetterPos.Letter[type.nletter].X>=type.x2-10){
							type.text[type.nletter] = '\n';
							type.nletter++;	
						}
					break;
					case 4:
						if(PA_LetterPos.Letter[type.nletter].X>=type.x2-14){
							type.text[type.nletter] = '\n';
							type.nletter++;	
						}
					break;
				}
				type.text[type.nletter] = type.letter;
				type.nletter++;

			}
			else if(type.letter == PA_TAB){// TAB Pressed...
				u8 i;
				for (i = 0; i < 4; i++){ // put 4 spaces...
					type.text[type.nletter] = ' ';
					type.nletter++;
				}
	
			}
			else if ((type.letter == PA_BACKSPACE)&&type.nletter) { // Backspace pressed
				type.nletter--;
				type.text[type.nletter] = ' '; // Erase the last letter
			}
			else if (type.letter == '\n'){ // Enter pressed
				type.text[type.nletter] = type.letter;
				type.nletter++;
			}

			if(Stylus.Released){
				UndoSlot(MENUBUF);
				PA_16bitText(0,type.x1,type.y1,type.x2,type.y2,type.text,color[pal],type.fontsize,1,9999);
				ClearGreyAreas();
			}
		
			if(PA_StylusInZone(207,1,249,18)&& Stylus.Newpress){
				typewriter=0;
				PA_WaitForVBL();PA_WaitForVBL();
				UndoSlot(MENUBUF);
				loadgui(0,0,0);
				Tools(0);
			}
			//font sizes
			if(PA_StylusInZone(78,35,98,55)&& Stylus.Newpress)type.fontsize=1;
			if(PA_StylusInZone(102,35,122,55)&& Stylus.Newpress)type.fontsize=2;
			if(PA_StylusInZone(126,35,146,55)&& Stylus.Newpress)type.fontsize=3;
			if(PA_StylusInZone(150,35,170,55)&& Stylus.Newpress)type.fontsize=4;
			if(PA_SpriteTouched(4)&&Stylus.Newpress){
				//PA_16bitText(0,type.x1,type.y1,type.x2,type.y2,type.text,color[pal],type.fontsize,1,100);
				UndoSlot(EndSlot);
				typewriter=0;
				ColorCreator(1,1);
			}
			//ok button
			if(PA_StylusInZone(109,63,144,84)&& Stylus.Newpress){
				typewriter=0;
				PA_WaitForVBL();PA_WaitForVBL();
				//PA_16bitText(0,type.x1,type.y1,type.x2,type.y2,type.text,color[pal],type.fontsize,1,100);
				loadgui(0,0,0);
				Tools(0);
			}
			//new boarder button
			if(PA_StylusInZone(192,37,210,54)&& Stylus.Newpress){
				UndoSlot(MENUBUF);
				PA_16bitText(0,type.x1,type.y1,type.x2,type.y2,type.text,color[pal],type.fontsize,1,9999);
				NewTextBox=1;
				screen=0;
				PA_SwitchScreens();
				PA_WaitForVBL();PA_WaitForVBL();
				
			}
		}
		if(screen==0){
		
			if(Stylus.Held && NewTextBox==0){
				UndoSlot(MENUBUF);
				type.x1=Stylus.X;
				type.x2=Stylus.X+type.width;
				type.y1=Stylus.Y;
				type.y2=Stylus.Y+type.height;
				PA_16bitText(0,type.x1,type.y1,type.x2,type.y2,type.text,color[pal],type.fontsize,1,9999);
				ClearGreyAreas();
			}
			if(Stylus.Newpress&& NewTextBox==1){
				
				TextBoarder(0,(((Stylus.X*zoom)>>8)+scrollx),(((Stylus.Y*zoom)>>8)+scrolly),color[pal],0);
				if(Stylus.Released && NewTextBox==1){
				
					ResetKeys();
					NewTextBox=0;
					TextBoxDirection();
					screen=1;
					PA_SwitchScreens();
					PA_WaitForVBL();PA_WaitForVBL();
					PA_UpdatePad();
				}
			}

		}
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
					PA_WaitForVBL();PA_WaitForVBL();
					Update8bitColorSquare(220,38);
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
					PA_WaitForVBL();PA_WaitForVBL();
					Update8bitColorSquare(220,38);
				}
			break;
		}
		PA_WaitForVBL();
	}
}
