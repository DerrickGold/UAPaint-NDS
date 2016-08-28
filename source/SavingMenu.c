#include <PA9.h>
#include "all_gfx.h"
#include "header.h"
#include "bmp.h"

void KeyBoardMenu(bool savetype){//save type decides if your saving a picture or a pallet
	PA_WaitForVBL();PA_WaitForVBL();
	bool UseKeyboard=1;
	//variables
	int nletter = 0; // Next letter to right. 0 since no letters are there yet
	char letter = 0; // New letter to write.
	char text[200];  // This will be our text.
	
	if(screen==0 && savetype==1)PA_SwitchScreens();

	//clear screen
	PA_ResetBgSysScreen (1);
	PA_ResetSpriteSysScreen(1);
	//load backgrounds
	PA_EasyBgLoad(1, 3,loadbg); 
	PA_InitText(1, 0);
	PA_SetTextCol (1,0,0,0);
	
	//loads teh keyboard
	PA_SetKeyboardScreen(1);
	PA_InitKeyboard(2); // Load the keyboard on background 2...
	
	//save image
	if(savetype==0){
		PA_LoadSpritePal(1, 0, (void*)savebuttons_Pal);
		PA_CreateSprite(1, 0, (void*)savebuttons_Sprite, OBJ_SIZE_64X32, 1, 0,91,35);
		PA_SetSpriteAnim (1,0,0);
		HideSprite(1,0,1);
		if(Canvas.drawheight==16&&Canvas.drawwidth==16){
			//save image
			PA_SetSpriteXY (1,0,56,35);
			HideSprite(1,0,0);
			//save stamp
			PA_CreateSprite(1, 1, (void*)savebuttons_Sprite, OBJ_SIZE_64X32, 1, 0,131,35);
			PA_SetSpriteAnim (1,1,1);
			HideSprite(1,1,0);
		}
		else HideSprite(1,0,0);
	}
	
	PA_KeyboardIn(23, 88); // This scrolls the keyboard from the bottom, until it's at the right position
	PA_OutputSimpleText(1, 5, 9, "Save as: "); 
	
	while(UseKeyboard==1){
	
		letter = PA_CheckKeyboard();
		if(nletter<15){
			if (letter > 31) { // there is a new letter
				if(letter!='/' && letter!='"' && letter!=':'&& letter!='?' && letter!='>'&& letter!='<'&& letter!='|'&&letter!='\\'){
					text[nletter] = letter;
					nletter++;
				}
			}
			else if(letter == PA_TAB){// TAB Pressed...
				u8 i;
				for (i = 0; i < 4; i++){ // put 4 spaces...
					text[nletter] = ' ';
					nletter++;
				}
	
			}
		}
		if ((letter == PA_BACKSPACE)&&nletter) { // Backspace pressed
			nletter--;
			text[nletter] = ' '; // Erase the last letter
		}
		PA_OutputSimpleText(1, 13, 9, text); // Write the text	
		
		if (letter == '\n'){ // Enter pressed
			if(savetype==0){
				UseKeyboard=0;
				screenshotbmp(text,0);//save bitmap
				
				loadgui(0,0,0);
				Tools(0);
			}
			if(savetype==1){
				SavePallet(text);
				UseKeyboard=0;
				ColorCreator(0,0);
			}
		}

		if(PA_StylusInZone(207,1,249,18)&& Stylus.Newpress){
			if(savetype==0){
				UseKeyboard=0;
				loadgui(0,0,0);
				Tools(0);
			}
			if(savetype==1){
				UseKeyboard=0;
				ColorCreator(0,0);
			}
		}
		if(Stylus.Newpress && PA_SpriteTouched(0)){
			screenshotbmp(text,0);//save bitmap
			UseKeyboard=0;
			loadgui(0,0,0);
			Tools(0);
		}
		else if(Stylus.Newpress && PA_SpriteTouched(1)){
			screenshotbmp(text,1);//save bitmap
			UseKeyboard=0;
			loadgui(0,0,0);
			Tools(0);
		}
		PA_WaitForVBL();
	}
}
