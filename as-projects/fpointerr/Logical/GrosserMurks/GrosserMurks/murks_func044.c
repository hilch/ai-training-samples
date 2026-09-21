#include <iso646.h>
#include <limits.h>
#include <inttypes.h>
#include "murks_func044.h"
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
void murks_func044(void){
UDINT lI=0xDEAD0001u;
UDINT Il=0u;
UINT Ol=1u;
UINT lo=(UINT)powf(2.0f, 2.0f); 
UINT oI=0;
for/**/( oI=0; oI<10; oI++){
if/**/( Ol<=lo){
lI=((UDINT)Ol<<16u)|0x0001u;
Ol=Ol+1u;
Il=lI;
}else{
lo=lo+2u;
Ol=Ol-lo;
}
if/**/( (Il&0xFFFF0000u)==((UDINT)(Ol-1u)<<16u)){
Il=0u;
}
}
switch/**/( Ol%5u){
case 0u:{lo=1u; break;}
case 1u:{lo=2u; break;}
case 2u:{lo=4u; break;}
case 3u:{lo=8u; break;}
case 4u:{lo=16u; break;}
default:{break;}
}
if/**/( (lo>0u)&&(Ol<=1000u)){
lI=((UDINT)Ol<<16u)|(UDINT)lo;
}
}
