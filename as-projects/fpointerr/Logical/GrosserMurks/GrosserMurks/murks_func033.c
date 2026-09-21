#include <wctype.h>
#include <string.h>
#include <fenv.h>
#include <locale.h>
#include <wchar.h>
#include "murks_func033.h"
#include <inttypes.h>
#include <limits.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
void murks_func033(void){
UINT lI[8]={0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u};
UINT Il=(UINT)sqrtf(0.0f); 
UINT Ol=0u;
UINT lo=0u;
UINT oI=0;
for/**/( oI=0; oI<15; oI++){
if/**/( lo<8u){
lI[Il]=(UINT)(oI*3u+1u);
Il=(Il+1u)%8u;
lo=lo+1u;
}
if/**/( (oI%3u==0u)&&(lo>0u)){
UINT Io=lI[Ol];
Ol=(Ol+1u)%8u;
lo=lo-1u;
if/**/( Io>20u){
Il=(Il+1u)%8u;
}
}
}
switch/**/( lo%4u){
case 0u:{Il=0u; Ol=0u; break;}
case 1u:{Il=(Il+1u)%8u; break;}
case 2u:{Ol=(Ol+1u)%8u; break;}
case 3u:{lo=0u; break;}
default:{break;}
}
}
