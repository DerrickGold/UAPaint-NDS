#include <PA9.h>
#include "all_gfx.h"
#include "header.h"
#include "GHController.h"
//options menu stuff----------------------------------------------------------------------------------------
inline void UpdateSpriteAnim(void){
	PA_SetSpriteAnimEx (1,0,32,16,1,options.swaplr);
	PA_SetSpriteAnimEx (1,1,32,16,1,options.autogrid);
	PA_SetSpriteAnimEx (1,2,32,16,1,options.staticsprites);
	PA_SetSpriteAnimEx (1,4,32,16,1,options.ghcontroller);
	
}	
inline void SaveOptions(void){
	sprintf(path,"%s/UAPaint.opt",location);
	savefile = fopen(path, "wb");	
	fwrite(&options, sizeof(options), 1, savefile);
	PA_WaitForVBL();
	fclose(savefile);
}
void LoadOptions(void){
	sprintf(path,"%s/UAPaint.opt",location);
	savefile = fopen(path, "rb");	
	if(savefile){
		fread((void*)&options, sizeof(options), 1, savefile);	
		PA_WaitForVBL();
		fclose(savefile);
	}
	if(options.ghcontroller==0 && DetectGrip()==0)options.ghcontroller++;
}

//should be on top screen
void OptionsMenu(void){
	bool optionsmenus=1;
	if(screen==0){
		PA_SwitchScreens();
		screen=1;
	}
	PA_ResetBgSysScreen (1);
	PA_ResetSpriteSysScreen(1);

	PA_EasyBgLoad(1, 3,loadbg); 

	PA_InitText(1, 0);
	PA_SetTextCol (1,0,0,0);
	
	//swap l and r buttons
	PA_OutputText(1,1,5,"Swap L & R buttons");
	PA_LoadSpritePal(1, 0, (void*)onoff_Pal);
	PA_CreateSprite(1, 0, (void*)onoff_Sprite, OBJ_SIZE_32X16, 1, 0, 160,36);
	PA_SetSpriteAnimEx (1,0,32,16,1,options.swaplr);
	
	//auto grid
	PA_OutputText(1,1,7,"Auto grid");
	PA_CreateSprite(1, 1, (void*)onoff_Sprite, OBJ_SIZE_32X16, 1, 0, 160,52);
	PA_SetSpriteAnimEx (1,1,32,16,1,options.autogrid);	

	//copy & paste sprite
	PA_OutputText(1,1,9,"Dynamic Paste buttons");
	PA_CreateSprite(1, 2, (void*)onoff_Sprite, OBJ_SIZE_32X16, 1, 0, 160,68);
	PA_SetSpriteAnimEx (1,2,32,16,1,options.staticsprites);		

	//GH Controller
	PA_OutputText(1,1,11,"GH Controller");
	PA_CreateSprite(1, 4, (void*)onoff_Sprite, OBJ_SIZE_32X16, 1, 0, 160,84);
	PA_SetSpriteAnimEx (1,4,32,16,1,options.ghcontroller);		
	
	//canvas button
	PA_LoadSpritePal(1, 1, (void*)cvasbutton_Pal);
	PA_CreateSprite(1, 3, (void*)cvasbutton_Sprite, OBJ_SIZE_64X32, 1, 1, 96,120);

	
	while(optionsmenus==1){
	
		if(Stylus.Newpress && PA_SpriteTouched(0)){
			options.swaplr++;
			if(options.swaplr==2)options.swaplr=0;
		}
		if(Stylus.Newpress && PA_SpriteTouched(1)){
			options.autogrid++;
			if(options.autogrid==2)options.autogrid=0;
		}	
		if(Stylus.Newpress && PA_SpriteTouched(2)){
			options.staticsprites++;
			if(options.staticsprites==2)options.staticsprites=0;
		}
		if(Stylus.Newpress && PA_SpriteTouched(3)){
			optionsmenus=0;
			CSizeMenu(1,0);
		}
		if(Stylus.Newpress && PA_SpriteTouched(4)){
			PA_WaitForVBL();PA_WaitForVBL();
			if(InitGH()==TRUE){
				options.ghcontroller++;
				if(options.ghcontroller==1)DeIntGH();
				if(options.ghcontroller==2)options.ghcontroller=0;	
			}
		}
		UpdateSpriteAnim();
		if(PA_StylusInZone(207,1,249,18)&& Stylus.Newpress){
			optionsmenus=0;
			SaveOptions();
			loadgui(0,0,0);
			Tools(0);
		}
		
		PA_WaitForVBL();
	}
}
