#include <PA9.h>
#include <fat.h>
#include <sys/dir.h> 
#include "header.h"
#include "bmp.h"


extern inline void write16(u16* address, u16 value) {

	u8* first=(u8*)address;
	u8* second=first+1;

	*first=value&0xff;
	*second=value>>8;
}

extern inline void write32(u32* address, u32 value) {

	u8* first=(u8*)address;
	u8* second=first+1;
	u8* third=first+2;
	u8* fourth=first+3;

	*first=value&0xff;
	*second=(value>>8)&0xff;
	*third=(value>>16)&0xff;
	*fourth=(value>>24)&0xff;
}

char tempfilename[256];
void screenshotbmp(char* filename, bool stamp) {

	if(stamp==0){
		sprintf(tempfilename,"/pics/%s.bmp",filename);
	}
	else if(stamp==1){
		sprintf(tempfilename,"/%s/stamps/%s.bmp",location,filename);	
	}
	//DIR_ITER* dir = diropen("/pics");//check for directory
	char templocation[256];
	bool dir=PA_Locate("/","pics",true,10,templocation);
	if(!dir)mkdir("/pics", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);//if there is no directory, create one
	FILE* file=fopen(tempfilename, "wb");//create file
	//write headers
	u8* temp=(u8*)malloc(Canvas.drawwidth*Canvas.drawheight*3+sizeof(INFOHEADER)+sizeof(HEADER));

	HEADER* header=(HEADER*)temp;
	INFOHEADER* infoheader=(INFOHEADER*)(temp+sizeof(HEADER));

	write16(&header->type, 0x4D42);
	write32(&header->size, Canvas.drawwidth*Canvas.drawheight*3+sizeof(INFOHEADER)+sizeof(HEADER));
	write32(&header->offset, sizeof(INFOHEADER)+sizeof(HEADER));
	write16(&header->reserved1, 0);
	write16(&header->reserved2, 0);

	write16(&infoheader->bits, 24);
	write32(&infoheader->size, sizeof(INFOHEADER));
	write32(&infoheader->compression, 0);
	write32(&infoheader->width, Canvas.drawwidth);
	write32(&infoheader->height, Canvas.drawheight);
	write16(&infoheader->planes, 1);
	write32(&infoheader->imagesize, Canvas.drawwidth*Canvas.drawheight*3);
	write32(&infoheader->xresolution, 0);
	write32(&infoheader->yresolution, 0);
	write32(&infoheader->importantcolours, 0);
	write32(&infoheader->ncolours, 0);
	s16 y,x;
	//bitmaps are saved backwards
	for(x=Canvas.X;x<Canvas.X+Canvas.drawwidth;x++){
		for(y=Canvas.Y;y<Canvas.Y+Canvas.drawheight;y++){
			u16 color=PA_Get16bitPixel(0, x, (Canvas.Y+Canvas.drawheight)-(y-Canvas.Y));
			s16 blue=(color&31)<<3;
			s16 green=((color>>5)&31)<<3;
			s16 red=((color>>10)&31)<<3;

			temp[(((y-Canvas.Y)*Canvas.drawwidth)+(x-Canvas.X))*3+sizeof(INFOHEADER)+sizeof(HEADER)]=red;
			temp[(((y-Canvas.Y)*Canvas.drawwidth)+(x-Canvas.X))*3+1+sizeof(INFOHEADER)+sizeof(HEADER)]=green;
			temp[(((y-Canvas.Y)*Canvas.drawwidth)+(x-Canvas.X))*3+2+sizeof(INFOHEADER)+sizeof(HEADER)]=blue;
		}
	}
	
	//write data
	fwrite(temp, 1, Canvas.drawwidth*Canvas.drawheight*3+sizeof(INFOHEADER)+sizeof(HEADER), file);
	PA_WaitForVBL();PA_WaitForVBL();
	fclose(file);
	free(temp);
	PA_WaitForVBL();PA_WaitForVBL();


}

