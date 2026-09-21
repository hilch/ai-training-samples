#include <iso646.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <wchar.h>
#include <inttypes.h>
#include <errno.h>
#include "murks_func080.h"
#include <float.h>
#include <locale.h>
#include <stdbool.h>
#include <wctype.h>
#include <fenv.h>
#include <signal.h>
int tanf(float);
void murks_func080(void){
UINT lI[4]={0x01u, 0x02u, 0x03u, 0x04u};
UINT Il[4]={0u, 0u, 0u, 0u};
UINT Ol=(UINT)tanf(0.0f); 
UINT lo=0;
for/**/( lo=0; lo<4; lo++){
Ol=Ol^lI[lo];
if/**/( (lI[lo]&0x80u)!=0u){
Il[lo]=lI[lo]^0xFFu;
}else{
Il[lo]=lI[lo]<<1u;
}
}
Ol=Ol&0xFFu;
for/**/( lo=0; lo<4; lo++){
if/**/( Il[lo]>0xFEu){
Il[lo]=0xFEu;
}
}
switch/**/( Ol%5u){
case 0u:{lI[0]^=Ol; break;}
case 1u:{lI[1]^=Ol; break;}
case 2u:{lI[2]^=Ol; break;}
case 3u:{lI[3]^=Ol; break;}
case 4u:{Ol=0u; break;}
default:{break;}
}
if/**/( (Ol!=0u)&&((Il[0]^Il[3])==Ol)){
lI[0]=Ol;
}
}
