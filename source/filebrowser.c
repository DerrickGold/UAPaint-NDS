
#include <PA9.h>       // Include for PA_Lib
#include <fat.h>
#include <sys/dir.h> 
#include "header.h"


extern inline char PA_GetSmall(char letter){
   if((letter >= 'A')&&(letter <= 'Z')) return (letter-'A'+'a');
   return letter;
}   

void ResetFATSearch(void){
	number = 0;
}
s16 PA_FATSearchExt(char *directory, PAfiletype *filelist, char *ext){
   
   u8 i, j;
   struct stat st;
   	DIR_ITER* dir = diropen(directory);
	if(!dir){
		mkdir("/pics", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		filesfound=0;
	}
   	else filesfound=1;
	
	while (dirnext(dir, filelist[number].filename, &st) == 0 && filesfound==1) {
	   if(!(st.st_mode & S_IFDIR)){// Is a file...
	   	// Check if right extension...
	   	i = 0;
	   	while( filelist[number].filename[i]) i++; // go to end of name
	   	while( filelist[number].filename[i-1] != '.') i--; // get to extension
	   	
	   	u8 same = 1; // same by default
	   	for(j = 0; ext[j]; j++) if(PA_GetSmall(ext[j]) !=  PA_GetSmall(filelist[number].filename[i+j])) same = 0; // not right extension
	   	
	   	if(same){ // Ok, register file
	   	
		   	for(j = 0; j < i-1; j++) filelist[number].name[j] = filelist[number].filename[j];
		   	filelist[number].name[j] = 0;
		   	
		   	for(i = 0; ext[i]; i++) filelist[number].ext[i] = ext[i];
		   	filelist[number].ext[i] = 0;
		   	
		   	filelist[number].size = st.st_size;
		   	
		   	
		   	number++;
		 	}  	
	   
		}   
	}
 
   
   return number;
}  

void PA_LoadFileFat(int filenumber){ 
	//loading bmp files
	if(strcmp (files[filenumber].ext,".bmp")){
		LoadCanvas(1);
		PA_LoadBmpEx (0,Canvas.X,Canvas.Y, (void *)buffer);
	}
	//loading gif files
	else if(strcmp (files[filenumber].ext,".gif")){
		Canvas.drawwidth=PA_GetGifWidth ((void *)buffer);
		Canvas.drawheight=PA_GetGifHeight((void *)buffer);

		LoadCanvas(1);
		PA_LoadGifXY(0,Canvas.X,Canvas.Y,(void*)buffer); 
	}	
}

void LoadPreviewPic(char *directory,int filenumber){
	sprintf(filename,"%s%s.%s",directory,files[filenumber].name,files[filenumber].ext);
	FILE* imgFile=fopen(filename,"rb");	
			
	// obtain file size:
	u32 imgSize;
	fseek (imgFile, 0 , SEEK_END);
	imgSize = ftell (imgFile);
	rewind (imgFile);
	if(strcmp (files[filenumber].ext,".bmp")){
		s16 wd,ht;
		//get width and heights
		fseek(imgFile, 18, SEEK_SET);
		fread(&wd, sizeof(int),1, imgFile);

		fseek(imgFile, 22, SEEK_SET);
		fread(&ht, sizeof(int), 1, imgFile);
		rewind (imgFile);
		
		// allocate memory to store the image file
		if(wd>256 ||ht>192){
			PA_OutputText(1, 5,9+nfile,"Unsupported Image Size");
			PA_WaitFor(Pad.Newpress.Anykey || Stylus.Newpress || Pad.Newpress.Y);
		}
		if(wd<=256 ||ht<=192){
			buffer = (char*) malloc (sizeof(char)*imgSize);
			fread (buffer, 1, imgSize, imgFile);
			Canvas.drawwidth=wd;
			Canvas.drawheight=ht;
			PA_Clear16bitBg(1);
			PA_LoadBmpEx (1,0,0, (void *)buffer);
			

		}
	}	
	else if(strcmp (files[filenumber].ext,".gif")){
		//if(imgSize<=1459){
			buffer = (char*) malloc (sizeof(char)*imgSize);
			fread (buffer, 1, imgSize, imgFile);
			PA_Clear16bitBg(1);
			Canvas.drawwidth=PA_GetGifWidth ((void *)buffer);
			Canvas.drawheight=PA_GetGifHeight((void *)buffer);
			//PA_LoadGifXY(1,0,0,(void*)buffer); 
			PA_LoadGif(1, (void *)buffer); // Show GIF on top screen

		//}
		fclose (imgFile);
		free((void*)buffer);
	}
	fclose (imgFile);
	free((void*)buffer);
}
void LoadStamp(char *filepath, int filenumber,int x,int y,bool disp){
	sprintf(filename,"%s%s.%s",filepath,files[filenumber].name,files[filenumber].ext);
	FILE* imgFile=fopen(filename,"rb");		
	// obtain file size:
	u32 imgSize;
	fseek (imgFile, 0 , SEEK_END);
	imgSize = ftell (imgFile);
	rewind (imgFile);
	//16X16 stamps
	if(imgSize<=822){
		// allocate memory to store the image file
		buffer = (char*) malloc (sizeof(char)*imgSize);
		fread (buffer, 1, imgSize, imgFile);
		if(disp==1){
			PA_LoadBmpEx (1,x,y, (void *)buffer);
			//ClearBuffer();
		}
		
	}
	fclose (imgFile);
}
void ClearBuffer(void){
	free((void*)buffer);
}

inline void InitFatSys(char *directory, char *ext){
	FileNumber = PA_FATSearchExt(directory, files,ext);
}

void ListDirectory(char *directory, bool screen,int x, int y){
	int i;
	for(i=0;i<FileNumber;i++){
		PA_OutputText(screen,x,i+y,files[i].name);
	}
}

