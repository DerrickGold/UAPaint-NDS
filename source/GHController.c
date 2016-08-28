#ifdef __cplusplus
extern "C" {
#endif

#include "GHController.h"

#ifdef ARM9
#define WAIT_CR	REG_EXMEMCNT
#else
#define WAIT_CR	REG_EXMEMSTAT
#endif


#define GH_BUTTONS (*(vuint8*)0x0A000000)
#define BUTTON_BLUE 8
#define BUTTON_YELLOW 16
#define BUTTON_RED 32
#define BUTTON_GREEN 64


extern u16 GHCompletePad, GHExPad, GHTempPad;
#define UPDATEGHPAD(type, pad)  type.Green = (pad & BUTTON_GREEN)>>6;\
   type.Red = (pad & BUTTON_RED) >> 5;\
   type.Yellow = (pad & BUTTON_YELLOW) >> 4;\
   type.Blue = (pad & BUTTON_BLUE) >> 3;\
   type.Anykey = (!(!((pad&120))));

bool DetectGrip(void){
	if(((~GBA_BUS[0]) & 0x0600) == 0x0600) {
      return true;
    } 
	return 0;
}
bool InitGH(void){
	if(((~GBA_BUS[0]) & 0x0600) == 0x0600) {
		WAIT_CR &= ~0x8080;
		return 1;
	}
	return 0;
}

void DeIntGH(void){
	WAIT_CR &= ~0x8000;
}

void GH_UpdatePad(void) {
   GHExPad = GHCompletePad;
   GHCompletePad = ~(GH_BUTTONS );
   UPDATEGHPAD(GHPad.Held, GHCompletePad);
   UPDATEGHPAD(GHPad.Released, (GHExPad & (~GHCompletePad)));
   UPDATEGHPAD(GHPad.Newpress, (GHCompletePad & (~GHExPad)));
}

#ifdef __cplusplus
}
#endif
