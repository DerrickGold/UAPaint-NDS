#ifndef _GH_KEYS
#define _GH_KEYS

#ifdef __cplusplus
extern "C" {
#endif
#include "PA9.h"

typedef struct {
   u8 Green, Red, Yellow, Blue, Anykey;
} GH_Pad;

typedef struct {
   GH_Pad Held, Released, Newpress; // Pressed si on appuye, Held si on garde appuyé, et Released si on relache
} GHPads;
GHPads GHPad;
   
u16 GHCompletePad, GHExPad, GHTempPad;

void GH_UpdatePad(void);
bool InitGH(void);
bool DetectGrip(void);
void DeIntGH(void);

#ifdef __cplusplus
}
#endif
#endif
