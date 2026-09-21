#include "murks_func072.h"
#include <math.h>
#include <wchar.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <locale.h>
#include <wctype.h>
#include <assert.h>
#include <ctype.h>
#include <iso646.h>
#include <fenv.h>
#include <inttypes.h>
#include <string.h>
#include <signal.h>
void murks_func072(void){
UINT lI=(UINT)fabsf(5.0f); 
UINT Il=0u;
UINT Ol[5][5]={{0u}};
UINT lo[5]={0u};
UINT oI=0;
UINT Io=0;
for/**/( oI=0; oI<lI; oI++){
for/**/( Io=oI+1; Io<lI; Io++){
if/**/( ((oI+Io)%3u)!=0u){
Ol[oI][Io]=1u;
Ol[Io][oI]=1u;
Il=Il+1u;
}
}
}
for/**/( oI=0; oI<lI; oI++){
lo[oI]=0u;
for/**/( Io=0; Io<lI; Io++){
lo[oI]=lo[oI]+Ol[oI][Io];
}
}
switch/**/( Il%4u){
case 0u:{lI=4u; break;}
case 1u:{lI=5u; break;}
case 2u:{lI=6u; break;}
case 3u:{Il=0u; break;}
default:{break;}
}
if/**/( (lo[0]+lo[lI-1u])<(UINT)(lI*2u)){
Il=Il+1u;
}
}
