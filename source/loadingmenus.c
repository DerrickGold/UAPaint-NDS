#include <PA9.h>
#include "all_gfx.h"
#include "header.h"
inline void UpdateCursor(int updatepic,int x, int y){
	if(updatepic==1)LoadPreviewPic("/pics/",nfile);
	PA_OutputSimpleText(1,x,nfile-1+y," ");
	PA_OutputSimpleText(1,x,nfile+y,">");
	PA_OutputSimpleText(1,x,nfile+1+y," ");
}

//Picture browsing menu----------------------------------------------------------------------------------------------------------
void LoadMenu(void){
	bool loadmenu=1;
	PA_ResetBgSysScreen (1);
	PA_ResetSpriteSysScreen(1);

	PA_Init16bitBg(1, 3);
	PA_SetBgRot(1, 3,-240,-25,0,0,0,512);

	PA_InitText(1, 0);
	PA_SetBgColor(1,PA_RGB(29, 29, 29));
	PA_SetTextCol (1,0,0,0);
	
	//init file browser and scan for pucture files
	ResetFATSearch();
	InitFatSys("/pics/","bmp");
	//if no files found, create dirctory and display error
	if(filesfound==0){
		PA_OutputText(1,7,9,"No pictures found");
		PA_LoadSpritePal(1, 5, (void*)backbutton_Pal);
		PA_CreateSprite(1, 5, (void*)backbutton_Sprite, OBJ_SIZE_64X32, 1, 5, 94,110);
	}
	//if pictures foud, display directory contents
	else{
		ListDirectory("/pics/",1,1,3);
		UpdateCursor(1,0,3);
		PA_LoadSpritePal(1, 4, (void*)button_Pal);
		PA_CreateSprite(1, 4, (void*)button_Sprite, OBJ_SIZE_64X32, 1, 4, 150,110);
		PA_LoadSpritePal(1, 5, (void*)backbutton_Pal);
		PA_CreateSprite(1, 5, (void*)backbutton_Sprite, OBJ_SIZE_64X32, 1, 5, 150,147);
	}
	while(loadmenu==1){
		if(Stylus.Newpress&&PA_SpriteTouched(4)){
			PA_LoadFileFat(nfile);
			loadmenu=0;
			loadgui(0,0,0);
			Tools(0);
			
		}
		if(Stylus.Newpress&&PA_SpriteTouched(5)){
			loadmenu=0;
			loadgui(0,0,0);
			Tools(0);
			
		}
		
		if (Pad.Newpress.Down){
			nfile+=1;
			if (nfile>=FileNumber)nfile=FileNumber-1;
			UpdateCursor(1,0,3);
		}
		else if (Pad.Newpress.Up){
			nfile-=1;
			if (nfile<0)nfile+=1;
			UpdateCursor(1,0,3);
		}
		if(nfile>20)PA_BGScrollXY(1,0,0,((nfile-20)*8));
		else if(nfile<21)PA_BGScrollXY(1,0,0,0);
		
		PA_WaitForVBL();
	}
}

//color pallet loading menu----------------------------------------------------------------------------------------------------------
void LoadPallet(char *directory,int filenumber){
	sprintf(filename,"%s%s.%s",directory,files[filenumber].name,files[filenumber].ext);
	FILE* imgFile=fopen(filename,"rb");	
	if(imgFile){
		fread((void*)&tempcolors, sizeof(tempcolors), 1, imgFile);	
		PA_WaitForVBL();
		fclose(imgFile);
	}
}

void SavePallet(char* filename){
	char tempfilename[256];
	sprintf(tempfilename,"%s/pal/%s.uap",location,filename);
	savefile = fopen(tempfilename, "wb");	
	fwrite(&tempcolors, sizeof(tempcolors), 1, savefile);
	fclose(savefile);
}

void LoadDefaultPallet(void){
	sprintf(path,"%s/pal/default.uap",location);
	savefile = fopen(path, "rb");	
	if(savefile){
		fread((void*)&tempcolors, sizeof(tempcolors), 1, savefile);	
		PA_WaitForVBL();
		fclose(savefile);
	}
}

void LoadPalMenu(void){
	PA_WaitForVBL();PA_WaitForVBL();
	int palmenu=1;
	PA_SwitchScreens();
	screen=1;
	
	PA_ResetBgSysScreen (1);
	PA_ResetSpriteSysScreen(1);
	PA_EasyBgLoad(1, 3,loadbg); 
	
	PA_InitText(1, 0);
	PA_SetTextCol (1,0,0,0);
	
	//init file browser and scan for pucture files
	ResetFATSearch();
	sprintf(path,"%s/pal/",location);
	InitFatSys(path,"uap");
	//if no files found, create dirctory and display error
	if(filesfound==0){
		PA_OutputText(1,7,9,"No pallets found");
		PA_LoadSpritePal(1, 5, (void*)backbutton_Pal);
		PA_CreateSprite(1, 5, (void*)backbutton_Sprite, OBJ_SIZE_64X32, 1, 5, 94,110);
	}
	//if pictures foud, display directory contents
	else{
		sprintf(path,"%s/pal/",location);
		ListDirectory(path,1,3,5);
		UpdateCursor(0,2,5);
		PA_LoadSpritePal(1, 4, (void*)button_Pal);
		PA_CreateSprite(1, 4, (void*)button_Sprite, OBJ_SIZE_64X32, 1, 4, 150,110);
		PA_LoadSpritePal(1, 5, (void*)backbutton_Pal);
		PA_CreateSprite(1, 5, (void*)backbutton_Sprite, OBJ_SIZE_64X32, 1, 5, 150,147);
	}
	while(palmenu==1){
		if(Stylus.Newpress&&PA_SpriteTouched(4)){
			sprintf(path,"%s/pal/",location);
			LoadPallet(path,nfile);
			palmenu=0;
			screen=0;
			ColorCreator(0,0);	
		}
		if(Stylus.Newpress&&PA_SpriteTouched(5)){
			palmenu=0;
			screen=0;
			ColorCreator(0,0);	
		}
		
		if (Pad.Newpress.Down){
			nfile+=1;
			if (nfile>=FileNumber)nfile=FileNumber-1;
			UpdateCursor(0,2,5);
		}
		else if (Pad.Newpress.Up){
			nfile-=1;
			if (nfile<0)nfile+=1;
			UpdateCursor(0,2,5);
		}
		PA_EasyBgScrollXY (1,0,0,nfile*8);
		PA_WaitForVBL();
	}
}
