#include <PA9.h>  
#include "all_gfx.h"
#include "header.h"

bool delgrid;
inline void CheckWindow(void){
	if(zoom==240)zoom=256;
	if (scrollx>=(256-zoom))scrollx = (256-zoom);
	if (scrollx<=0)scrollx = 0;
	if (scrolly>(192-((zoom*192)>>8)))scrolly = (192-((zoom*192)>>8));
	if (scrolly<0) scrolly = 0;
}
void CheckScroll(void){
	scrollx = centerx - (zoom>>1);
	scrolly = centery - (zoom>>1);	
	
	CheckWindow();
	PA_SetBgRot(0, 3,scrollx,scrolly,0,0,0, zoom);
	//PA_SetRotsetNoAngle(0,0,256-32+zoom,256-32+zoom);
	if(zoom!=256){
		if(Pad.Held.B){
			if(delgrid==1){
				PA_DeleteBg(0,1);
			}
			
			PA_SetBgRot(0, 3,0,0,0,0,0, 256);
		}
		if(Pad.Released.B){
			if(delgrid==1 && zoom>=32)PA_EasyBgLoad(0, 1, grid);
		}
	}
}
void CheckZoom(void){
	//if Zoomed in, allow scrolling
	if (zoom<256) {
		//scrolling when zoomed
		if (Pad.Held.Up) {
			scrolly = scrolly-1;
			centery = scrolly + (zoom >> 1); 
		}
		if (Pad.Held.Down) {
			scrolly = scrolly+1;
			centery = scrolly+ (zoom >> 1); 
		}
		if (Pad.Held.Right) {
			scrollx = scrollx+1;
			centerx = scrollx + (zoom >> 1); 
		}
		if (Pad.Held.Left) {
			scrollx = scrollx-1;
			centerx = scrollx + (zoom >> 1); 
		}
	}
	//zoom stuff
	//if auto grid is off, manually control the grid with b button.
	if(Pad.Newpress.Select){
		if(zoom<=32 &&delgrid==0){
			PA_EasyBgLoad(0, 1, grid);
			delgrid=1;
			PA_WaitForVBL();
		}
		else if(zoom<=32 && delgrid==1){
			PA_DeleteBg(0,1);
			delgrid=0;
			PA_WaitForVBL();
		}
	}
	
	//if auto grid is turned on, load grid 
	if (zoom<32 && options.autogrid==0){
		zoom = 32;
		PA_EasyBgLoad(0, 1, grid);
		delgrid=1;
	}
	else if(zoom<32)zoom = 32;
	//if zooming out a grid is created, del the grid
	if(zoom>32 && delgrid==1){
		PA_DeleteBg(0,1);
		HideSprite(0,0,1);
		DrawMode=0;
		delgrid=0;
	}
	if(zoom>32){
		HideSprite(0,0,1);
		DrawMode=0;
		delgrid=0;
	}
	if (zoom>256) {
		zoom = 256;
		if (zoom==256) {
				scrolly = 0;
				scrollx = 0;
			}
		}	
	//scroll limits
	CheckScroll();
}
